/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2008, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * $Id: hostip.c,v 1.214 2008-11-06 17:19:57 yangtse Exp $
 ***************************************************************************/

#include "setup.h"

#include <string.h>

#ifdef NEED_MALLOC_H
#include <malloc.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>     /* required for free() prototypes */
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>     /* for the close() proto */
#endif
#ifdef  VMS
#include <in.h>
#include <inet.h>
#include <stdlib.h>
#endif

#ifdef HAVE_SETJMP_H
#include <setjmp.h>
#endif
#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAVE_PROCESS_H
#include <process.h>
#endif

#include "urldata.h"
#include "sendf.h"
#include "hostip.h"
#include "hash.h"
#include "share.h"
#include "strerror.h"
#include "url.h"
#include "inet_ntop.h"

#define _MPRINTF_REPLACE /* use our functions only */
#include <curl/mprintf.h>

#include "memory.h"
/* The last #include file should be: */
#include "memdebug.h"

#if defined(HAVE_ALARM) && defined(SIGALRM) && defined(HAVE_SIGSETJMP) \
    && !defined(USE_ARES)
/* alarm-based timeouts can only be used with all the dependencies satisfied */
#define USE_ALARM_TIMEOUT
#endif

/*
 * hostip.c explained
 * ==================
 *
 * The main COMPILE-TIME DEFINES to keep in mind when reading the host*.c
 * source file are these:
 *
 * CURLRES_IPV6 - this host has getaddrinfo() and family, and thus we use
 * that. The host may not be able to resolve IPv6, but we don't really have to
 * take that into account. Hosts that aren't IPv6-enabled have CURLRES_IPV4
 * defined.
 *
 * CURLRES_ARES - is defined if libcurl is built to use c-ares for
 * asynchronous name resolves. This can be Windows or *nix.
 *
 * CURLRES_THREADED - is defined if libcurl is built to run under (native)
 * Windows, and then the name resolve will be done in a new thread, and the
 * supported API will be the same as for ares-builds.
 *
 * If any of the two previous are defined, CURLRES_ASYNCH is defined too. If
 * libcurl is not built to use an asynchronous resolver, CURLRES_SYNCH is
 * defined.
 *
 * The host*.c sources files are split up like this:
 *
 * hostip.c   - method-independent resolver functions and utility functions
 * hostasyn.c - functions for asynchronous name resolves
 * hostsyn.c  - functions for synchronous name resolves
 * hostares.c - functions for ares-using name resolves
 * hostthre.c - functions for threaded name resolves
 * hostip4.c  - ipv4-specific functions
 * hostip6.c  - ipv6-specific functions
 *
 * The hostip.h is the united header file for all this. It defines the
 * CURLRES_* defines based on the config*.h and setup.h defines.
 */

/* These two symbols are for the global DNS cache */
static struct curl_hash hostname_cache;
static int host_cache_initialized;

static void freednsentry(void *freethis);

/*
 * Curl_global_host_cache_init() initializes and sets up a global DNS cache.
 * Global DNS cache is general badness. Do not use. This will be removed in
 * a future version. Use the share interface instead!
 *
 * Returns a struct curl_hash pointer on success, NULL on failure.
 */
struct curl_hash *Curl_global_host_cache_init(void)
{
  int rc = 0;
  if(!host_cache_initialized) {
    rc = Curl_hash_init(&hostname_cache, 7, Curl_hash_str,
                        Curl_str_key_compare, freednsentry);
    if(!rc)
      host_cache_initialized = 1;
  }
  return rc?NULL:&hostname_cache;
}

/*
 * Destroy and cleanup the global DNS cache
 */
void Curl_global_host_cache_dtor(void)
{
  if(host_cache_initialized) {
    Curl_hash_clean(&hostname_cache);
    host_cache_initialized = 0;
  }
}

/*
 * Return # of adresses in a Curl_addrinfo struct
 */
int Curl_num_addresses(const Curl_addrinfo *addr)
{
  int i = 0;
  while(addr) {
    addr = addr->ai_next;
    i++;
  }
  return i;
}

/*
 * Curl_printable_address() returns a printable version of the 1st address
 * given in the 'ai' argument. The result will be stored in the buf that is
 * bufsize bytes big.
 *
 * If the conversion fails, it returns NULL.
 */
