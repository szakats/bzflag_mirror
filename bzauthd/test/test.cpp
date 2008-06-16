#include <stdio.h>
#include <conio.h>
#define LDAP_DEPRECATED 1
#include <ldap.h>
#include <gcrypt.h>

bool test_ret(int ret)
{
    if(ret != LDAP_SUCCESS)
    {
        fprintf(stderr, "ERROR: %s\n", ldap_err2string(ret));
        return false;
    }
    else
        return true;
}

#define TEST(exp) if(!test_ret(exp)) return;

struct AttrValPair
{
    char *attr;
    char *vals[2];
};

AttrValPair t[] = {
    {"cn", {"Babs Jensen", NULL} },
    {"objectClass", {"person", NULL} },
    {"sn", {"Jensen", NULL} },
    {"description", {"the world's most famous mythical manager", NULL} },
};

#define NUM_ATTRS (sizeof(t)/sizeof(AttrValPair))

LDAP *ld = NULL;

void test_bind()
{
    int version = LDAP_VERSION3;

    TEST( ldap_initialize(&ld, "ldap://127.0.0.1") );
	TEST( ldap_set_option(ld, LDAP_OPT_PROTOCOL_VERSION, &version) );
    TEST( ldap_bind_s(ld, "cn=Manager,dc=my-domain,dc=com", "secret", LDAP_AUTH_SIMPLE) );
    printf("LOG: bind successful\n");
}

void test_add()
{
    LDAPMod *attrs[NUM_ATTRS+1];
    LDAPMod attr[NUM_ATTRS];

    for(int i = 0; i < NUM_ATTRS; i++)
    {
        attr[i].mod_op = LDAP_MOD_ADD; 
        attr[i].mod_type = t[i].attr;
        attr[i].mod_values = t[i].vals;
        attrs[i] = &attr[i];
    }
    attrs[NUM_ATTRS] = NULL;

    TEST( ldap_add_s(ld, "cn=Barbara Jensen,dc=my-domain,dc=com", attrs) );
    printf("LOG: add successful\n");
}

void test_delete()
{
    TEST( ldap_delete_s(ld, "cn=Barbara Jensen,dc=my-domain,dc=com") );
    printf("LOG: delete successful\n");
}

void test_search()
{
    LDAPMessage *res, *msg;
    TEST( ldap_search_s(ld, "dc=my-domain,dc=com", LDAP_SCOPE_SUBTREE, "(objectClass=person)", NULL, 0, &res) );

    for (msg = ldap_first_message(ld, res); msg; msg = ldap_next_message(ld, msg))
    {
        switch(ldap_msgtype(msg))
        {
            case LDAP_RES_SEARCH_ENTRY:
            {
                // print the dn
                printf("LOG: found match\n");
                char *dn = ldap_get_dn(ld, msg);
                printf("dn: %s\n", dn);
                ldap_memfree(dn);
                // print the values of the attributes
                BerElement *ber_itr;
                for(char *attr = ldap_first_attribute(ld, msg, &ber_itr); attr; attr = ldap_next_attribute(ld, msg, ber_itr))
                {
                    printf("%s:", attr);
                    char **values = ldap_get_values(ld, msg, attr);
                    int nrvalues = ldap_count_values(values);
                    for(int i = 0; i < nrvalues; i++)
                    {
                        if(i > 0) printf(";");
                        printf(" %s", values[i]);
                    }
                    printf("\n");
                    ldap_value_free(values);
                    ldap_memfree(attr);
                }
                ber_free(ber_itr, 0); // 0 or 1 ?
            } break;
            case LDAP_RES_SEARCH_RESULT:
            {
                int errcode;
                char *matcheddn;
                char *errmsg;
                char **referrals;
                LDAPControl **serverctrls;
                if(!test_ret( ldap_parse_result(ld, msg, &errcode, &matcheddn, &errmsg, &referrals, &serverctrls, 0) ))
                    break;
                if(test_ret(errcode))
                    printf("LOG: search successful\n");

                if(errmsg)
                {
                    if(errmsg[0]) printf("ERROR: %s\n", errmsg);
                    ldap_memfree(errmsg);
                }
                if(matcheddn)
                {
                    if(matcheddn[0]) printf("Matched dn: %s\n", matcheddn);
                    ldap_memfree(matcheddn);
                }
                if(referrals)
                {
                    ldap_value_free(referrals);
                }
                if(serverctrls)
                {
                    ldap_controls_free(serverctrls);
                }
            } break;
            default:
                printf("LOG: unknown message type %d\n", ldap_msgtype(msg));
        }
    }
    
    ldap_msgfree(res);
    
}

