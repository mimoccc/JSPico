//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------

#include <stdint.h>

//-----------------------------------------------------------------------------

#define JS_VERSION "2.2.0"

//-----------------------------------------------------------------------------

// array size helper
#define array_size(x) (sizeof(x) / sizeof((x)[0]))

//-----------------------------------------------------------------------------

enum {
    TOK_ERR, TOK_EOF, TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING,
    TOK_SEMICOLON, TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE,
    // Keyword tokens
    TOK_BREAK = 50, TOK_CASE, TOK_CATCH, TOK_CLASS, TOK_CONST, TOK_CONTINUE,
    TOK_DEFAULT, TOK_DELETE, TOK_DO, TOK_ELSE, TOK_FINALLY, TOK_FOR,
    TOK_FUNC, TOK_IF, TOK_IN, TOK_INSTANCEOF, TOK_LET, TOK_NEW, TOK_RETURN,
    TOK_SWITCH, TOK_THIS, TOK_THROW, TOK_TRY, TOK_VAR, TOK_VOID, TOK_WHILE,
    TOK_WITH, TOK_YIELD, TOK_UNDEF, TOK_NULL, TOK_TRUE, TOK_FALSE,
    // JS Operator tokens
    TOK_DOT = 100, TOK_CALL, TOK_POSTINC, TOK_POSTDEC, TOK_NOT, TOK_NEG,
    TOK_TYPEOF, TOK_UPLUS, TOK_UMINUS, TOK_EXP, TOK_MUL, TOK_DIV, TOK_REM,
    TOK_PLUS, TOK_MINUS, TOK_SHL, TOK_SHR, TOK_ZSHR, TOK_LT, TOK_LE, TOK_GT,
    TOK_GE, TOK_EQ, TOK_NE, TOK_AND, TOK_XOR, TOK_OR, TOK_LAND, TOK_LOR,
    TOK_COLON, TOK_Q, TOK_ASSIGN, TOK_PLUS_ASSIGN, TOK_MINUS_ASSIGN,
    TOK_MUL_ASSIGN, TOK_DIV_ASSIGN, TOK_REM_ASSIGN, TOK_SHL_ASSIGN,
    TOK_SHR_ASSIGN, TOK_ZSHR_ASSIGN, TOK_AND_ASSIGN, TOK_XOR_ASSIGN,
    TOK_OR_ASSIGN, TOK_COMMA,
};

//-----------------------------------------------------------------------------

enum {
    // IMPORTANT: T_OBJ, T_PROP, T_STR must go first.  That is required by the
    // memory layout functions: memory entity types are encoded in the 2 bits,
    // thus type values must be 0,1,2,3
    T_OBJ, T_PROP, T_STR, T_UNDEF, T_NULL, T_NUM, T_BOOL, T_FUNC, T_CODEREF, T_ERR
};

//-----------------------------------------------------------------------------

// JS engine (opaque)
struct js;

//-----------------------------------------------------------------------------

// JS value placeholder
typedef uint64_t jsval_t;

//-----------------------------------------------------------------------------

// Create JS instance
struct js *js_create(void *buf, size_t len);

//-----------------------------------------------------------------------------

// Stringify JS value
const char *js_str(struct js *, jsval_t val);

//-----------------------------------------------------------------------------

// Execute JS code from string
jsval_t js_eval(struct js *js, const char *string);

//-----------------------------------------------------------------------------

// Execute JS code from memory
jsval_t js_eval_buff(struct js *js, const char *buf, size_t len);

//-----------------------------------------------------------------------------

// Return global object
jsval_t js_glob(struct js *);

//-----------------------------------------------------------------------------

// Create a new object
jsval_t js_mkobj(struct js *);

//-----------------------------------------------------------------------------

// Create a string
jsval_t js_mkstr(struct js *, const void *, size_t);

//-----------------------------------------------------------------------------

// Create a number
jsval_t js_mknum(struct js *, double);

//-----------------------------------------------------------------------------

// Import native func
jsval_t js_import(struct js *, uintptr_t, const char *);

//-----------------------------------------------------------------------------

// Set obj attribute
void js_set(struct js *, jsval_t, const char *, jsval_t);

//-----------------------------------------------------------------------------

// Return mem usage
int js_usage(struct js *);

//-----------------------------------------------------------------------------

// call gc
void js_gc(struct js *js);

//-----------------------------------------------------------------------------
