#define DR   0x00
#define FR   0x18
#define RXFE 0x10
#define TXFF 0x20
//char *tab = "0123456789ABCDEF";

typedef struct uart{
  char *base;
  int n;
}UART;

UART uart[4];

int uart_init()
{
  int i; UART *up;

  for (i=0; i<4; i++)	//uart0 to uart2 are adjacent
  {
    up = &uart[i];
    up->base = (char *)(0x10009000 + i*0x1000);
    up->n = i;
  }
  // uart[3].base = (char *)(0x10009000); // uart3 at 0x10009000
}

//input a char from UART pointed by up
int ugetc(UART *up)
{
  while (*(up->base + FR) & RXFE);	//loop if UFR is RXFE
  return *(up->base + DR);			//return a char in UDR
}

//output a char to UART pointed by up
int uputc(UART *up, char c)
{
  while(*(up->base + FR) & TXFF);	//loop if UFR is TXFF
  *(up->base + DR) = c;				//white char to data register
}

//input a string of chars
int ugets(UART *up, char *s)	
{
  while ((*s = (char)ugetc(up)) != '\r'){
    uputc(up, *s);
    s++;
  }
  *s = 0;
}

//output a string of chars
int uprints(UART *up, char *s)
{
  while(*s)
    uputc(up, *s++);
}

int urpx(UART *up,int x)
{
  char c;
  if(x)
  {
    c = tab[x % 16];
    urpx(up,x / 16);
  }
  uputc(up, c);
}

//prints an int in hex
int uprintx(UART *up, int x)
{
  uputc(up,'0'); uputc(up,'x');
  if(x==0)
    uputc(up,'0');
  else
    urpx(up,x);
  uputc(up,' ');
}

int urpu(UART *up,int x)
{
  char c;
  if(x)
  {
    c = tab[x % 10];
    urpu(up,x /10);
    uputc(up,c);
  }
  
}

//print unsigned int
int uprintu(UART *up,int x)
{
  if(x==0)
    uputc(up,'0');
  else
    urpu(up,x);
  uputc(up,' ');
}

//print int
int uprinti(UART *up, int x)
{
  if(x<0)
  {
    uputc(up,'-');
    x =-x;
  }
  uprintu(up,x);
}

//custom print function works just like printf()
int fuprintf(UART *up,char *fmt, ...)
{
  int *ip;
  char *cp;
  cp = fmt;
  ip = (int *)&fmt + 1;

  while(*cp)
  {
    if(*cp != '%')
    {
      uputc(up,*cp);
      if(*cp == '\n')
        uputc(up,'\r');
      cp++;
      continue;
    }
    cp++;
    switch(*cp)
    {
      case 'c': uputc(up,(char)*ip); break;
      case 's': uprints(up,(char *)*ip);break;
      case 'd': uprinti(up, *ip); break;
      case 'u': uprintu(up,*ip); break;
      case 'x': uprintx(up,*ip); break;

    }
    cp++; ip++;
  }
}

