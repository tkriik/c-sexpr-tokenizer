#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	TOKEN_ERR = 0, /* error marker */
	TOKEN_LIST_START,
	TOKEN_LIST_END,
	TOKEN_SYMBOL,
	TOKEN_STRING
};

struct token {
	enum		 token_type type;
	const char	*src;
	int		 len;
	int		 err;
};

const char *token_get(struct token *, const char *);

#endif
