#include <assert.h>
#include <ctype.h>
#include <string.h>

#include "ttl.h"

static int
is_whitespace(char c)
{
	return isspace(c);
}

static int
is_symbol_start(char c)
{
	return isalpha(c) || strchr("-><+/", c) != NULL;
}

static int
is_symbol(char c)
{
	return isdigit(c) || is_symbol_start(c);
}

static void
set_list_start(struct ttl_token *token, const char *src)
{
	token->type = TTL_TOKEN_LIST_START;
	token->len = 1;
	token->src = src;
	token->err = 0;
}

static void
set_list_end(struct ttl_token *token, const char *src)
{
	token->type = TTL_TOKEN_LIST_END;
	token->len = 1;
	token->src = src;
	token->err = 0;
}

static void
set_symbol(struct ttl_token *token, const char *src)
{
	token->type = TTL_TOKEN_SYMBOL;
	token->len = 1;
	token->src = src;
	token->err = 0;
}

static void
inc_len(struct ttl_token *token)
{
	token->len++;
}

static void
set_err(struct ttl_token *token)
{
	token->err = 1;
}

int
ttl_tokenize(struct ttl_token *token_buf, size_t token_cnt, const char *src)
{
	enum {
		SEEK,
		SOME_TOKEN,
		LIST_START,
		LIST_END,
		SYMBOL_START,
		SYMBOL
	} st = SEEK;

	size_t ntokens = 0;
	size_t i = 0;
	while (*src != '\0' && i < token_cnt) {
		char c = *src;

		switch (st) {
		case SEEK:
			if (is_whitespace(c)) {
				src++;
				continue;
			}
			st = SOME_TOKEN;
			continue;
		case SOME_TOKEN:
			switch (c) {
			case '(':
				st = LIST_START;
				continue;
			case ')':
				st = LIST_END;
				continue;
			}
			st = SYMBOL_START;
			continue;
		case LIST_START:
			set_list_start(&token_buf[i], src);
			st = SEEK;
			ntokens++;
			src++;
			i++;
			continue;
		case LIST_END:
			set_list_end(&token_buf[i], src);
			st = SEEK;
			ntokens++;
			src++;
			i++;
			continue;
		case SYMBOL_START:
			set_symbol(&token_buf[i], src);
			if (!is_symbol_start(c))
				set_err(&token_buf[i]);
			st = SYMBOL;
			ntokens++;
			src++;
			continue;
		case SYMBOL:
			if (is_whitespace(c)) {
				st = SEEK;
				src++;
				i++;
				continue;
			}
			switch (c) {
				case '(':
					st = LIST_START;
					i++;
					continue;
				case ')':
					st = LIST_END;
					i++;
					continue;
			}
			if (!is_symbol(c))
				set_err(&token_buf[i]);
			inc_len(&token_buf[i]);
			src++;
			continue;
		}
	}

	return ntokens;
}
