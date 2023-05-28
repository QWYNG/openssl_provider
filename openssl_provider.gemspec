# frozen_string_literal: true

require_relative "lib/openssl_provider/version"

Gem::Specification.new do |spec|
  spec.name = "openssl_provider"
  spec.version = OpensslProvider::VERSION
  spec.authors = ["qwyng"]
  spec.email = ["ikusawasi@gmail.com"]

  spec.summary = "Provides APIs to load OpenSSL providers"
  spec.homepage = "https://github.com/QWYNG/openssl_provider"
  spec.license = "MIT"
  spec.required_ruby_version = ">= 3.1.0"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/QWYNG/openssl_provider"
  spec.metadata["changelog_uri"] = "https://github.com/QWYNG/openssl_provider/blob/master/CHANGELOG.md"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files = Dir.chdir(__dir__) do
    `git ls-files -z`.split("\x0").reject do |f|
      (f == __FILE__) || f.match(%r{\A(?:(?:bin|test|spec|features)/|\.(?:git|travis|circleci)|appveyor)})
    end
  end
  spec.bindir = "exe"
  spec.executables = spec.files.grep(%r{\Aexe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_dependency "openssl", "~> 3.0"

  # For more information and examples about making a new gem, check out our
  # guide at: https://bundler.io/guides/creating_gem.html
end
