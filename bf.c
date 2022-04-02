#include <stdio.h>
#include <stdlib.h>

#define ARRSZ	30000 /* lol */
#define STKSZ	512

void clean(FILE *fp, unsigned char *ptr, fpos_t *stk)
{
	if (fp)
		fclose(fp);
	if (ptr)
		free(ptr);
	if (stk)
		free(stk);
}

void die(const char *msg, FILE *fp, unsigned char *ptr, fpos_t *stk)
{
	printf("%s\n", msg);
	clean(fp, ptr, stk);
	exit(1);
}

int main(int argc, char **argv)
{
	FILE 		*fp;
	unsigned char	*ptr, *p;
	unsigned int	c, t, l;
	fpos_t		*stk;

	t = l = 0; /* stack location and number of loops passed without exec */
	fp = NULL;
	stk = NULL;
	ptr = NULL;

	if (argc != 2)
		die("not enough, or too many arguments", fp, ptr, stk);

	ptr = calloc(ARRSZ, sizeof(*ptr));
	if (!ptr)
		die("failed to allocate memory cells", fp, ptr, stk);
	p = ptr;

	stk = calloc(STKSZ, sizeof(*stk));
	if (!stk)
		die("failed to allocate memory for stack", fp, ptr, stk);

	fp = fopen(argv[1], "r");
	if (!fp)
		die("failed to open file", fp, ptr, stk);

	while ((c = fgetc(fp)) != EOF) {
		switch (c) {
			case '>':
				p++;
				break;
			case '<':
				p--;
				break;
			case '+':
				(*p)++;
				break;
			case '-':
				(*p)--;
				break;
			case '.':
				putchar(*p);
				break;
			case ',':
				*p = getchar();
				break;
			case '[':
				if (*p) {
					if (t > STKSZ)
						die("stack overflow", fp, ptr, stk);
					fgetpos(fp, stk + t++);
				} else {
					while (++l && (c = fgetc(fp)) != EOF) {
						if (c == '[')
							l++;
						else if (c == ']')
							l--;
					}
				}
				break;
			case ']':
				if (*p)
					fsetpos(fp, stk + t - 1);
				else
					t--;
				break;
		}
	}

	clean(fp, ptr, stk);
	return 0;
}
