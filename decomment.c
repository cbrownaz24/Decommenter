#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* Enumerated states that the code text could potentially be in at any 
given moment */
enum Statetype {
  CODE, 
  COMMENT_START,
  COMMENT, 
  COMMENT_END,
  STRING, 
  ESCAPE_STRING, 
  CHAR, 
  ESCAPE_CHAR
  };

/* ------------------------------------------------------------------ */
/*
  Assess the next state transition from the present CODE state using the
  current char value, c.
*/
int handleCodeState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */

  if (c == '/') {
    state = COMMENT_START;
  } 
  else if (c == '\'') {
    putchar(c);
    state = CHAR;
  } 
  else if (c == '"') {
    putchar(c);
    state = STRING;
  } 
  else {
    putchar(c);
    state = CODE;
  }
  return state;
}

/* ------------------------------------------------------------------ */
/*
  Assess the next state transition from the present COMMENT_START state 
  using the current char value, c. If transition to COMMENT, update 
  last_comment to equal current_line.
*/
int handleCommentStartState(
  int c, 
  int *current_line, 
  int *last_comment
  ) {
  enum Statetype state; /* Keep track of which state to transition to */

  /* '/' indicates that a new comment might still be forming. So, the 
  last '/' which caused COMMENT_START is not actually a part of comment.
  Since comment might still be forming, transition to COMMENT_START 
  again.*/
  if (c == '/') { 
    putchar('/');
    state = COMMENT_START;
  } 
  /* '*' indicates that c was in fact the start of a comment. Transition
  to COMMENT state and put a ' ' in place of the new comment. Update the
  last line occurance of a new comment, last_comment, according to the 
  current_line value. */
  else if (c == '*') {
    putchar(' ');
    *last_comment = *current_line;
    state = COMMENT;
  }
  /* Any other character indicates that there is neither a comment nor 
  the potential start of one. Transition back to appropriate non-COMMENT
   state. */
  else if (c == '\"') {
    putchar('/');
    putchar(c);
    state = STRING;
  }
  else if (c == '\'') {
    putchar('/');
    putchar(c);
    state = CHAR;
  }
  else {
    putchar('/');
    putchar(c);
    state = CODE;
  }
  return state;
}

/* ------------------------------------------------------------------ */
/*
  Assess the state transition from COMMENT state using the current char 
  value, c.
*/
int handleCommentState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */

  if (c == '*') {
    state = COMMENT_END;
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

/* ------------------------------------------------------------------ */
/*
  Assess state transition from COMMENT_END state using the current char 
  value, c.
*/
int handleCommentEndState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */

  /* '/' indicates that the comment has in fact ended. Transition back
  to CODE state. */
  if (c == '/') {
    state = CODE;
  } 
  /* '*' inidicates that an end comment pattern might still be forming.
  Repeat COMMENT_END state. */
  else if (c == '*') {
    state = COMMENT_END;
  } 
  else if (c == '\n') {
    putchar(c);
  }
  else {
    state = COMMENT;
  }
  return state;
}

/* ------------------------------------------------------------------ */
/*
  Assess state transition from STRING state using the current char 
  value, c.
*/
int handleStringState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '"') {
    putchar(c);
    state = CODE;
  } 
  else if (c == '\\') {
    putchar(c);
    state = ESCAPE_STRING;
  } 
  else {
    putchar(c);
    state = STRING;
  }
  return state;
}

/* ------------------------------------------------------------------ */
/*
  Assess state transition from ESCAPE_STRING state using current char 
  value, c.
*/
int handleEscapeStringState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '\\') {
    putchar(c);
    state = ESCAPE_STRING;
  } 
  else {
    putchar(c);
    state = STRING;
  }
  return state;
}

/* ------------------------------------------------------------------ */
/*
  Assess state transition from CHAR state using current char value, c.
*/
int handleCharState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '\\') {
    putchar(c);
    state = ESCAPE_CHAR;
  } 
  else if (c == '\'') {
    putchar(c);
    state = CODE;
  } 
  else {
    putchar(c);
    state = CHAR;
  }
  return state;
}   

/* ------------------------------------------------------------------ */
/*
  Assess state transition from ESCAPE_CHAR state using current char
  value, c.s
*/
int handleEscapeCharState(int c) {
  enum Statetype state; /* Keep track of which state to transition to */
  if (c == '\\') {
    putchar(c);
    state = ESCAPE_CHAR;
  } 
  else {
    putchar(c);
    state = CHAR;
  }
  return state;
}

/* ------------------------------------------------------------------ */
/* Remove all comments in a given C-language styled input. */
int main(void) {
  /* c keeps track of the current character in file. */
  /* current_line tracks the current line in file. */
  /* last_comment tracks the last line of an opening comment. */
  int c, current_line, last_comment;

  enum Statetype state = CODE;
  current_line = 1;

  /* Iterate through all characters in file and apply state transitions 
  accordingly. */
  while((c = getchar()) != EOF) {
    switch (state) {
      case CODE:
        state = handleCodeState(c);
      break;

      case COMMENT_START:
        /* Pass the current_line and last_comment pointers in the case
        that a new comment has started and last_comment must be updated
        using current_line. */
        state = handleCommentStartState
        (
          c, 
          &current_line, 
          &last_comment
        );
      break;

      case COMMENT:
        state = handleCommentState(c);
      break;

      case COMMENT_END:
        state = handleCommentEndState(c);
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
    fprintf(
      stderr, 
      "Error: line %d: unterminated comment\n", 
      last_comment
      );
    exit(EXIT_FAILURE);
  } 
  else {
    exit(EXIT_SUCCESS);
  }
}