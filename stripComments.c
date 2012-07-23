#include<stdio.h>
#include<stdlib.h>
#include<setjmp.h>

/* stripComments relies on nested functions and tail call
   optimization.  gcc supports both features. */

int stripComments (void) {
  jmp_buf buf;
  void start(void){
    int getchar_(void) {
      int c=getchar();
      switch (c) {
        // We are done:
      case EOF: longjmp(buf, 1);
      default: return c;}}

    /* The next few void functions return a void value, just to
     * emphasize the tail recursion. */
    void single_slash(void) {
      void line_comment(void) {
        switch (getchar_()) {
        case '\n': putchar('\n'); return start();
        default: return line_comment();}}

      void long_comment(void){
        void long_comment_asterix(void) {
          switch (getchar_()) {
          case '/': return start();
          default: return long_comment();}}

        switch (getchar_()) {
        case '*': return long_comment_asterix();
        default: return long_comment();}}

      int c = getchar_();
      switch (c) {
      case '/': return line_comment();
      case '*': return long_comment();
      default: putchar('/'); putchar(c); return start();}}

    /* Too bad C doesn't allow this:

       void quote(const int q) {
       int c = getchar_();
       putchar(c);
       switch (c){
       case q: case '\n': return start();
       case '\\': putchar(getchar_()); // fall-through
       default: return quote(q);}}
    */

    void quote(const int q) {
      int c = getchar_();
      putchar(c);
      if ((c==q) || (c=='\n')) return start();
      switch (c){
      case '\\': putchar(getchar_()); // fall-through
      default: return quote(q);}}

    int c = getchar_();
    switch (c){
    case '/': return single_slash();
    case '\'': case '"': putchar(c); return quote(c);
    default: putchar(c); return start();}}
  
  if (!setjmp(buf)) {
    start();
    // If start() returns for whatever reason, we failed:
    return 0;}
  else return 1;}

int main () {
  if (stripComments())
    exit (EXIT_SUCCESS);
  else exit(EXIT_FAILURE);}
