
#include "ucode.c"

#define NUMLINES 128

int clearstr(char *str)
{
	int i;
	for (i = 0; i < NUMLINES; i++)
		str[i] = 0;

	return 0;
}

int checkline(char *str, char *pattern, int patternsize)
{
	int i;

	while(*(str + patternsize - 1)) {
		if(*str == *pattern) {
			for(i = 0; i < patternsize; i++) {
				if(str[i] == pattern[i]) {
					continue;
				}
				else {
					break;
				}
			}
			if(i == patternsize)
				return 1;
		}
		str++;
	}
	return 0;
}


int main(int argc, char *argv[])
{
	int fd, patternsize, i, reader, linenum = 0;
	char line[1024];
	int searching = 1;

	if (argc == 1) 
		exit(1);
	else if (argc == 2)
		fd = 0;
	else
		fd = open(argv[2], O_RDONLY);

	if (fd >= 0) {
		patternsize = strlen(argv[1]);
		while (searching)
		{
			clearstr(line);

			for (i = 0; i < NUMLINES - 1; i++)
			{
				reader = read(fd, &line[i], 1);

				if (reader < 1)
				{
					searching = 0;
					break;
				}
				if (line[i] == '\n')
				{
					line[i] = 0;
					break;
				}
				if (line[i] == '\r')
				{
					line[i + 1] = 0;
					break;
				}
				if (line[i] == 0)
				{
					searching = 0; 
					break;
				}
			}

			linenum++;
			if (searching && checkline(line, argv[1], patternsize))
				printf("Found match at line %d (%s)\n", linenum / 2 + 1, line);
		}
	}
	else
		exit(-1);


	close(fd);
	exit(1);
}


