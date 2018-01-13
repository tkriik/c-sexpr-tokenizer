#include <err.h>
#include <stdio.h>

#include "ttl.h"

#define TOKEN_CNT	4096
#define INPUT_SIZE	512

void
usage(void)
{
	extern char *__progname;
	errx(1, "usage: %s <expression>", __progname);
}

void
print_indent(size_t indent)
{
	for (size_t i = 0; i < indent; i++) {
		putchar(' ');
		putchar(' ');
	}
}

void
dump_tokens(struct ttl_token *token_buf, size_t token_cnt)
{
	size_t level = 0;
	for (size_t i = 0; i < token_cnt; i++) {
		struct ttl_token *token = &token_buf[i];
		switch (token->type) {
		case TTL_TOKEN_LIST_START:
			print_indent(level);
			printf("LIST_START\n");
			level++;
			break;
		case TTL_TOKEN_LIST_END:
			level--;
			print_indent(level);
			printf("LIST_END\n");
			break;
		case TTL_TOKEN_SYMBOL:
			print_indent(level);
			if (token->err)
				printf("INVALID SYMBOL ");
			else
				printf("SYMBOL ");
			printf("%.*s\n", (int)token->len, token->src);
			break;
		default:
			print_indent(level);
			printf("UNKNOWN TOKEN (type: %d, len: %zu, src: %p, err: %d)\n",
			    token->type, token->len, token->src, token->err);
			break;
		}
	}
}

int
main(int argc, char **argv)
{
	if (argc != 2)
		usage();

	struct ttl_token token_buf[TOKEN_CNT];
	const char *src = argv[1];

	size_t ntokens = ttl_tokenize(token_buf, TOKEN_CNT, src);
	if (ntokens == TOKEN_CNT) {
		warn("possibly truncated tokenization");
	}

	dump_tokens(token_buf, ntokens);

	return 0;
}
