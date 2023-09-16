#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

enum Statetype {CODE, COMMENT, STRING, ESCAPE_STRING, CHAR, ESCAPE_CHAR};

int handleCommentBeginState(int c, int *current_line_number, int *last_comment) {
    enum Statetype state;
    int c_next;
    c_next = getchar();
    if (c_next == '/') {
        putchar(c);
        state = handleCommentBeginState(c_next, current_line_number, last_comment);
    } else if (c_next == '*') {
        putchar(' ');
        *last_comment = *current_line_number;
        state = COMMENT;
    } else {
        putchar(c);
        putchar(c_next);
        state = CODE;
    }
    return state;
}

int handleCommentEndState(int c) {
    enum Statetype state;
    int c_next;
    c_next = getchar();
    if (c_next == '/') {
        state = CODE;
    } else if (c_next == '*') {
        putchar(c);
        state = handleCommentEndState(c_next);
    } else {
        state = COMMENT;
    }
    return state;
}

int handleCommentState(int c, int *current_line_number) {
    enum Statetype state;
    if (c == '*') {
        state = handleCommentEndState(c);
    } else if (c == '\n') {
        putchar(c);
        *current_line_number++;
        state = COMMENT;
    } else {
        state = COMMENT;
    }
    return state;
}

int handleCodeState(int c, int *current_line_number, int *last_comment) {
    enum Statetype state;
    if (c == '/') {
        state = handleCommentBeginState(c, current_line_number, last_comment);
    } else if (c == '\'') {
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

int handleStringState(int c) {
    enum Statetype state;
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

int handleEscapeStringState(int c) {
    enum Statetype state;
    if (c == '\\') {
        putchar(c);
        state = ESCAPE_STRING;
    } else {
        putchar(c);
        state = STRING;
    }
    return state;
}

int handleCharState(int c) {
    enum Statetype state;
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

int handleEscapeCharState(int c) {
    enum Statetype state;
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
    int c;
    int current_line_number, last_comment;
    current_line_number = 1;
    enum Statetype state = CODE;
    while((c = getchar()) != EOF) {
        switch (state) {
            case CODE:
                state = handleCodeState(c, &current_line_number, &last_comment);
            break;

            case COMMENT:
                state = handleCommentState(c, &current_line_number);
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
        current_line_number++;
    }
    return 0;
}