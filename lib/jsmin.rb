require 'ffi'

module Jsmin
  extend FFI::Library
  ffi_lib File.dirname(__FILE__) + '/libjsmin.so.0'
  attach_function :minify, [:string], :string

  class ParseException < RuntimeError; end

  extend self

  # Minify the provided javascript
  # @param [String] buf Javascript text
  # @return [String] minified Javascript text
  def process!(buf)
    res = minify(buf)
    if res[0..0] == '!'
      raise ParseException.new(res.tr('!', ''))
    end
    res
  end
end
