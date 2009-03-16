/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: sampleconv.c,v 1.1 2006-04-09 08:39:08 bagder Exp $
 */
/*
   This is a simple example showing how a program on a non-ASCII platform
   would invoke callbacks to do its own codeset conversions instead of
   using the built-in iconv functions in libcurl.

   The IBM-1047 EBCDIC codeset is used for this example but the code
   would be similar for other non-ASCII codesets.

   Three callback functions are created below:
        my_conv_from_ascii_to_ebcdic,
        my_conv_from_ebcdic_to_ascii, and
        my_conv_from_utf8_to_ebcdic

   The "platform_xxx" calls represent platform-specific conversion routines.

 */

#include <stdio.h>
#include <curl/curl.h>

CURLcode my_conv_from_ascii_to_ebcdic(char *buffer, size_t length)
{
    char *tempptrin, *tempptrout;
    size_t bytes = length;
    int rc;
    tempptrin = tempptrout = buffer;
    rc = platform_a2e(&tempptrin, &bytes, &tempptrout, &bytes);
    if (rc == PLATFORM_CONV_OK) {
      return(CURLE_OK);
    } else {
      return(CURLE_CONV_FAILED);
    }
}

CURLcode my_conv_from_ebcdic_to_ascii(char *buffer, size_t length)
{
    char *tempptrin, *tempptrout;
    size_t bytes = length;
    int rc;
    tempptrin = tempptrout = buffer;
    rc = platform_e2a(&tempptrin, &bytes, &tempptrout, &bytes);
    if (rc == PLATFORM_CONV_OK) {
      return(CURLE_OK);
    } else {
      return(CURLE_CONV_FAILED);
    }
}

CURLcode my_conv_from_utf8_to_ebcdic(char *buffer, size_t length)
{
    char *tempptrin, *tempptrout;
    size_t bytes = length;
    int rc;
    tempptrin = tempptrout = buffer;
    rc = platform_u2e(&tempptrin, &bytes, &tempptrout, &bytes);
    if (rc == PLATFORM_CONV_OK) {
      return(CURLE_OK);
    } else {
      return(CURLE_CONV_FAILED);
    }
}

int main(void)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "curl.haxx.se");

    /* use platform-specific functions for codeset conversions */
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                     my_conv_from_ascii_to_ebcdic);
    curl_easy_setopt(curl, CURLOPT_CONV_TO_NETWORK_FUNCTION,
                     my_conv_from_ebcdic_to_ascii);
    curl_easy_setopt(curl, CURLOPT_CONV_FROM_UTF8_FUNCTION,
                     my_conv_from_utf8_to_ebcdic);

    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}
