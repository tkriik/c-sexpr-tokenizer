#include "token.h"

#define NULL ((void *)0)

static int
is_blank_char(char c)
{
	switch (c) {
	case ' ':
	case '\n':
	case '\t':
	case '\v':
	case '\r':
		return 1;
	default:
		return 0;
	}
}

static int
is_list_start_char(char c)
{
	return c == '(';
}

static int
is_list_end_char(char c)
{
	return c == ')';
}

static int
is_symbol_start_char(char c)
{
	return ('A' <= c && c <= 'Z')
	    || ('a' <= c && c <= 'z');
}

static int
is_symbol_char(char c)
{
	return is_symbol_start_char(c)
	    || ('0' <= c && c <= '9');
}

static int
is_symbol_end_char(char c)
{
	return is_blank_char(c)
	    || is_list_start_char(c)
	    || is_list_end_char(c);
}

static int
is_string_enclosing_char(char c)
{
	return c == '"';
}

static int
is_string_escape_char(char c)
{
	return c == '\\';
}

static int
is_err_end_char(char c)
{
	return is_blank_char(c)
	    || is_list_start_char(c)
	    || is_list_end_char(c);
}

const char *
token_get(struct token *token, const char *src)
{
	enum {
		SEEK,
		LIST_START,
		LIST_END,
		SYMBOL_START,
		SYMBOL,
		STRING_START,
		STRING,
		STRING_ESCAPE,
		ERR_START,
		ERR
	} st = SEEK;

	token->type = TOKEN_ERR;
	token->src = src;
	token->len = 0;
	token->err = 0;

	char c;
	while ((c = *src) != '\0') {
		switch (st) {
		case SEEK:
			if (is_blank_char(c)) {
				src++;
				break;
			}
			if (is_list_start_char(c)) {
				st = LIST_START;
				break;
			}
			if (is_list_end_char(c)) {
				st = LIST_END;
				break;
			}
			if (is_symbol_start_char(c)) {
				st = SYMBOL_START;
				break;
			}
			if (is_string_enclosing_char(c)) {
				st = STRING_START;
				break;
			}
			st = ERR_START;
			break;
		case LIST_START:
			token->type = TOKEN_LIST_START;
			token->src = src;
			token->len = 1;
			token->err = 0;
			return src + 1;
		case LIST_END:
			token->type = TOKEN_LIST_END;
			token->src = src;
			token->len = 1;
			token->err = 0;
			return src + 1;
		case SYMBOL_START:
			token->type = TOKEN_SYMBOL;
			token->src = src;
			token->len = 1;
			token->err = 0;
			st = SYMBOL;
			src++;
			break;
		case SYMBOL:
			if (is_symbol_end_char(c))
				return src;
			if (!is_symbol_char(c))
				token->err = 1;
			token->len++;
			src++;
			break;
		case STRING_START:
			token->type = TOKEN_STRING;
			token->src = src;
			token->len = 1;
			token->err = 0;
			st = STRING;
			src++;
			break;
		case STRING:
			if (is_string_enclosing_char(c)) {
				token->len++;
				return src + 1;
			}
			if (is_string_escape_char(c)) {
				st = STRING_ESCAPE;
				token->len++;
				src++;
				break;
			}
			token->len++;
			src++;
			break;
		case STRING_ESCAPE:
			st = STRING;
			token->len++;
			src++;
			break;
		case ERR_START:
			token->type = TOKEN_ERR;
			token->src = src;
			token->len = 1;
			token->err = 0;
			st = ERR;
			src++;
			break;
		case ERR:
			if (is_err_end_char(c))
				return src;
			token->len++;
			src++;
			break;
		}
	}

	return NULL;
}
