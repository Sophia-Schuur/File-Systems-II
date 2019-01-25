typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

/*
UART0 base address: 0x101f1000;
UART1 base address: 0x101f2000;
UART2 base address: 0x101f3000;
UART3 base address: 0x10009000;

// flag register at 0x18
//  7    6    5    4    3    2   1   0
// TXFE RXFF TXFF RXFE BUSY
// TX FULL : 0x20
// TX empty: 0x80
// RX FULL : 0x40
// RX empty: 0x10
// BUSY=1 :  0x08
*/

int N;
int v[] = {1,2,3,4,5,6,7,8,9,10};
int sum;

char *tab = "0123456789ABCDEF";

#include "string.c"
#include "uart.c"

int main()
{
  int i;
  int size = sizeof(int);
  char string[32];
  char line[128];

  N = 10;

  uart_init();  
  UART *up;

  //Run through each UART (terminal) once
  for (i=0; i<4; i++){
      up = &uart[i];
      fuprintf(up, "%s", "[!] - Enter a line from this UART: ");
      ugets(up, string);
      //fuprintf(up, "[!] - ECHO: %s\n", string);
      uprints(up, "   [!] - ECHO: "); uprints(up, string); uprints(up, "\n\r");
  }

  up = &uart[0];
  fuprintf(up, "Enter 'quit' to exit\n\r");

  //run indefinitely till user enters 'quit'
  while(1){
    ugets(up, string);
    fuprintf(up, "    ");
    if (strcmp(string, "quit")==0)
       break;
    fuprintf(up, "   [!] - ECHO: %s\n", string);
  }

  //print sum of integers in array v
  uprints(up, "\n[!] - Compute sum of array v:\n\r"); 
  sum = 0;
  fuprintf(up, "Summing integers: ");
  for (i=0; i<10; i++)
  {
    fuprintf(up, "%d ", v[i]);
    sum = sum + v[i];
  }
  fuprintf(up, "\nSum: %d\n", sum);
  fuprintf(up, "\n\rEND OF RUN\n\r");
}
