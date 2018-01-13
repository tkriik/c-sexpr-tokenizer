#include <err.h>
#include <stdio.h>

#include "token.h"

void
usage(void)
{
	extern char *__progname;
	errx(1, "usage: %s <expression>", __progname);
}

void
print_indent(int indent)
{
	for (int i = 0; i < indent; i++) {
		putchar(' ');
		putchar(' ');
	}
}

void
dump_tokens(const char *src)
{
	int level = 0;
	struct token token;
	while (1) {
		src = token_get(&token, src);

		switch (token.type) {
		case TOKEN_ERR:
			print_indent(level);
			printf("INVALID TOKEN: %.*s\n", token.len, token.src);
			break;
		case TOKEN_LIST_START:
			print_indent(level);
			level++;
			printf("LIST\n");
			break;
		case TOKEN_LIST_END:
			printf("END\n");
			level--;
			print_indent(level);
			break;
		case TOKEN_SYMBOL:
			print_indent(level);
			printf("%sSYMBOL: %.*s\n", token.err ? "INVALID" : "", token.len, token.src);
			break;
		default:
			print_indent(level);
			printf("UNKNOWN TOKEN: (type: %d, src: %p, len: %d, err: %d)\n",
			    token.type, token.src, token.len, token.err);
			break;
		}

		if (src == NULL)
			break;
	}
}

int
main(int argc, char **argv)
{
	if (argc != 2)
		usage();

	const char *src = argv[1];
	dump_tokens(src);

	return 0;
}
