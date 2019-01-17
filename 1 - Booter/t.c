#include "ext2.h"

#define BLK 1024

u16 prints(char *s)
{
	while (*s) 
	{
		putc(*s++);
	}
}

u16 gets(char s[ ])
{
	while ((*s = getc()) != '\r') 
	{
		putc(*s++);
	}
	*s = 0;
}

u16 getblk(u16 blk, char buf[ ])
{
   readfd(blk/18, (blk/9)%2, (blk*2)%18, buf);
}

INODE *search(char *str, INODE *ip, u16 iblk)
{
    u8 i;
    DIR   *dp;
    char *cp, buf[BLK], temp[64];
    
    for (i = 0; i < 12; i++)
    {
        if (ip->i_block[i] == 0) break;
        
        getblk((u16)ip->i_block[i], buf);
        dp = (DIR *)buf;
        cp = buf;
        
        while (cp < buf + BLK)
        {
        	//make temp into a string
            strncpy(temp, dp->name, dp->name_len);
            //prints(dp->name);
            temp[dp->name_len] = 0;
            
            //found name we are looking for?
            if (!strcmp(str, temp))
            {
                i = dp->inode - 1;
                getblk((i / 8) + iblk, buf);
                
                //return name's inode
                return (INODE *)buf + (i % 8);
            }
            //go to next dir
            cp += dp->rec_len;
            dp = (DIR*)cp;
        }
    }
    return -1;
}

main()
{
    GD    *gp;
    INODE *ip;
    DIR   *dp;
    u16  i, iblk;
    u32 *iblk2;
    char buf1[BLK], buf2[BLK], image[64];
        

    //inodes begin block
    getblk((u16)2, buf1);
    gp = (GD *)buf1;
    iblk = (u16)gp->bg_inode_table; 

    //read first inode block
    getblk(iblk, buf1); 
    //ip->root is inode #2 so we add one
    ip = (INODE *)buf1 + 1; 
    
    //get image name from user
    prints("Image boot: "); 
    gets(image); 
    
    //search for boot inode
    ip = search("boot", ip, iblk);
    if (ip < 0) error();

    //search for image name inode
    ip = search(image, ip, iblk);
    if (ip < 0) error();
    
    //read indirect block into buf2
    i = ip->i_block[12];
    getblk(i, buf2);
    iblk2 = buf2;
    
    //set ES to loading segment
    setes(0x1000); 
    
    //direct blox
    for (i = 0; i < 12; i++){
        getblk(ip->i_block[i], 0);
        inces(); //putc('*');
    }
    
    //indirext blox
    while (*iblk2 != 0){
        getblk(*iblk2++, 0);
        inces();	//putc('.');
    }


    // if((u16)ip->i_block[12])
    // {
    // 	iblk2 = (u32 *)buf2;
    // 	while(*iblk2++)
    // 	{
    // 		getblk((u32)*iblk2, 0);
    // 		inces(); //putc('.');
    // 	}
    // }
    
}