const char *
Curl_printable_address(const Curl_addrinfo *ai, char *buf, size_t bufsize)
{
  const struct sockaddr_in *sa4;
  const struct in_addr *ipaddr4;
#ifdef ENABLE_IPV6
  const struct sockaddr_in6 *sa6;
  const struct in6_addr *ipaddr6;
#endif

  switch (ai->ai_family) {
    case AF_INET:
      sa4 = (const void *)ai->ai_addr;
      ipaddr4 = &sa4->sin_addr;
      return Curl_inet_ntop(ai->ai_family, (const void *)ipaddr4, buf, bufsize);
#ifdef ENABLE_IPV6
    case AF_INET6:
      sa6 = (const void *)ai->ai_addr;
      ipaddr6 = &sa6->sin6_addr;
      return Curl_inet_ntop(ai->ai_family, (const void *)ipaddr6, buf, bufsize);
#endif
    default:
      break;
  }
  return NULL;
}

/*
 * Return a hostcache id string for the providing host + port, to be used by
 * the DNS caching.
 */
static char *
create_hostcache_id(const char *server, int port)
{
  /* create and return the new allocated entry */
  return aprintf("%s:%d", server, port);
}

struct hostcache_prune_data {
  long cache_timeout;
  time_t now;
};

/*
 * This function is set as a callback to be called for every entry in the DNS
 * cache when we want to prune old unused entries.
 *
 * Returning non-zero means remove the entry, return 0 to keep it in the
 * cache.
 */
static int
hostcache_timestamp_remove(void *datap, void *hc)
{
  struct hostcache_prune_data *data =
    (struct hostcache_prune_data *) datap;
  struct Curl_dns_entry *c = (struct Curl_dns_entry *) hc;

  if((data->now - c->timestamp < data->cache_timeout) ||
      c->inuse) {
    /* please don't remove */
    return 0;
  }

  /* fine, remove */
  return 1;
}

/*
 * Prune the DNS cache. This assumes that a lock has already been taken.
 */
static void
hostcache_prune(struct curl_hash *hostcache, long cache_timeout, time_t now)
{
  struct hostcache_prune_data user;

  user.cache_timeout = cache_timeout;
  user.now = now;

  Curl_hash_clean_with_criterium(hostcache,
                                 (void *) &user,
                                 hostcache_timestamp_remove);
}

/*
 * Library-wide function for pruning the DNS cache. This function takes and
 * returns the appropriate locks.
 */
void Curl_hostcache_prune(struct SessionHandle *data)
{
  time_t now;

  if((data->set.dns_cache_timeout == -1) || !data->dns.hostcache)
    /* cache forever means never prune, and NULL hostcache means
       we can't do it */
    return;

  if(data->share)
    Curl_share_lock(data, CURL_LOCK_DATA_DNS, CURL_LOCK_ACCESS_SINGLE);

  time(&now);

  /* Remove outdated and unused entries from the hostcache */
  hostcache_prune(data->dns.hostcache,
                  data->set.dns_cache_timeout,
                  now);

  if(data->share)
    Curl_share_unlock(data, CURL_LOCK_DATA_DNS);
}

/*
 * Check if the entry should be pruned. Assumes a locked cache.
 */
static int
remove_entry_if_stale(struct SessionHandle *data, struct Curl_dns_entry *dns)
{
  struct hostcache_prune_data user;

  if( !dns || (data->set.dns_cache_timeout == -1) || !data->dns.hostcache)
    /* cache forever means never prune, and NULL hostcache means
       we can't do it */
    return 0;

  time(&user.now);
  user.cache_timeout = data->set.dns_cache_timeout;

  if( !hostcache_timestamp_remove(&user,dns) )
    return 0;

  Curl_hash_clean_with_criterium(data->dns.hostcache,
                                 (void *) &user,
                                 hostcache_timestamp_remove);

  return 1;
}


#ifdef HAVE_SIGSETJMP
/* Beware this is a global and unique instance. This is used to store the
   return address that we can jump back to from inside a signal handler. This
   is not thread-safe stuff. */
sigjmp_buf curl_jmpenv;
#endif


/*
 * Curl_cache_addr() stores a 'Curl_addrinfo' struct in the DNS cache.
 *
 * When calling Curl_resolv() has resulted in a response with a returned
 * address, we call this function to store the information in the dns
 * cache etc
 *
 * Returns the Curl_dns_entry entry pointer or NULL if the storage failed.
 */
