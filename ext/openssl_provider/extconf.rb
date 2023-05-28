# frozen_string_literal: true

require "mkmf"
result = pkg_config("openssl") && have_header("openssl/ssl.h")
unless result
  Logging::message "openssl could not be found"
  exit 1
end

create_makefile("openssl_provider")
