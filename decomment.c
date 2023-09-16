#include <stdio.h>
#include <ctype.h>

/* Enumerated states that the code text could potentially be in at any 
given moment */
enum Statetype {
  CODE, 
  COMMENT, 
  STRING, 
  ESCAPE_STRING, 
  CHAR, 
  ESCAPE_CHAR
  };

/*
  Transition to the next logical state from a begin comment pattern, 
  according to the current character c. If a transition to the COMMENT 
  state, equate the comment's start line, last_comment, as the current 
  line in the file, current_line. 
*/
int handleCommentBeginState(c, current_line, last_comment) 
  int c;
  int *current_line;
  int *last_comment;
{
  enum Statetype state; /* Keep track of which state to transition to */
  int c_next; /* Store next character to see two-character pattern. */
  c_next = getchar();

  /* '/' indicates that a new comment might still be forming. So, ignore
  the current character as part of a comment and restart the comment 
  begin pattern analysis with the c_next char.*/
  if (c_next == '/') { 
    putchar(c);
    state = handleCommentBeginState(c_next, current_line, last_comment);
  } 
  /* '*' indicates that c was in fact the start of a comment. Transition
  to COMMENT state and update the last new comment line, last_comment,
  according to the current line in the file, current_line. */
  else if (c_next == '*') {
    putchar(' ');
    *last_comment = *current_line;
    state = COMMENT;
  }
  /* Any other character indicates that there is neither a comment nor 
  the potential start of one. Transition back to CODE state. */
  else {
    putchar(c);
    putchar(c_next);
    state = CODE;
  }
  return state;
}

/*
  Transition to the next logical state from an ending comment pattern,
  according to the current character c.
*/
int handleCommentEndState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  int c_next; /* Store next character to see two-character pattern. */
  c_next = getchar();

  /* '/' indicates that the comment has in fact ended. Transition back
  to CODE state. */
  if (c_next == '/') {
    state = CODE;
  } 
  /* '*' inidicates that an end comment pattern might still be forming.
  Check again if this is an end comment. Ignore the previous indication 
  (c character). */
  else if (c_next == '*') {
    putchar(c);
    state = handleCommentEndState(c_next);
  } 
  /* Any other character indicates that the comment is neither finished
  nor potentially finished yet. Transition back to COMMENT state. */
  else {
    state = COMMENT;
  }
  return state;
}

/*
  Transition to the next logical state from the previous COMMENT state, 
  according to the current character c. 
*/
int handleCommentState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */

  /* '*' indicates that the comment might be ending. Call 
  handleCommentEndState() to see if this is actually the case. */
  if (c == '*') {
    state = handleCommentEndState(c);
  } 
  else if (c == '\n') {
    putchar(c);
    state = COMMENT;
  } 
  else {
    state = COMMENT;
  }
  return state;
}

/*
  Transition to the next logical state from the previous CODE state, 
  according to the current character c. If c is a newline character,
  increment the current line of the file, current_line. If a potential
  new comment is begun, pass the last line occurence of a new comment, 
  last_comment, to handleCommentBeginState() such that it may be updated
  accordingly.
*/
int handleCodeState(int c, int *current_line, int *last_comment) {
  enum Statetype state; /* Keep track of which state to transition to */

  /* '/' indicates that the comment might be starting. Call 
  handleCommentBeginState() to see if this is actually the case. 
  Pass the current line in the file, current_line, and the last comment
  occurance line, last_comment, to update accordingly if a new comment
  is in fact formed. */
  if (c == '/') {
    state = handleCommentBeginState(c, current_line, last_comment);
  } 
  else if (c == '\'') {
    putchar(c);
    state = CHAR;
  } else if (c == '"') {
    putchar(c);
    state = STRING;
  } else {
    putchar(c);
    state = CODE;
  }
  return state;
}

/*
  Transition to the next logical state from the previous STRING state, 
  according to the current character c. 
*/
int handleStringState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '"') {
    putchar(c);
    state = CODE;
  } else if (c == '\\') {
    putchar(c);
    state = ESCAPE_STRING;
  } else {
    putchar(c);
    state = STRING;
  }
  return state;
}

/*
  Transition to the next logical state from the previous ESCAPE_STRING 
  state, according to the current character c. 
*/
int handleEscapeStringState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '\\') {
    putchar(c);
    state = ESCAPE_STRING;
  } else {
    putchar(c);
    state = STRING;
  }
  return state;
}

/*
  Transition to the next logical state from the previous CHAR state, 
  according to the current character c. 
*/
int handleCharState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '\\') {
    putchar(c);
    state = ESCAPE_CHAR;
  } else if (c == '\'') {
    putchar(c);
    state = CODE;
  } else {
    putchar(c);
    state = CHAR;
  }
  return state;
}   

/*
  Transition to the next logical state from the previous ESCAPE_CHAR 
  state, according to the current character c. 
*/
int handleEscapeCharState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '\\') {
    putchar(c);
    state = ESCAPE_CHAR;
  } else {
    putchar(c);
    state = CHAR;
  }
  return state;
}

int main(void) {
  /* c keeps track of the current character in file. */
  /* current_line tracks the current line in file. */
  /* last_comment tracks the last line of an opening comment. */
  int c, current_line, last_comment;

  /* More readable tracking of exit status by the end of file. */
  enum Exittype {EXIT_FAILURE, EXIT_SUCCESS};
  enum Exittype exit;

  enum Statetype state = CODE;
  current_line = 1;

  /* Iterate through all characters in file and apply state transitions 
  accordingly. */
  while((c = getchar()) != EOF) {
    switch (state) {
      case CODE:
        state = handleCodeState(c, &current_line, &last_comment);
      break;

      case COMMENT:
        state = handleCommentState(c);
      break;

      case STRING:
        state = handleStringState(c);
      break;

      case ESCAPE_STRING:
        state = handleEscapeStringState(c);
      break;

      case CHAR:
        state = handleCharState(c);
      break;

      case ESCAPE_CHAR:
        state = handleEscapeCharState(c);
      break;
    } 
        
    if (c == '\n') {
      current_line++;
    }
  }

  /* If still in a COMMENT by end of file, then the comment was 
  unterminated. */
  if (state == COMMENT) {
    printf("Error: line %d: unterminated comment\n", last_comment);
    exit = EXIT_FAILURE;
  } else {
    exit = EXIT_SUCCESS;
  }
  return exit;
}