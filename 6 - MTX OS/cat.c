#include "ucode.c"

int main(int argc, char *argv[ ])
{
    char buf1[1024], c;
    int fd, n, i = 0;

    if(argc > 1)
    {
        fd = open(argv[1], O_RDONLY);
        if(fd >= 0)
        {
            while((n = read(fd, buf1, 1024)))
            {
                for(i = 0; i < n; i++)
                {
                    write(1, &buf1[i], 1);
                    if(buf1[i] == '\n')
                    {
                        //write(1, "\r", 1);
                    }
                }
            }
            close(fd);
        }
    }
    else
    {
        while(1)
        {
            n = read(0, &c, 1);
            if(n < 1)
                break;
            if(c == '\r') {
                write(1, buf1, i);
                memset(buf1, 0, 1024);
                i = -1;
                write(1, "\n\r", 2);
            }
            else
                buf1[i] = c;
            i++;
        }
    }
    
}