void test_modify()
{
    char* attr = "description";
    char* values[2] = {"just another gal", NULL};

    LDAPMod mod;
    mod.mod_op = LDAP_MOD_REPLACE;
    mod.mod_type = attr;
    mod.mod_values = values;

    LDAPMod *mods[2] = {&mod, NULL};
    TEST( ldap_modify_s(ld, "cn=Barbara Jensen,dc=my-domain,dc=com", mods) );
    printf("LOG: modify successful\n");
}

void test_unbind()
{
    TEST( ldap_unbind(ld) );
    printf("LOG: unbind successful\n");
}


void test_ldap()
{
    test_bind();
    test_add();
    test_modify();
    test_search();
    test_delete();
    test_unbind();
}

void test_gcry()
{
    gcry_control (GCRYCTL_DISABLE_SECMEM, 0);

    gcry_ac_key_pair_t key_pair;
    gcry_ac_key_spec_rsa_t rsa_spec;
    gcry_ac_handle_t handle;
    gcry_error_t err;

    rsa_spec.e = gcry_mpi_new (0);
    gcry_mpi_set_ui (rsa_spec.e, 1);

    err = gcry_ac_open  (&handle, GCRY_AC_RSA, 0);
    if(err) return;

    err = gcry_ac_key_pair_generate(handle, 1024, (void*) &rsa_spec, &key_pair, NULL);
    if(err) return;

    gcry_ac_key_t public_key, secret_key;
    public_key = gcry_ac_key_pair_extract(key_pair, GCRY_AC_KEY_PUBLIC);
    secret_key = gcry_ac_key_pair_extract(key_pair, GCRY_AC_KEY_SECRET);

    char message[] = "let's see if this gets encrypted/decrypted properly";
    char *cipher = NULL;
    int cipher_len;

    gcry_ac_io_t io_message, io_cipher;
    gcry_ac_io_init(&io_message, GCRY_AC_IO_READABLE, GCRY_AC_IO_STRING, message, strlen(message));
    gcry_ac_io_init(&io_cipher, GCRY_AC_IO_WRITABLE, GCRY_AC_IO_STRING, &cipher, &cipher_len);

    err = gcry_ac_data_encrypt_scheme(handle, GCRY_AC_ES_PKCS_V1_5, 0, NULL, public_key, &io_message, &io_cipher);
    if(err) return;

    printf("encrypted: %s\n", cipher);

    char *output = NULL;
    int output_len;
 
    gcry_ac_io_t io_output, io_cip;
    gcry_ac_io_init(&io_cip, GCRY_AC_IO_READABLE, GCRY_AC_IO_STRING, cipher, cipher_len);
    gcry_ac_io_init(&io_output, GCRY_AC_IO_WRITABLE, GCRY_AC_IO_STRING, &output, &output_len);

    err = gcry_ac_data_decrypt_scheme(handle, GCRY_AC_ES_PKCS_V1_5, 0, NULL, secret_key, &io_cip, &io_output);
    if(err) return;

    output[output_len] = 0;

    printf("decrypted: %s\n", output);

    // TODO: these must be deallocated somehow but this way it crashes
    free(cipher);
    free(output);
    gcry_ac_close(handle);
}

int main(int argc, char* argv[])
{
    test_gcry();
    test_ldap();
    getch();

	return 0;
}