#include <stdio.h>
#include <ctype.h>

enum Statetype {CODE, COMMENT, STRING, ESCAPE_STRING, CHAR, ESCAPE_CHAR};

handleCommentBeginState(int c) {
    enum Statetype state;
    int c_next;
    c_next = getchar();
    if (c_next == '/') {
        putchar(c);
        state = handleCommentBeginState(c_next);
    } else if (c_next == '*') {
        putchar(' ');
        state = COMMENT;
    } else {
        putchar(c);
        putchar(c_next);
        state = CODE;
    }
    return state;
}

handleCommentEndState(int c) {
    enum Statetype state;
    int c_next;
    c_next = getchar();
    if (c_next == '/') {
        state = CODE;
    } else if (c_next == '*') {
        putchar(c);
        state = handleCommentEndState(c_next);
    } else {
        putchar(c);
        putchar(c_next);
        state = CODE;
    }
    return state;
}

handleCodeState(int c) {
    enum Statetype state;
    if (c == '/') {
        state = handleCommentBeginState(c);
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

handleCommentState(int c) {
    enum Statetype state;
    if (c == '*') {
        state = handleCommentEndState(c);
    } else {
        state = COMMENT;
    }
    return state;
}

handleStringState(int c) {
    enum Statetype state;
    if (c == '"') {
        putchar(c);
        state = CODE;
    } else if (c == '\\') {
        pputchar(c);
        state = ESCAPE_STRING;
    } else if (c == '/') {
        state = handleCommentBeginState(c);
    } else {
        putchar(c);
        state = STRING;
    }
    return state;
}

handleEscapeStringState(int c) {
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

handleCharState(int c) {
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

handleEscapeCharState(int c) {
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
    enum Statetype state = CODE;
    while((c = getchar()) != EOF) {
        switch (state) {
            case CODE:
                state = handleCodeState(c);
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
    }
    return 0;
}