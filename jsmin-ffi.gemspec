# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{jsmin-ffi}
  s.version = "0.1.0"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Roman Shterenzon"]
  s.date = %q{2009-06-20}
  s.description = %q{Very fast jsmin implementation using FFI}
  s.email = %q{romanbsd@yahoo.com}
  s.extensions = ["ext/extconf.rb"]
  s.extra_rdoc_files = [
    "README.textile"
  ]
  s.files = [
    "README.textile",
     "Rakefile",
     "VERSION",
     "ext/Makefile",
     "ext/extconf.rb",
     "ext/jsmin.cpp",
     "ext/jsmin.h",
     "lib/jsmin.rb"
  ]
  s.homepage = %q{http://github.com/romanbsd/jsmin-ffi}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.rubygems_version = %q{1.3.4}
  s.summary = %q{Very fast jsmin implementation using FFI}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
      s.add_runtime_dependency(%q<ffi>, [">= 0.3.5"])
    else
      s.add_dependency(%q<ffi>, [">= 0.3.5"])
    end
  else
    s.add_dependency(%q<ffi>, [">= 0.3.5"])
  end
end
