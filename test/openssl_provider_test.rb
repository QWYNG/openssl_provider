# frozen_string_literal: true

require "test_helper"

class ProviderTest < Test::Unit::TestCase
  def test_openssl_provider_name_inspect
    provider = OpenSSL::Provider.load("default")
    assert_equal("default", provider.name)
    assert_not_nil(provider.inspect)
  end

  # default provider does not support RC4
  def test_openssl_legacy_provider_with_rc4
    legacy = OpenSSL::Provider.load("legacy")
    algo = "RC4"
    data = "a" * 1000
    key = OpenSSL::Random.random_bytes(16)

    cipher = OpenSSL::Cipher.new(algo)
    cipher.encrypt
    cipher.key = key
    encrypted = cipher.update(data) + cipher.final

    other_cipher = OpenSSL::Cipher.new(algo)
    other_cipher.decrypt
    other_cipher.key = key
    decrypted = other_cipher.update(encrypted) + other_cipher.final
    OpenSSL::Provider.unload(legacy)

    assert_equal(data, decrypted)
  end

  def test_openssl_providers
    orig = OpenSSL::Provider.providers
    legacy = OpenSSL::Provider.load("legacy")
    assert_equal(1, OpenSSL::Provider.providers.size - orig.size)

    OpenSSL::Provider.unload(legacy)
  end
end
