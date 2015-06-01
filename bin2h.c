#include <stdio.h>
#include <stdlib.h>

#define LINE_MAX_DATA 16

#define ErrorCleanup(e,x) { e = (x); goto __cleanup; }

#define ERR_BAD_ARG 1
#define ERR_BAD_SRC 2
#define ERR_BAD_DEST 3

int main(int argc, char *argv[])
{
	int res = 0;
	FILE *src = NULL, *dest = NULL;
	unsigned char ch;
	int k;

	if (argc != 4)
		ErrorCleanup(res, ERR_BAD_ARG);

	src = fopen(argv[1], "rb");
	if (!src)
		ErrorCleanup(res, ERR_BAD_SRC);

	dest = fopen(argv[2], "w");
	if (!dest)
		ErrorCleanup(res, ERR_BAD_DEST);

	fprintf(dest, "unsigned char %s[] = {", argv[3]);
	k = 0;

	while (1)
	{
		ch = fgetc(src);

		if (feof(src))
			break;
		
		k = k % LINE_MAX_DATA;
		if (k++ == 0)
			fprintf(dest, "\n");

		fprintf(dest, "0x%02x, ", ch);
	}

	fprintf(dest, "\n};\n");

	printf("[%s] -> [%s] completed.\n", argv[1], argv[2]);

__cleanup:

	if (src)
		fclose(src);

	if (dest)
		fclose(dest);

	switch (res)
	{
	case ERR_BAD_ARG:
		printf("Usage: bin2h <src file> <dest file> <token name>\n");
		break;

	case ERR_BAD_SRC:
		printf("Can't open src file: %s.\n", argv[1]);
		break;

	case ERR_BAD_DEST:
		printf("Can't open dest file: %s.\n", argv[2]);
		break;

	default:
		break;
	}

	return 0;
}
