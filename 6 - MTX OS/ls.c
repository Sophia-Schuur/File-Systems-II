#include "ucode.c"

#define	S_ISDIR(m)	((m & 0170000) == 0040000)	
#define	S_ISCHR(m)	((m & 0170000) == 0020000)
#define	S_ISBLK(m)	((m & 0170000) == 0060000)
#define	S_ISREG(m)	((m & 0170000) == 0100000)

int ls_file(char *path) {

	static const char *permissions = "rwxrwxrwx";
	STAT st;

	printf("%s ", path);
	stat(path, &st);

	switch(st.st_mode & 0xF000)
	{
		case 0x8000:  putchar('-');     break; // 0x8 = 1000
		case 0x4000:  putchar('d');     break; // 0x4 = 0100
		case 0xA000:  putchar('l');     break; // oxA = 1010
		default:      putchar('?');     break;
	}

	for(int i = 0; i < strlen(permissions); i++) {
		putchar(st.st_mode & (1 << (strlen(permissions) - 1 - i)) ? permissions[i] : '-');
	}


	printf("%d %d %d %d\n", st.st_nlink, st.st_gid, st.st_uid, st.st_size);

}

int ls_dir(char *path) {

	int fd, n, i;
	DIR *dp;
	char buf[1024], name[255], *cp;

	fd = open(path, 0);

	if(fd >= 0)
	{
		while( (n = read(fd, buf, 1024)) ) {
			cp = buf;

			while(cp < buf + 1024) {
				dp = (DIR *)cp;

				for(i = 0; i < dp->name_len; i++) {
					name[i] = dp->name[i];
				}


				name[i] = '\0';
				ls_file(name);
				cp += dp->rec_len;
				dp = (DIR *)cp;
			}
		}
		return 0;
	}

	printf("%s does not exist!\n", path);
	return -1;

}
int main(int argc, char *argv[]) {

	STAT st;
	char *path;

	if(argc < 2)
		return ls_dir(".");	
	else
		path = argv[1];

	stat(path, &st);

	if( S_ISDIR(st.st_mode) ){
		ls_dir(argv[1]);
	}
	else if ( S_ISREG(st.st_mode) ){
		ls_file(argv[1]);
	}




	return 0;


}