struct Curl_dns_entry *
Curl_cache_addr(struct SessionHandle *data,
                Curl_addrinfo *addr,
                const char *hostname,
                int port)
{
  char *entry_id;
  size_t entry_len;
  struct Curl_dns_entry *dns;
  struct Curl_dns_entry *dns2;
  time_t now;

  /* Create an entry id, based upon the hostname and port */
  entry_id = create_hostcache_id(hostname, port);
  /* If we can't create the entry id, fail */
  if(!entry_id)
    return NULL;
  entry_len = strlen(entry_id);

  /* Create a new cache entry */
  dns = calloc(sizeof(struct Curl_dns_entry), 1);
  if(!dns) {
    free(entry_id);
    return NULL;
  }

  dns->inuse = 0;   /* init to not used */
  dns->addr = addr; /* this is the address(es) */

  /* Store the resolved data in our DNS cache. This function may return a
     pointer to an existing struct already present in the hash, and it may
     return the same argument we pass in. Make no assumptions. */
  dns2 = Curl_hash_add(data->dns.hostcache, entry_id, entry_len+1,
                       (void *)dns);
  if(!dns2) {
    /* Major badness, run away. */
    free(dns);
    free(entry_id);
    return NULL;
  }
  time(&now);
  dns = dns2;

  dns->timestamp = now; /* used now */
  dns->inuse++;         /* mark entry as in-use */

  /* free the allocated entry_id again */
  free(entry_id);

  return dns;
}

/*
 * Curl_resolv() is the main name resolve function within libcurl. It resolves
 * a name and returns a pointer to the entry in the 'entry' argument (if one
 * is provided). This function might return immediately if we're using asynch
 * resolves. See the return codes.
 *
 * The cache entry we return will get its 'inuse' counter increased when this
 * function is used. You MUST call Curl_resolv_unlock() later (when you're
 * done using this struct) to decrease the counter again.
 *
 * Return codes:
 *
 * CURLRESOLV_ERROR   (-1) = error, no pointer
 * CURLRESOLV_RESOLVED (0) = OK, pointer provided
 * CURLRESOLV_PENDING  (1) = waiting for response, no pointer
 */

int Curl_resolv(struct connectdata *conn,
                const char *hostname,
                int port,
                struct Curl_dns_entry **entry)
{
  char *entry_id = NULL;
  struct Curl_dns_entry *dns = NULL;
  size_t entry_len;
  struct SessionHandle *data = conn->data;
  CURLcode result;
  int rc = CURLRESOLV_ERROR; /* default to failure */

  *entry = NULL;

  /* Create an entry id, based upon the hostname and port */
  entry_id = create_hostcache_id(hostname, port);
  /* If we can't create the entry id, fail */
  if(!entry_id)
    return rc;

  entry_len = strlen(entry_id);

  if(data->share)
    Curl_share_lock(data, CURL_LOCK_DATA_DNS, CURL_LOCK_ACCESS_SINGLE);

  /* See if its already in our dns cache */
  dns = Curl_hash_pick(data->dns.hostcache, entry_id, entry_len+1);

  /* See whether the returned entry is stale. Done before we release lock */
  if( remove_entry_if_stale(data, dns) )
    dns = NULL; /* the memory deallocation is being handled by the hash */

  if(dns) {
    dns->inuse++; /* we use it! */
    rc = CURLRESOLV_RESOLVED;
  }

  if(data->share)
    Curl_share_unlock(data, CURL_LOCK_DATA_DNS);

  /* free the allocated entry_id again */
  free(entry_id);

  if(!dns) {
    /* The entry was not in the cache. Resolve it to IP address */

    Curl_addrinfo *addr;
    int respwait;

    /* Check what IP specifics the app has requested and if we can provide it.
     * If not, bail out. */
    if(!Curl_ipvalid(data))
      return CURLRESOLV_ERROR;

    /* If Curl_getaddrinfo() returns NULL, 'respwait' might be set to a
       non-zero value indicating that we need to wait for the response to the
       resolve call */
    addr = Curl_getaddrinfo(conn, hostname, port, &respwait);

    if(!addr) {
      if(respwait) {
        /* the response to our resolve call will come asynchronously at
           a later time, good or bad */
        /* First, check that we haven't received the info by now */
        result = Curl_is_resolved(conn, &dns);
        if(result) /* error detected */
          return CURLRESOLV_ERROR;
        if(dns)
          rc = CURLRESOLV_RESOLVED; /* pointer provided */
        else
          rc = CURLRESOLV_PENDING; /* no info yet */
      }
    }
    else {
      if(data->share)
        Curl_share_lock(data, CURL_LOCK_DATA_DNS, CURL_LOCK_ACCESS_SINGLE);

      /* we got a response, store it in the cache */
      dns = Curl_cache_addr(data, addr, hostname, port);

      if(data->share)
        Curl_share_unlock(data, CURL_LOCK_DATA_DNS);

      if(!dns)
        /* returned failure, bail out nicely */
        Curl_freeaddrinfo(addr);
      else
        rc = CURLRESOLV_RESOLVED;
    }
  }

