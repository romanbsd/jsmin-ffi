/* This trivial wrapper can be used for native extension
 * currently unused.
 */

#include <ruby.h>

#ifndef RSTRING_PTR
#define RSTRING_PTR(str) (RSTRING(str)->ptr)
#endif

extern char *minify(char *);

static VALUE rb_eParseError;

/*
 *  call-seq:
 *     Jsmin.minify(str)   -> new_str
 *
 *  Returns a new string object containing a minified copy of <i>str</i>.
 *  May raise Jsmin::ParseError if parsing fails.
 */
static VALUE minify_wrap(VALUE self, VALUE arg)
{
	char *input;
	char *res;
	VALUE str, rv;

	str = StringValue(arg);
	input = RSTRING_PTR(str);

	res = minify(input);

	if (res[0] == '!') {
		rv = rb_str_new2(res+1);
		free(res);
		rb_raise(rb_eParseError, "%s", RSTRING_PTR(rv));
	}

	rv = rb_str_new2(res);
	free(res);

	return rv;
}

static VALUE mJsmin;

void Init_Jsmin(void) {
	mJsmin = rb_define_module("Jsmin");
	rb_eParseError = rb_define_class_under(mJsmin, "ParseError", rb_eRuntimeError);
	rb_define_module_function(mJsmin, "minify", minify_wrap, 1);
}
