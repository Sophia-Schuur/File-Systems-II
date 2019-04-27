#include "ucode.c"

int main(int argc, char *argv[]) {
	int n;
	char buf[1024];
	int src = open(argv[1], 0);
	int dest = open(argv[2], 1);

	printf("%d\n", src);
	printf("%d\n", dest);
	if(src < 0) {
		return -1;
	}

	if(dest < 0) {
		creat(argv[2]);
		dest = open(argv[2], 1);
	}

	while( (n = read(src, buf, 1024)) ){
		write(dest, buf, n);
	}

	close(src);
	close(dest);
}
