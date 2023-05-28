#include <openssl/opensslv.h>

#define OSSL_OPENSSL_PREREQ(maj, min, pat) \
      (OPENSSL_VERSION_NUMBER >= ((maj << 28) | (min << 20) | (pat << 12)))

#if OSSL_OPENSSL_PREREQ(3, 0, 0)

#if !defined(OSSL_PROVIDER_H)
#define OSSL_PROVIDER_H

extern VALUE cProvider;
extern VALUE eProviderError;

void Init_openssl_provider(void);
#endif