

/*--------------------------------
Camera Function Called in visca.c
----------------------------------*/

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

#define BUFFER_SIZE 256
  
     //Camera Function
       //Power
       unsigned char power_on[] = {0x81,0x01,0x04,0x00,0x02,0xFF};
       unsigned char power_off[] = {0x81,0x01,0x04,0x00,0x03,0xFF};
       
       //Reset
       unsigned char reset[] = {0x81,0x01,0x04,0x19,0x03,0xFF};
       //Mute
       unsigned char mute_on[] = {0x81,0x01,0x04,0x75,0x02,0xFF};
       unsigned char mute_off[] = {0x81,0x01,0x04,0x75,0x03,0xFF};       
       //Lens Initialize
       unsigned char lens_init[]= {0x81,0x01,0x04,0x19,0x01,0xFF};
       //reverse
       unsigned char lr_reverse_on[] = {0x81, 0x01,0x04,0x61,0x02,0xFF};
       unsigned char lr_reverse_off[] = {0x81, 0x01,0x04,0x61,0x03,0xFF};
       //freeze
       unsigned char freeze_on[] = {0x81, 0x01,0x04,0x62,0x02,0xFF};
       unsigned char freeze_off[] = {0x81, 0x01,0x04,0x62,0x03,0xFF};   
    
       //monochrome
       unsigned char monochrome_on[] = {0x81, 0x01,0x04,0x63,0x04,0xFF};
       unsigned char monochrome_off[] = {0x81, 0x01,0x04,0x63,0x00,0xFF}; 
       
       //flip
       unsigned char picflip_on[] = {0x81,0x01,0x04,0x66,0x02,0xFF};
       unsigned char picflip_off[] = {0x81,0x01,0x04,0x66,0x03,0xFF};
       

       
       //function
       void camera(int fd){
	   bool mute, power, choiceReverse, choiceFreeze, choiceFlip, choiceMono;
           bool camera;
           camera = true;
           char inputCAM[BUFFER_SIZE];  
           int optionCAM;
           while(camera){
             printf("\n\n\n");
             printf("Choose 1 of the following options:\n");
             printf("1) Power On/Off\n2) Reset\n3) Mute\n4) Lens initialize\n5) CameraID write(In progress)\n6) LR reverse\n7) Freeze\n8) Monochrome\n9) Picture Flip\n0) Quit\n");
             if(fgets(inputCAM,sizeof(inputCAM),stdin)!=NULL){
               if(sscanf(inputCAM,"%d",&optionCAM) == 1){
                 switch(optionCAM){
                 case 1:
                
                 power = true;
                 char inputPow[BUFFER_SIZE];
                 int optionPow;
                 while(power){
                 printf("\n\n\n");
                 printf("Choose 1 of the following options:\n");
                 printf("1) Off\n2) On\n0) Quit\n");
                   if(fgets(inputPow,sizeof(inputPow),stdin)!=NULL){
                     if(sscanf(inputPow,"%d",&optionPow)==1){
                       switch(optionPow){
                       case 1:
                       printf("Power turned OFF\n");
                       write(fd,power_off,sizeof(power_off));
                       break;
                       
                       case 2:
                       printf("Power turned ON\n");
                       write(fd,power_on,sizeof(power_on));
                       break;
                       
                       case 0:
                        printf("Quitting Power");
                       power = false;
                       break;
                       
                       }
                     }
                   
                   }
                 
                 }
                 break;
                 case 2:
                 printf("Resetting\n");
                 write(fd,reset,sizeof(reset));
                 sleep(5);
                 break;
                
                 case 3:
                
                 mute = true;
                 char inputMute[BUFFER_SIZE];
                 int optionMute;
                 while(mute){
                 printf("\n\n\n");
                 printf("Choose 1 of the following options:\n");
                 printf("1) Off\n2) On\n0) Quit\n");
                   if(fgets(inputMute,sizeof(inputMute),stdin)!=NULL){
                     if(sscanf(inputMute,"%d",&optionMute)==1){
                       switch(optionMute){
                       case 1:
                       printf("Mute Off\n");
                       write(fd,mute_off,sizeof(mute_off));
                       break;
                       
                       case 2:
                       printf("Mute On\n");
                       write(fd,mute_on,sizeof(mute_on));
                       break;
                       
                       case 0:
                        printf("Quitting Mute");
                       mute = false;
                       break;
                       
                       }
                     }
                   
                   }
                 
                 }
                 break;
                 case 4:
                 printf("Lens is initializing\n");
                 write(fd,lens_init,sizeof(lens_init));
                 
                 break;
                 case 5:
                 printf("camera write\n");
                 break;
                 case 6:
                
                 choiceReverse = true;
           	 char bufReverse[BUFFER_SIZE];
                 int optionReverse;
                 while(choiceReverse){
          	   printf("\n\n\n");
          	   printf("Choose one option:\n");
                   printf("1) Reverse On\n2) Reverse Off\n0) Quit\n");
                     if(fgets(bufReverse,sizeof(bufReverse),stdin)!=NULL){
                       if(sscanf(bufReverse,"%d",&optionReverse) == 1){
                         switch(optionReverse){
                         case 1:
                         printf("Reverse on\n");
                         write(fd,lr_reverse_on,sizeof(lr_reverse_on));
                         break;
                         case 2:
                         printf("Reverse Off\n");
                         write(fd,lr_reverse_off,sizeof(lr_reverse_off));
                         break;
                         case 0:// reverse quit
                         choiceReverse = false;
                         break;
                         }
                       }
                     }
                   }
                 break;
                 case 7:
               
                 choiceFreeze = true;
                 char bufFreeze[BUFFER_SIZE];
                 int optionFreeze;
                 while(choiceFreeze){
          	   printf("\n\n\n");
          	   printf("Choose one option:\n");
                   printf("1) Freeze On\n2) Freeze Off\n0) Quit\n");
                   if(fgets(bufFreeze,sizeof(bufFreeze),stdin)!=NULL){
                     if(sscanf(bufFreeze,"%d",&optionFreeze) == 1){
                         switch(optionFreeze){
                         case 1:
                         printf("Freeze on\n");
                         write(fd,freeze_on,sizeof(freeze_on));
                         break;
                         case 2:
                         printf("Freeze Off\n");
                         write(fd,freeze_off,sizeof(freeze_off));
                         break;
                         case 0:// freeze quit
                         choiceFreeze = false;
                         break;
                         }
                       }
                     }
                   }
                 break;
                 case 8:
               
                 choiceMono = true;
                 char bufMono[BUFFER_SIZE];
                 int optionMono;
                 while(choiceMono){
          	   printf("\n\n\n");
          	   printf("Choose one option:\n");
                   printf("1) Monochrome On\n2) Monochrome Off\n0) Quit\n");
                   if(fgets(bufMono,sizeof(bufMono),stdin)!=NULL){
                     if(sscanf(bufMono,"%d",&optionMono) == 1){
                       switch(optionMono){
                       case 1:
                       printf("Monochrome on\n");
                       write(fd,monochrome_on,sizeof(monochrome_on));
                       break;
                       case 2:
                       printf("Monochrome Off\n");
                       write(fd,monochrome_off,sizeof(monochrome_off));
                       break;
                       case 0:
                       choiceMono = false;
                       break;                        }
                     }
                   }
                 }
                 break;
                 case 9:
               
                 choiceFlip = true;
                 char bufFlip[BUFFER_SIZE];
                 int optionFlip;
                 while(choiceFlip){
          	   printf("\n\n\n");
          	   printf("Choose one option:\n");
                   printf("1) Picture Flip On\n2) Picture Flip Off\n0) Quit\n");
                   if(fgets(bufFlip,sizeof(bufFlip),stdin)!=NULL){
                     if(sscanf(bufFlip,"%d",&optionFlip) == 1){
                       switch(optionFlip){
                       case 1:
                       printf("Picture Flip on\n");
                       write(fd,picflip_on,sizeof(picflip_on));
                       break;
                       case 2:
                       printf("Picture Flip Off\n");
                       write(fd,picflip_off,sizeof(picflip_off));
                       break;
                       case 0:
                       choiceFlip = false;
                       break;                        }
                     }
                   }
                 }
                 break;
                 case 0:
                 printf("Quitting Camera");
                 camera = false;
                 break;

                 }
               }
             }
           
           }
       
       }
  
  
  

    
    
