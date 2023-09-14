#include <stdio.h>
#include <ctype.h>

enum Statetype {CODE, COMMENT_BEGIN, COMMENT, COMMENT_END, STRING, ESCAPE_STRING, CHAR, ESCAPE_CHAR};

handleCodeState(int c) {
    enum Statetype state;
    if (c == '/') {
        state = COMMENT_BEGIN;
    } else if (c == '\'') {
        state = CHAR;
    } else if (c == '"') {
        state = STRING;
    } else {
        state = CODE;
    }
    return state;
}

handleCommentBeginState(int c) {
    enum Statetype state;
    if (c == '/') {
        state = COMMENT_BEGIN;
    } else if (c == '*') {
        state = COMMENT;
    } else {
        state = CODE;
    }
    return state;
}

handleCommentState(int c) {
    enum Statetype state;
    if (c == '*') {
        state = COMMENT_END;
    } else {
        state = COMMENT;
    }
    return state;
}

handleCommentEndState(int c) {
    enum Statetype state;
    if (c == '*') {
        state = COMMENT_END;
    } else if (c == '/') {
        state = CODE;
    } else {
        state = COMMENT;
    }
    return state;
}

handleStringState(int c) {
    enum Statetype state;
    if (c == '"') {
        state = CODE;
    } else if (c == '\\') {
        state = ESCAPE_STRING;
    } else {
        state = STRING;
    }
    return state;
}

handleEscapeStringState(int c) {
    enum Statetype state;
    if (c == '\\') {
        state = ESCAPE_STRING;
    } else {
        state = STRING;
    }
    return state;
}

handleCharState(int c) {
    enum Statetype state;
    if (c == '\\') {
        state = ESCAPE_CHAR;
    } else if (c == '\'') {
        state = CODE;
    } else {
        state = CHAR;
    }
    return state;
}   

handleEscapeCharState(int c) {
    enum Statetype state;
    if (c == '\\') {
        state = ESCAPE_CHAR;
    } else {
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

            case COMMENT_BEGIN:
                state = handleCommentBeginState(c);
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
    }
    return 0;
}