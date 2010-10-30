require 'ffi'

module JsminFFI
  extend FFI::Library
  ffi_lib File.dirname(__FILE__) + '/Jsmin.' + (RUBY_PLATFORM=~/darwin/ ? 'bundle' : 'so')
  attach_function :minify, [:string], :string

  class ParseError < RuntimeError; end

  extend self

  # Minify the provided javascript
  # @param [String] buf Javascript text
  # @return [String] minified Javascript text
  # @raise [ParseError] when parsing fails
  def minify!(buf)
    res = minify(buf)
    if res[0..0] == '!'
      raise ParseError.new(res.tr('!', ''))
    end
    res
  end
end