  *entry = dns;

  return rc;
}

#ifdef USE_ALARM_TIMEOUT 
/*
 * This signal handler jumps back into the main libcurl code and continues
 * execution.  This effectively causes the remainder of the application to run
 * within a signal handler which is nonportable and could lead to problems.
 */
static
RETSIGTYPE alarmfunc(int sig)
{
  /* this is for "-ansi -Wall -pedantic" to stop complaining!   (rabe) */
  (void)sig;
  siglongjmp(curl_jmpenv, 1);
  return;
}
#endif /* USE_ALARM_TIMEOUT */

/*
 * Curl_resolv_timeout() is the same as Curl_resolv() but specifies a
 * timeout.  This function might return immediately if we're using asynch
 * resolves. See the return codes.
 *
 * The cache entry we return will get its 'inuse' counter increased when this
 * function is used. You MUST call Curl_resolv_unlock() later (when you're
 * done using this struct) to decrease the counter again.
 *
 * If built with a synchronous resolver and use of signals is not
 * disabled by the application, then a nonzero timeout will cause a
 * timeout after the specified number of milliseconds. Otherwise, timeout
 * is ignored.
 *
 * Return codes:
 *
 * CURLRESOLV_TIMEDOUT(-2) = warning, time too short or previous alarm expired
 * CURLRESOLV_ERROR   (-1) = error, no pointer
 * CURLRESOLV_RESOLVED (0) = OK, pointer provided
 * CURLRESOLV_PENDING  (1) = waiting for response, no pointer
 */

