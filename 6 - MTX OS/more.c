#include "ucode.c"

#define ROWS 20
#define COLUMNS  80

void print(int fd, int lines)
{
	int i, j, size;
	char c;

	for (i = 0; i < lines; i++) {
		for (j = 0; j < COLUMNS; j++) {
			size = read(fd, &c, 1);
			printc(c);

			if (size < 1)
			{
				exit(0);
			}

			if (c == '\n' || c == '\r')
				break;
		}
	}

	printc('\n');
}
int main(int argc, char *argv[])
{
	char c, tty[64];
	int fd;

	if (argc == 1) {
		fd = dup(0);
		close(0);
		gettty(tty);
		open(tty, O_RDONLY);
	}
	else {
		fd = open(argv[1], O_RDONLY);
	}
	if (fd < 0) {
		printf("File not found!\n");
		return -1;
	}

	while (1) {
		print(fd, ROWS);

		while (1) {
			c = getc();

			switch (c)
			{
				case '\r':
				case '\n':
					print(fd, 1);
					break;
				case ' ':
					print(fd, ROWS);
					break;
				case 'q':
					printc('\n');
					return 0;
					break;
			}
		}
	}

	return 0;
}

