typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

#include "string.c"
#include "uio.c"

int ubody(char *name)
{
  int pid, ppid;
  char line[64];
  u32 mode,  *up;

  mode = getcsr();
  mode = mode & 0x1F;
  printf("CPU mode=%x\n", mode);
  pid = getpid();
  ppid = getppid();

  while(1){
    printf("This is process #%d in Umode at %x parent=%d\n", pid, getPA(),ppid);
    umenu();
    printf("input a command : ");
    ugetline(line); 
    uprintf("\n"); 
 
    if (strcmp(line, "getpid")==0)
       ugetpid();
    if (strcmp(line, "getppid")==0)
       ugetppid();
    if (strcmp(line, "ps")==0)
       ups();
    if (strcmp(line, "chname")==0)
       uchname();
    if (strcmp(line, "sleep") == 0)
       usleep();
    if (strcmp(line, "wakeup") == 0)
       uwakeup();
    if (strcmp(line, "wait") == 0)
       uwait();
    if (strcmp(line, "switch")==0)
       uswitch();
    if (strcmp(line, "exit") == 0)
      uexit();
    if (strcmp(line, "fork") == 0)
      ufork();
  }
}

int ufork()
{
  char s[12];
  uprintf("enter user type to fork on (u1, u2, u3, u4) : ");
  ugetline(s);
  return syscall(9, s, 0, 0);
}

int uexit()
{
  return syscall(8,0,0,0);
}

int usleep()
{
  char s[12];
  int i;
  uprintf("enter an event value to sleep on : ");
  ugetline(s);
  i = atoi(s);
  return syscall(6,i,0,0);
}

int uwakeup()
{
  char s[12];
  int i;
  uprintf("enter an event value to wakeup with : ");
  ugetline(s);
  i = atoi(s);
  return syscall(7,i,0,0);
}

int uwait()
{
  return syscall(5,0,0,0);
}

int umenu()
{
  uprintf("-----------------------------------------------------------\n");
  uprintf("getpid getppid ps chname switch sleep wait wakeup exit fork\n");
  uprintf("-----------------------------------------------------------\n");
}

int getpid()
{
  int pid;
  pid = syscall(0,0,0,0);
  return pid;
}    

int getppid()
{ 
  return syscall(1,0,0,0);
}

int ugetpid()
{
  int pid = getpid();
  uprintf("pid = %d\n", pid);
}

int ugetppid()
{
  int ppid = getppid();
  uprintf("ppid = %d\n", ppid);
}

int ups()
{
  return syscall(2,0,0,0);
}

int uchname()
{
  char s[32];
  uprintf("input a name string : ");
  ugetline(s);
  printf("\n");
  return syscall(3,s,0,0);
}

int uswitch()
{
  return syscall(4,0,0,0);
}


int ugetc()
{
  return syscall(90,0,0,0);
}

int uputc(char c)
{
  return syscall(91,c,0,0);
}

int getPA()
{
  return syscall(92,0,0,0);
}
