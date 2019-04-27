#include "ucode.c"

int console, console2, console3;

int parent()
{
    int pid, status;
    while(1)
    {
        pid = wait(&status);
        if(pid == console)
        {
            console = fork();
            if(console)
            {
                continue;
            }
            else
            {
                exec("login /dev/tty0");
            }
        }
        else if(pid == console2)
        {
            console2 = fork();
            if(console2)
            {
                continue;
            }
            else
            {
                exec("login /dev/ttyS0");
            }
        }
        if(pid == console3)
        {
            console3 = fork();
            if(console3)
            {
                continue;
            }
            else
            {
                exec("login /dev/ttyS1");
            }
        }
    }
}

int main(int argc, char *argv[ ])
{
    int in, out, in2, out2, in3, out3;
    in = open("/dev/tty0", O_RDONLY);
    out = open("/dev/tty0", O_WRONLY);

    console = fork();
    if(console)
    {
        in2 = open("/dev/ttyS0", O_RDONLY);
        out2 = open("/dev/ttyS0", O_WRONLY);
        console2 = fork();
        if(console2)
        {
            in3 = open("/dev/ttyS1", O_RDONLY);
            out3 = open("/dev/ttyS1", O_WRONLY);
            console3 = fork();
            if(console3)
            {
                parent();
            }
            else
            {
                exec("login /dev/ttyS1");
            }
        }  
        else
        {
            exec("login /dev/ttyS0");
        }
    }
    else
    {
        exec("login /dev/tty0");
    }
}
