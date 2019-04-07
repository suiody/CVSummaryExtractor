//
// File Patch.cpp
//
//

#include <openssl/ssl.h>

#if defined(OPENSSL_NO_SSL2)
// backport to make neon happy with only OpenSSL 1.0.0 available
extern "C" const SSL_METHOD *
SSLv2_client_method(void)
{
    // use SSL v2 and v3 even if only v2 is requested
    return SSLv23_client_method();
}

extern "C" const SSL_METHOD *
SSLv2_server_method(void)
{
    // use SSL v2 and v3 even if only v2 is requested
    return SSLv23_server_method();
}

extern "C" const SSL_METHOD *
SSLv2_method(void)
{
    // use SSL v2 and v3 even if only v2 is requested
    return SSLv23_method();
}
#endif

