

/*----------------------------------------
Spot Focus Function called in visca.c
------------------------------------------*/


#include <errno.h>
#include <fcntl.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <string.h>

#include "visca.h"  
  
    //Spot Focus 
    unsigned char spot_focus_on[] =     {0x81,0x01,0x05,0x08,0x02,0xFF};
    unsigned char spot_focus_off[] =    {0x81,0x01,0x05,0x08,0x02,0xFF};
    unsigned char spot_focus_center[] = {0x81,0x01,0x05,0x69,0x00,0x03,0x02,0x06,0x04,0xFF};
    unsigned char spot_focus_topL[] =   {0x81,0x01,0x05,0x69,0x00,0x00,0x00,0x04,0x03,0xFF};
    unsigned char spot_focus_topR[] =   {0x81,0x01,0x05,0x69,0x00,0x04,0x00,0x08,0x03,0xFF};
    unsigned char spot_focus_bottomL[] = {0x81,0x01,0x05,0x69,0x00,0x00,0x03,0x04,0x06,0xFF};
    unsigned char spot_focus_bottomR[] = {0x81,0x01,0x05,0x69,0x00,0x04,0x03,0x08,0x06,0xFF};
    unsigned char spot_focus_displayON[]= {0x81,0x01,0x05,0x15,0x02,0xFF};
    unsigned char spot_focus_displayOFF[]= {0x81,0x01,0x05,0x15,0x03,0xFF};
    
    
    void spotFocus(int fd){
    // spot focus option to choose which frame to focus on. 
           char choice[100];
           int option;
           int num;
           num = 1;
           write(fd,spot_focus_on,sizeof(spot_focus_on));
           write(fd,spot_focus_displayON,sizeof(spot_focus_displayON));
           // spot focus sub menu
           while(num == 1){
             printf("\n\n\n");
             printf("Please select which frame to focus on (0,1,2,3,4 (center) or choose 5 to quit)\n");
             // Read the user input
             if (fgets(choice, sizeof(choice), stdin) != NULL) {
               // Parse the input
               if (sscanf(choice, "%d", &option) == 1) {
                 // Handle the options
                 switch(option){
                 case 0:
                 printf("Focusing on frame %d\n", option);
                 write(fd,spot_focus_topL,sizeof(spot_focus_topL));
                 break;
                 case 1:
                 printf("Focusing on frame %d\n", option);
                 write(fd,spot_focus_topR,sizeof(spot_focus_topR));
                 break;
                 case 2:
                 printf("Focusing on frame %d\n", option);
                 write(fd,spot_focus_bottomL,sizeof(spot_focus_bottomL));
                 break;
                 case 3:
                 printf("Focusing on frame %d\n", option);
                 write(fd,spot_focus_bottomR,sizeof(spot_focus_bottomR));
                 break;
                 case 4:
                 printf("Focusing on the center of the screen\n");
                 write(fd,spot_focus_center,sizeof(spot_focus_center));
                 break;
                 case 5:
                 printf("Quitting\n");
                 write(fd,spot_focus_off,sizeof(spot_focus_off));
                 write(fd,spot_focus_displayOFF,sizeof(spot_focus_displayOFF));
                 num = 2;
                 break;
                 }
               }
             }
           }
    }
    
    
    
