/* This trivial wrapper can be used for native extension
 * currently unused.
 */

#include <ruby.h>

#ifndef RSTRING_PTR
#define RSTRING_PTR(str) (RSTRING(str)->ptr)
#endif

extern char *minify(char *);

static VALUE minify_wrap(VALUE self, VALUE arg)
{
	char *input;
	char *res;
	VALUE str, rv;

	str = StringValue(arg);
	input = RSTRING_PTR(str);

	res = minify(input);

	if (res[0] == '!') {
		free(res);
		rb_raise(rb_eRuntimeError, "Parse error");
	}

	rv = rb_str_new2(res);
	free(res);

	return rv;
}

VALUE mJsmin;

void Init_jsmin(void) {
	mJsmin = rb_define_module("Jsmin");
	rb_define_module_function(mJsmin, "minify", minify_wrap, 1);
}
