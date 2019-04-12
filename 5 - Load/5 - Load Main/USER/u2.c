#include "ucode.c"

extern int argc;
extern char *argv[32];

int main0(char *cmdline)
{
  if(cmdline != 0)
  {
    printf("Main0 Line: %s\n", cmdline);
    parseArg(cmdline);
  }
  main(argc, argv);
}

int main(int argc, char *argv[])
{
  int i;
  printf("argc : %d\n", argc);
  for(i = 0; i < argc; i++)
  {
    printf("argv[%d] : %s\n", i, argv[i]);
  }
  ubody("two");
}