# OpenSSL::Provider
The concept of provider was introduced with OpenSSL 3.
https://www.openssl.org/docs/man3.0/man7/migration_guide.html

With OpenSSL 3, it is possible to specify the provider to be used by any application, either programmatically or through a configuration file.
However, `ruby/openssl` 3.1.0 does not have an API to load providers without configuration file.
This Gem provides `OpenSSL::Provider` class with APIs to load providers.


## Installation

Install the gem and add to the application's Gemfile by executing:

    $ bundle add openssl_provider

If bundler is not being used to manage dependencies, install the gem by executing:

    $ gem install openssl_provider

## Usage

```ruby
require 'openssl_provider'

legacy = OpenSSL::Provider.load("legacy")

OpenSSL::Provider.providers
# => [#<OpenSSL::Provider name="default">, #<OpenSSL::Provider name="legacy">]

# default provider does not support RC4
# OpenSSL::Cipher.new("RC4")
# => OpenSSL::Cipher::CipherError: unsupported
cipher = OpenSSL::Cipher.new("RC4")

# do something with cipher...

OpenSSL::Provider.unload(legacy)
```

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/QWYNG/openssl_provider. This project is intended to be a safe, welcoming space for collaboration, and contributors are expected to adhere to the [code of conduct](https://github.com/QWYNG/openssl_provider/blob/main/CODE_OF_CONDUCT.md).

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Code of Conduct

Everyone interacting in the OpensslProvider project's codebases, issue trackers, chat rooms and mailing lists is expected to follow the [code of conduct](https://github.com/QWYNG/openssl_provider/blob/main/CODE_OF_CONDUCT.md).
