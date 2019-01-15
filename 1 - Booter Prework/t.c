
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#include "ext2.h"
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

#define BLK 1024
char buf1[BLK], buf2[BLK];
int iblock;

int myprints(char *s)
{
  while(*s)
  {
    putc(*s++);
  }
  return 0;
}

int mygets(char s[]) 
{ 
  while ( (*s = getc()) != '\r')
  {   
    putc(*s++);
  }
  *s = 0;
  return 0;
}

u16 getblk(u16 blk, char *buf)
{
	//readfd( blk/18, (blk / 9) % 2, (blk*2)%18, buf);
	readfd( blk/18, ((blk)%18)/9, ( ((blk)%18)%9)<<1, buf);
}

char *dev = "mtximage";
char temp[256];
main()
{

	char name[64];
	u16    i,iblk;
	char   c;
	GD    *gp;
	INODE *ip;
	DIR   *dp;
	u32   *bp;
	char *cp;
	char *x;

	getblk(2, buf1);
	
	myprints("What's your name? ");
	mygets(name);    
    myprints("\n\r"); myprints("We've been expecting you, "); myprints(name); myprints("\n\r");
  
	gp = (GD *)buf1;
	iblk = (u16)gp->bg_inode_table;

	getblk((u16)iblk, buf1);  
	ip = (INODE *)buf1 + 1;   



  	myprints("inode_block="); putc(iblk+'0'); myprints("\n\r");
  
  	getblk((u16)ip->i_block[0], buf2);
  	dp = (DIR *)buf2;         // buf2 contains DIR entries


  	myprints(" Contents of "); myprints(dev); myprints("\n\r");

  	while((char *)dp < &buf2[BLK])
  	{
  		x = dp->name[dp->name_len];
  		dp->name[dp->name_len] = 0;
  		myprints("\n\r"); myprints(dp->name); 
  	
  		dp = (DIR *)((char *)dp + dp->rec_len);
  	}
	return 1;
}
