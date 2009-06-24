/* jsmin.c
   2008-08-03

Copyright (c) 2002 Douglas Crockford  (www.crockford.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

The Software shall be used for Good, not Evil.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "jsmin.h"

Jsmin::Jsmin()
{
  index_in = 0;
  index_out = 0;
  theLookahead = 0;
  output_buf = NULL;
  input_buf = NULL;
}


/* isAlphanum -- return true if the character is a letter, digit, underscore,
        dollar sign, or non-ASCII character.
*/

int Jsmin::isAlphanum(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ||
        (c >= 'A' && c <= 'Z') || c == '_' || c == '$' || c == '\\' ||
        c > 126);
}


/* get -- return the next character from stdin. Watch out for lookahead. If
        the character is a control character, translate it to a space or
        linefeed.
*/

int Jsmin::get()
{
    int c = theLookahead;
    theLookahead = 0;
    if (c == 0) {
        c = input_buf[index_in++];
    }
    if (c >= ' ' || c == '\n' || c == 0) {
        return c;
    }
    if (c == '\r') {
        return '\n';
    }
    return ' ';
}


/* peek -- get the next character without getting it.
*/

int Jsmin::peek()
{
    theLookahead = get();
    return theLookahead;
}


/* next -- get the next character, excluding comments. peek() is used to see
        if a '/' is followed by a '/' or '*'.
*/

int Jsmin::next()
{
    int c = get();
    if  (c == '/') {
        switch (peek()) {
        case '/':
            for (;;) {
                c = get();
                if (c <= '\n') {
                    return c;
                }
            }
        case '*':
            get();
            for (;;) {
                switch (get()) {
                case '*':
                    if (peek() == '/') {
                        get();
                        return ' ';
                    }
                    break;
                case 0:
                    free(output_buf);
                    throw("!Unterminated comment");
                }
            }
        default:
            return c;
        }
    }
    return c;
}


/* action -- do something! What you do is determined by the argument:
        1   Output A. Copy B to A. Get the next B.
        2   Copy B to A. Get the next B. (Delete A).
        3   Get the next B. (Delete B).
   action treats a string as a single character. Wow!
   action recognizes a regular expression if it is preceded by ( or , or =.
*/

void Jsmin::action(int d)
{
    switch (d) {
    case 1:
	output_buf[index_out++] = theA;
    case 2:
        theA = theB;
        if (theA == '\'' || theA == '"') {
            for (;;) {
	        output_buf[index_out++] = theA;
                theA = get();
                if (theA == theB) {
                    break;
                }
                if (theA == '\\') {
	            output_buf[index_out++] = theA;
                    theA = get();
                }
                if (theA == 0) {
                    free(output_buf);
                    throw("!Unterminated string literal");
                }
            }
        }
    case 3:
        theB = next();
        if (theB == '/' && (theA == '(' || theA == ',' || theA == '=' ||
                            theA == ':' || theA == '[' || theA == '!' ||
                            theA == '&' || theA == '|' || theA == '?' ||
                            theA == '{' || theA == '}' || theA == ';' ||
                            theA == '\n')) {
	    output_buf[index_out++] = theA;
	    output_buf[index_out++] = theB;
            for (;;) {
                theA = get();
                if (theA == '/') {
                    break;
                }
                if (theA =='\\') {
	            output_buf[index_out++] = theA;
                    theA = get();
                }
                if (theA == 0) {
                    free(output_buf);
                    throw("!Unterminated Regular Expression literal");
                }
	        output_buf[index_out++] = theA;
            }
            theB = next();
        }
    }
}


/* minify -- Copy the input to the output, deleting the characters which are
        insignificant to JavaScript. Comments will be removed. Tabs will be
        replaced with spaces. Carriage returns will be replaced with linefeeds.
        Most spaces and linefeeds will be removed.
*/

char* Jsmin::minify(char *original)
{
    input_buf = original;
    index_in = 0;
    index_out = 0;

    if (output_buf != NULL) {
      free(output_buf);
      output_buf = NULL;
    }

    output_buf = (char *)malloc(sizeof(char) * strlen(original));

    theA = '\n';
    action(3);
    while (theA != 0) {
        switch (theA) {
        case ' ':
            if (isAlphanum(theB)) {
                action(1);
            } else {
                action(2);
            }
            break;
        case '\n':
            switch (theB) {
            case '{':
            case '[':
            case '(':
            case '+':
            case '-':
                action(1);
                break;
            case ' ':
                action(3);
                break;
            default:
                if (isAlphanum(theB)) {
                    action(1);
                } else {
                    action(2);
                }
            }
            break;
        default:
            switch (theB) {
            case ' ':
                if (isAlphanum(theA)) {
                    action(1);
                    break;
                }
                action(3);
                break;
            case '\n':
                switch (theA) {
                case '}':
                case ']':
                case ')':
                case '+':
                case '-':
                case '"':
                case '\'':
                    action(1);
                    break;
                default:
                    if (isAlphanum(theA)) {
                        action(1);
                    } else {
                        action(3);
                    }
                }
                break;
            default:
                action(1);
                break;
            }
        }
    }
    output_buf[index_out] = 0;
    return output_buf;
}

extern "C" {
  char* minify(char *in)
  {
    char *out;
    Jsmin *m = new Jsmin();
    try {
      out = m->minify(in);
    }
    catch (char const *e) {
      out = strdup(e);
    }
    delete(m);
    return out;
  }
}