int Curl_resolv_timeout(struct connectdata *conn,
                        const char *hostname,
                        int port,
                        struct Curl_dns_entry **entry,
                        long timeoutms)
{
#ifdef USE_ALARM_TIMEOUT 
#ifdef HAVE_SIGACTION
  struct sigaction keep_sigact;   /* store the old struct here */
  bool keep_copysig=FALSE;        /* did copy it? */
  struct sigaction sigact;
#else
#ifdef HAVE_SIGNAL
  void (*keep_sigact)(int);       /* store the old handler here */
#endif /* HAVE_SIGNAL */
#endif /* HAVE_SIGACTION */
  volatile long timeout;
  unsigned int prev_alarm=0;
  struct SessionHandle *data = conn->data;
#endif /* USE_ALARM_TIMEOUT */
  int rc;

  *entry = NULL;

#ifdef USE_ALARM_TIMEOUT 
  if (data->set.no_signal)
    /* Ignore the timeout when signals are disabled */
    timeout = 0;
  else
    timeout = timeoutms;

  if(timeout && timeout < 1000)
    /* The alarm() function only provides integer second resolution, so if
       we want to wait less than one second we must bail out already now. */
    return CURLRESOLV_TIMEDOUT;

  if (timeout > 0) {
    /* This allows us to time-out from the name resolver, as the timeout
       will generate a signal and we will siglongjmp() from that here.
       This technique has problems (see alarmfunc). */
      if(sigsetjmp(curl_jmpenv, 1)) {
        /* this is coming from a siglongjmp() after an alarm signal */
        failf(data, "name lookup timed out");
        return CURLRESOLV_ERROR;
      }

    /*************************************************************
     * Set signal handler to catch SIGALRM
     * Store the old value to be able to set it back later!
     *************************************************************/
#ifdef HAVE_SIGACTION
    sigaction(SIGALRM, NULL, &sigact);
    keep_sigact = sigact;
    keep_copysig = TRUE; /* yes, we have a copy */
    sigact.sa_handler = alarmfunc;
#ifdef SA_RESTART
    /* HPUX doesn't have SA_RESTART but defaults to that behaviour! */
    sigact.sa_flags &= ~SA_RESTART;
#endif
    /* now set the new struct */
    sigaction(SIGALRM, &sigact, NULL);
#else /* HAVE_SIGACTION */
    /* no sigaction(), revert to the much lamer signal() */
#ifdef HAVE_SIGNAL
    keep_sigact = signal(SIGALRM, alarmfunc);
#endif
#endif /* HAVE_SIGACTION */

    /* alarm() makes a signal get sent when the timeout fires off, and that
       will abort system calls */
    prev_alarm = alarm((unsigned int) (timeout/1000L));
  }

#else
#ifndef CURLRES_ASYNCH
  if(timeoutms)
    infof(conn->data, "timeout on name lookup is not supported\n");
#else
  (void)timeoutms; /* timeoutms not used with an async resolver */
#endif
#endif /* USE_ALARM_TIMEOUT */

  /* Perform the actual name resolution. This might be interrupted by an
   * alarm if it takes too long.
   */
  rc = Curl_resolv(conn, hostname, port, entry);

#ifdef USE_ALARM_TIMEOUT 
  if (timeout > 0) {

#ifdef HAVE_SIGACTION
    if(keep_copysig) {
      /* we got a struct as it looked before, now put that one back nice
         and clean */
      sigaction(SIGALRM, &keep_sigact, NULL); /* put it back */
    }
#else
#ifdef HAVE_SIGNAL
    /* restore the previous SIGALRM handler */
    signal(SIGALRM, keep_sigact);
#endif
#endif /* HAVE_SIGACTION */

    /* switch back the alarm() to either zero or to what it was before minus
       the time we spent until now! */
    if(prev_alarm) {
      /* there was an alarm() set before us, now put it back */
      unsigned long elapsed_ms = Curl_tvdiff(Curl_tvnow(), conn->created);

      /* the alarm period is counted in even number of seconds */
      unsigned long alarm_set = prev_alarm - elapsed_ms/1000;

      if(!alarm_set ||
         ((alarm_set >= 0x80000000) && (prev_alarm < 0x80000000)) ) {
        /* if the alarm time-left reached zero or turned "negative" (counted
           with unsigned values), we should fire off a SIGALRM here, but we
           won't, and zero would be to switch it off so we never set it to
           less than 1! */
        alarm(1);
        rc = CURLRESOLV_TIMEDOUT;
        failf(data, "Previous alarm fired off!");
      }
      else
        alarm((unsigned int)alarm_set);
    }
    else
      alarm(0); /* just shut it off */
  }
#endif /* USE_ALARM_TIMEOUT */

  return rc;
}

/*
 * Curl_resolv_unlock() unlocks the given cached DNS entry. When this has been
 * made, the struct may be destroyed due to pruning. It is important that only
 * one unlock is made for each Curl_resolv() call.
 */
void Curl_resolv_unlock(struct SessionHandle *data, struct Curl_dns_entry *dns)
{
  DEBUGASSERT(dns && (dns->inuse>0));

  if(data->share)
    Curl_share_lock(data, CURL_LOCK_DATA_DNS, CURL_LOCK_ACCESS_SINGLE);

  dns->inuse--;

  if(data->share)
    Curl_share_unlock(data, CURL_LOCK_DATA_DNS);
}

/*
 * File-internal: free a cache dns entry.
 */
static void freednsentry(void *freethis)
{
  struct Curl_dns_entry *p = (struct Curl_dns_entry *) freethis;

  if(p) {
    Curl_freeaddrinfo(p->addr);
    free(p);
  }
}

/*
 * Curl_mk_dnscache() creates a new DNS cache and returns the handle for it.
 */
struct curl_hash *Curl_mk_dnscache(void)
{
  return Curl_hash_alloc(7, Curl_hash_str, Curl_str_key_compare, freednsentry);
}

#ifdef CURLRES_ADDRINFO_COPY

/* align on even 64bit boundaries */
#define MEMALIGN(x) ((x)+(8-(((unsigned long)(x))&0x7)))

/*
 * Curl_addrinfo_copy() performs a "deep" copy of a hostent into a buffer and
 * returns a pointer to the malloc()ed copy. You need to call free() on the
 * returned buffer when you're done with it.
 */
Curl_addrinfo *Curl_addrinfo_copy(const void *org, int port)
{
  const struct hostent *orig = org;

  return Curl_he2ai(orig, port);
}
#endif /* CURLRES_ADDRINFO_COPY */
