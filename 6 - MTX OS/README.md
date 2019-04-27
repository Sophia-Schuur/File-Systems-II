## MTX OS
Custom OS that supports process management, memory management, device drivers, a Linux-compatible EXT2 file system 
and a command line based user interface.

### Hardware:
Uses an SDC virtual disk called `sdimage` as the primary mass storage device.

After creating the virtual SDC, we set up a loop device for the SDC partition and format it as an EXT2 file system with 4 KB block size 
and one blocks-group. The single blocks-group on the SDC image simplifies both the file system traversal and the inodes and disk blocks
management algorithms. Mount the loop device and populate it with DIRs and files, making it ready for use. 


 The sdimage is an EXT2 FS containing:

            /---|
                |----bin/ : All binary executables are in /bin
                |
                |---dev/ : special files tty0  c 4 0 
                |                        ttyS0 c 5 0 
                |                        ttyS1 c 5 1   
                |                        lp0   c 6 0
                |---etc/ : passwd file 
                |           
                |---boot/: bootable Wanix kernels
                |
                |---user/: users HOME directories


`wanix` is the base kernel used for basic syscall functions. 

The Fresh Copy of `sdimage` contains no custom commands, only what is in the wanix. 

### Initialization: 
After mounting the root file system, P0 creats P1, whose Umode image is 
  the /bin/init program. P1 will go Umode directly to execute /bin/init, 
  in which it forks children P2 on /dev/tty0. P1 may also fork P3 /dev/ttyS0 
  and P4 on /dev/ttyS1, etc. Then P1 waits for any child to die. Henceforth, P1
  plays the role of the INIT process (P1) in Unix.
  
### Process: 
P2 is a login process. It executes /bin/login on the console terminal 
/dev/tty0. So the special file /dev/tty0 MUST exist. To support serial 
terminals and printer, /dev/ttyS0, /dev/ttyS1 and /dev/lp0 must also exist.

1. In /bin/login, P2 opens its tty special file (/dev/tty0) as stdin(0), stdout(1) and stderr(2). It prompts the user to login
and waits for him to do so. When a user tries to login, it reads the username and password (from its stdin) and opens 
the /etc/passwd file to authenticate the user.

2. If the user has a valid account in /etc/passwd, P2 becomes the user's process (by taking on the user's uid). 
It chdir to user's HOME directory and executes the listed program, which is normally the sh (/bin/sh).
Then (in sh) it loops forever until the user enters `logout`.

3. When the child proc terminates (by exit() syscall to WANIX kernel), it wakes up sh, which prompts for another cmdLine, etc.

4. When sh dies, it wakes up its parent, INIT, which forks another login process.

### Run:
`wanix` supports dynamic process creation and termination. So, there are two methods to run this program.
##### To add or edit a command in the bin directory to be run as an executable command in `wanix`:
`sh mk <filename>` without the .c at the end. Use this to dynamically add or edit existing commands that can then be used directly
by the user in the shell. Upon using a fresh sdimage, all custom commands will need to be reallocated this way. 

##### To run without adding or editing any commands:
`sh run`

##### Some logins:
* Username: root | Password: 12345. Can access bin, dev, boot, etc, etc.. and has some ready to go files to mess with. Try `cat f`!
* Username: kcw | Password: abcde. Nothing in here

##### My current custom commands: 
###### Startup commands:
* init
* login
* sh
* Single and double piping (code in sh.c)
* I/O redirects >, <, <<, >>

###### User commands:
* ls [filename]
* cat [filename]
* more [filename]
* grep [pattern] [filename] 
* l2u (convert lower to upper) [file1] [file2]
* cp [source] [destination]

### Known Bugs:
* Triple piping is broken. 
