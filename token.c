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
	return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}

static int
is_symbol_char(char c)
{
	return is_symbol_start_char(c) || ('0' <= c && c <= '9');
}

const char *
token_get(struct token *token, const char *src)
{
	enum {
		SEEK,
		SYMBOL,
		ERR
	} st = SEEK;

	token->type = TOKEN_ERR;
	token->src = src;
	token->len = 0;
	token->err = 1;

	char c;
	while ((c = *src) != '\0') {
		switch (st) {
		case SEEK:
			if (is_blank_char(c)) {
				src++;
				continue;
			}
			if (is_list_start_char(c)) {
				token->type = TOKEN_LIST_START;
				token->src = src;
				token->len = 1;
				token->err = 0;
				return src + 1;
			}
			if (is_list_end_char(c)) {
				token->type = TOKEN_LIST_END;
				token->src = src;
				token->len = 1;
				token->err = 0;
				return src + 1;
			}
			if (is_symbol_start_char(c)) {
				token->type = TOKEN_SYMBOL;
				token->src = src;
				token->len = 1;
				token->err = 0;
				st = SYMBOL;
				src++;
				continue;
			}
			st = ERR;
			continue;
		case SYMBOL:
			if (is_blank_char(c))
				return src;
			if (is_symbol_char(c)) {
				token->len++;
				src++;
				continue;
			}
			token->err = 1;
			continue;
		case ERR:
			if (is_blank_char(c))
				return src;
			token->len++;
			continue;
		}
	}

	return NULL;
}
