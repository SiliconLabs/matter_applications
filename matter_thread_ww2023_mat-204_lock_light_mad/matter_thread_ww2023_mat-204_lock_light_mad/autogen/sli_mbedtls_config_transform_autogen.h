// This is an autogenerated config file, any changes to this file will be overwritten

#ifndef SLI_MBEDTLS_CONFIG_TRANSFORM_AUTOGEN_H
#define SLI_MBEDTLS_CONFIG_TRANSFORM_AUTOGEN_H

// Convert CMSIS Markup config defines to mbedTLS specific config defines

// Allow undefining the specified cipher suites
#if defined(SLI_MBEDTLS_AUTODETECT_CIPHERSUITES)
#undef MBEDTLS_SSL_CIPHERSUITES
#endif

#if SL_MBEDTLS_SSL_MAX_FRAGMENT_LENGTH
  #define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH
#endif
#if SL_MBEDTLS_SSL_EXPORT_KEYS
  #define MBEDTLS_SSL_EXPORT_KEYS
#endif
#if SL_MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
  #define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
#endif
#if SL_MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED
  #define MBEDTLS_KEY_EXCHANGE_ECDHE_PSK_ENABLED
#endif
#if SL_MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
  #define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#endif
#if SL_MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
  #define MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED
#endif

#if !defined(MBEDTLS_SSL_IN_CONTENT_LEN)
  #define MBEDTLS_SSL_IN_CONTENT_LEN SL_MBEDTLS_SSL_IN_CONTENT_LEN
#endif

#if !defined(MBEDTLS_SSL_OUT_CONTENT_LEN)
  #define MBEDTLS_SSL_OUT_CONTENT_LEN SL_MBEDTLS_SSL_OUT_CONTENT_LEN
#endif


#define SLI_SSL_IN_CONTENT_LEN_REQUIREMENT    900

// If the SDK requires a certain SSL buffer size, configure Mbed TLS
// to size the buffer to the highest of the user/default or SDK setting.
#if MBEDTLS_SSL_IN_CONTENT_LEN < SLI_SSL_IN_CONTENT_LEN_REQUIREMENT
  #undef MBEDTLS_SSL_IN_CONTENT_LEN
  #define MBEDTLS_SSL_IN_CONTENT_LEN SLI_SSL_IN_CONTENT_LEN_REQUIREMENT
#endif



#define SLI_SSL_OUT_CONTENT_LEN_REQUIREMENT    900

// If the SDK requires a certain SSL buffer size, configure Mbed TLS
// to size the buffer to the highest of the user/default or SDK setting.
#if MBEDTLS_SSL_OUT_CONTENT_LEN < SLI_SSL_OUT_CONTENT_LEN_REQUIREMENT
  #undef MBEDTLS_SSL_OUT_CONTENT_LEN
  #define MBEDTLS_SSL_OUT_CONTENT_LEN SLI_SSL_OUT_CONTENT_LEN_REQUIREMENT
#endif


#endif // SLI_MBEDTLS_CONFIG_TRANSFORM_AUTOGEN_H
