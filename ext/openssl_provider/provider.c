#include <ruby.h>
#include <openssl/provider.h>

VALUE cProvider;
VALUE eProviderError;

#define NewProvider(klass) \
    TypedData_Wrap_Struct((klass), &ossl_provider_type, 0)
#define SetProvider(obj, provider) do { \
    if (!(provider)) { \
	rb_raise(rb_eRuntimeError, "Provider wasn't initialized."); \
    } \
    RTYPEDDATA_DATA(obj) = (provider); \
} while(0)
#define GetProvider(obj, provider) do { \
    TypedData_Get_Struct((obj), OSSL_PROVIDER, &ossl_provider_type, (provider)); \
    if (!(provider)) { \
        rb_raise(rb_eRuntimeError, "PROVIDER wasn't initialized."); \
    } \
} while (0)

static const rb_data_type_t ossl_provider_type = {
    "OpenSSL/Provider",
    {
	0
    },
    0, 0, RUBY_TYPED_FREE_IMMEDIATELY,
};

static VALUE
ossl_provider_s_load(VALUE klass, VALUE name)
{
    OSSL_PROVIDER *provider = NULL;
    VALUE obj;

    const char *provider_name_ptr = StringValueCStr(name);

    provider = OSSL_PROVIDER_load(NULL, provider_name_ptr);
    if (provider == NULL) {
      rb_raise(eProviderError, "Failed to load %s provider\n", provider_name_ptr);
    }
    obj = NewProvider(klass);
    SetProvider(obj, provider);

    return obj;
}

static VALUE
ossl_provider_s_unload(VALUE klass, VALUE obj)
{
    OSSL_PROVIDER *prov;
    GetProvider(obj, prov);

    int result = OSSL_PROVIDER_unload(prov);

    if (result != 1) {
      return Qfalse;
    }
    return Qtrue;
}

DEFINE_STACK_OF(OSSL_PROVIDER)
static int provider_cmp(const OSSL_PROVIDER * const *a,
                        const OSSL_PROVIDER * const *b)
{
    return strcmp(OSSL_PROVIDER_get0_name(*a), OSSL_PROVIDER_get0_name(*b));
}
static int collect_providers(OSSL_PROVIDER *provider, void *stack)
{
    STACK_OF(OSSL_PROVIDER) *provider_stack = stack;

    sk_OSSL_PROVIDER_push(provider_stack, provider);
    return 1;
}

static VALUE
ossl_provider_s_providers(VALUE klass)
{
    STACK_OF(OSSL_PROVIDER) *providers = sk_OSSL_PROVIDER_new(provider_cmp);
    VALUE ary = rb_ary_new();

    OSSL_PROVIDER_do_all(NULL, &collect_providers, providers);
    sk_OSSL_PROVIDER_sort(providers);
    for (int i = 0; i < sk_OSSL_PROVIDER_num(providers); i++) {
        OSSL_PROVIDER *provider = sk_OSSL_PROVIDER_value(providers, i);
        VALUE obj = NewProvider(klass);
        SetProvider(obj, provider);

        rb_ary_push(ary, obj);
    }
    sk_OSSL_PROVIDER_free(providers);
    return ary;
}

static VALUE
ossl_provider_get_name(VALUE self)
{
    OSSL_PROVIDER *prov;
    GetProvider(self, prov);

    return rb_str_new2(OSSL_PROVIDER_get0_name(prov));
}

static VALUE
ossl_provider_inspect(VALUE self)
{
    OSSL_PROVIDER *prov;
    GetProvider(self, prov);

    return rb_sprintf("#<%"PRIsVALUE" name=\"%s\">",
		      rb_obj_class(self), OSSL_PROVIDER_get0_name(prov));
}

void
Init_openssl_provider(void)
{

    VALUE mOSSL;
    VALUE eOSSLError;

    rb_require("openssl");
    mOSSL = rb_path2class("OpenSSL");
    eOSSLError = rb_path2class("OpenSSL::OpenSSLError");
    cProvider = rb_define_class_under(mOSSL, "Provider", rb_cObject);

    eProviderError = rb_define_class_under(cProvider, "ProviderError", eOSSLError);
    rb_define_singleton_method(cProvider, "load", ossl_provider_s_load, 1);
    rb_define_singleton_method(cProvider, "unload", ossl_provider_s_unload, 1);
    rb_define_singleton_method(cProvider, "providers", ossl_provider_s_providers, 0);

    rb_define_method(cProvider, "name", ossl_provider_get_name, 0);
    rb_define_method(cProvider, "inspect", ossl_provider_inspect, 0);
}
