#ifndef JSMIN_H
#define JSMIN_H

class Jsmin
{
public:
  Jsmin();
  char* minify(char *);

private:
  int   theA;
  int   theB;
  int   theLookahead;
  int index_in;
  int index_out;
  char *input_buf;
  char *output_buf;
  int m_size;

  int isAlphanum(int c);
  int get();
  int peek();
  int next();
  void action(int d);
};
#endif
