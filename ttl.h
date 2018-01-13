#ifndef TTL_H
#define TTL_H

#include <stddef.h>

enum ttl_token_type {
	TTL_TOKEN_LIST_START = 1,
	TTL_TOKEN_LIST_END,
	TTL_TOKEN_SYMBOL
};

struct ttl_token {
	enum		 ttl_token_type type;
	size_t		 len;
	const char	*src;
	int		 err;
};

int ttl_tokenize(struct ttl_token *, size_t, const char *);

#endif
