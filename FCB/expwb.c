

/*------------------------------------------------------
Exposure and White Balance Function Called in visca.c
--------------------------------------------------------*/

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
 
 // Only Focusing on Auto Exposure (Full Auto, Manual, and shutter/iris priority) and White Balance Commands
 
    //Auto Exposure
    unsigned char ae_full_auto[] = {0x81,0x01,0x04,0x39,0x00,0xFF};
    unsigned char ae_manual[] = {0x81,0x01,0x04,0x39,0x03,0xFF};
    
     //Priorities
    unsigned char shutter_priority[] = {0x81,0x01,0x04,0x39,0x0A,0xFF};
    unsigned char iris_priority[] = {0x81,0x01,0x04,0x39,0x0B,0xFF};

 
    //Shutter
    unsigned char shutter_reset[] = {0x81, 0x01,0x04,0x0A,0x00,0xFF}; 
    unsigned char shutter_inc[] = {0x81, 0x01,0x04,0x0A,0x02,0xFF};
    unsigned char shutter_dec[] = {0x81, 0x01,0x04,0x0A,0x03,0xFF};
   //unsigned char shutter_direct[] = {0x81,0x01,0x04,0x4A,0x00,0x00,0x00,0x0A,0xFF};
    
    //iris
    unsigned char iris_reset[] = {0x81, 0x01,0x04,0x0B,0x00,0xFF}; 
    unsigned char iris_inc[] = {0x81, 0x01,0x04,0x0B,0x02,0xFF};
    unsigned char iris_dec[] = {0x81, 0x01,0x04,0x0B,0x03,0xFF};
    
    //gain 
    unsigned char gain_reset[] = {0x81, 0x01,0x04,0x0C,0x00,0xFF}; 
    unsigned char gain_inc[] = {0x81, 0x01,0x04,0x0C,0x02,0xFF};
    unsigned char gain_dec[] = {0x81, 0x01,0x04,0x0C,0x03,0xFF};
    
    // White Balance
    unsigned char wb_auto[] =         {0x81,0x01,0x04,0x35,0x00,0xFF};
    unsigned char wb_indoor[] =       {0x81,0x01,0x04,0x35,0x01,0xFF};
    unsigned char wb_outdoor[] =      {0x81,0x01,0x04,0x35,0x02,0xFF};
    unsigned char wb_onepush_wb[] =      {0x81,0x01,0x04,0x35,0x03,0xFF};
    unsigned char wb_atw[] =          {0x81,0x01,0x04,0x35,0x04,0xFF};
    unsigned char wb_manual_wb[] =       {0x81,0x01,0x04,0x35,0x05,0xFF};
    unsigned char wb_onepush_trigger[] = {0x81,0x01,0x04,0x10,0x05,0xFF};
    unsigned char wb_outdoor_auto[] = {0x81,0x01,0x04,0x35,0x06,0xFF};
    unsigned char wb_sodlamp_auto[] = {0x81,0x01,0x04,0x35,0x07,0xFF};
    unsigned char wb_sodlamp[] =      {0x81,0x01,0x04,0x35,0x08,0xFF};
    unsigned char wb_sodlamp_out_auto[] =  {0x81,0x01,0x04,0x35,0x09,0xFF};
    
    // Manual wb
    unsigned char rgain_reset[] = {0x81,0x01,0x04,0x03,0x00,0xFF};
    unsigned char rgain_inc[] =   {0x81,0x01,0x04,0x03,0x02,0xFF};
    unsigned char rgain_dec[] = {0x81,0x01,0x04,0x03,0x03,0xFF}; 
    
    unsigned char bgain_reset[] = {0x81,0x01,0x04,0x04,0x00,0xFF};
    unsigned char bgain_inc[] = {0x81,0x01,0x04,0x04,0x02,0xFF};
    unsigned char bgain_dec[] = {0x81,0x01,0x04,0x04,0x03,0xFF};
    
     
    
  void expwb(int fd){
    
    
    bool expwb,ae,manual, shutter,iris,gain, shutterPriority,irisPriority,wb, wb_manual,rgain,bgain;
    expwb = true;
    char inpEXPWB[BUFFER_SIZE];
    int optEXPWB;
    while(expwb){
      printf("\n\n\n");
      printf("Please choose one of the following options\n");
      printf("1) AE Mode\n2) WB\n0) Quit\n");
      if(fgets(inpEXPWB,sizeof(inpEXPWB), stdin)!=NULL){
        if(sscanf(inpEXPWB, "%d", &optEXPWB) == 1){
          switch(optEXPWB){
          case 1:
         
          ae = true;
          char inpAE[BUFFER_SIZE];
          int optAE;
          while(ae){
          printf("\n\n\n");
          printf("Please choose one of the following options\n");
          printf("1) AE Full Auto\n2) AE Manual\n3) Shutter Priority\n4) Iris Priority\n0) Quit\n");
          if(fgets(inpAE, sizeof(inpAE), stdin)!= NULL){
            if(sscanf(inpAE, "%d",&optAE) == 1){
              switch(optAE){
              case 1:
              printf("AE Full Auto On\n");
              write(fd, ae_full_auto, sizeof(ae_full_auto));
              break;
              case 2:
             // AE Manual control (sub menu)
            
             manual = true;
             char inpMan[BUFFER_SIZE];
             int optMan;
             write(fd,ae_manual,sizeof(ae_manual)); // turn manual on
             while(manual){
               printf("\n\n\n");
               printf("Manual Mode Activated\n");
               printf("Please choose 1 of the following options\n");
               printf("1) Shutter\n2) Iris\n3) Gain\n0) Quit\n");           
               
               if (fgets(inpMan,sizeof(inpMan),stdin) != NULL){
                 if(sscanf(inpMan,"%d",&optMan) == 1){
         
                   switch(optMan){   
                   case 1:
                     //Shutter Control
                  
                     shutter = true;
                     char inputShut[BUFFER_SIZE];
                     int optShut;

                     while (shutter){
                       printf("\n\n\n");
                       printf("Shutter Mode\n");
                       printf("Please choose 1 of the following options\n");
                       printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                       if (fgets(inputShut,sizeof(inputShut),stdin) != NULL){
                         if(sscanf(inputShut,"%d",&optShut) == 1){
                           switch(optShut){
                           case 1:
                           printf("Resetting shutter\n");
                           write(fd,shutter_reset,sizeof(shutter_reset));
                           break;
                           case 2:
                           printf("Incrementing shutter\n");
                           write(fd,shutter_inc,sizeof(shutter_inc));
                           break;
                           case 3:
                           printf("Decrementing shutter\n");
                           write(fd,shutter_dec,sizeof(shutter_dec));
                           break;
                           case 0:
                           printf("Quitting\n");
                           shutter = false;
                           break;
                           }
                         }
                       }
                     }
                   break;
                   case 2:
                   // Iris control
                                       
               
                     iris = true;
                     char inputIris[BUFFER_SIZE];
                     int optIris;

                     while (iris){
                       printf("\n\n\n");
                       printf("Iris Mode\n");
                       printf("Please choose 1 of the following options\n");
                       printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                       if (fgets(inputIris,sizeof(inputIris),stdin) != NULL){
                         if(sscanf(inputIris,"%d",&optIris) == 1){
                           switch(optIris){
                           case 1:
                           printf("Resetting iris\n");
                           write(fd,iris_reset,sizeof(iris_reset));
                           break;
                           case 2:
                           printf("Incrementing iris\n");
                           write(fd,iris_inc,sizeof(iris_inc));
                           break;
                           case 3:
                           printf("Decrementing iris\n");
                           write(fd,iris_dec,sizeof(iris_dec));
                           break;
                           case 0:
                           printf("Quitting\n");
                           iris = false;
                           break;
                           }
                         }
                       }
                     }
                   break;
                   case 3:
                   // Gain control
                                       
                  
                     gain = true;
                     char inputGain[BUFFER_SIZE];
                     int optGain;

                     while (gain){
                       printf("\n\n\n");
                       printf("Gain Mode\n");
                       printf("Please choose 1 of the following options\n");
                       printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                       if (fgets(inputGain,sizeof(inputGain),stdin) != NULL){
                         if(sscanf(inputGain,"%d",&optGain) == 1){
                           switch(optGain){
                           case 1:
                           printf("Resetting gain\n");
                           write(fd,gain_reset,sizeof(gain_reset));
                           break;
                           case 2:
                           printf("Incrementing gain\n");
                           write(fd,gain_inc,sizeof(gain_inc));
                           break;
                           case 3:
                           printf("Decrementing gain\n");
                           write(fd,gain_dec,sizeof(gain_dec));
                           break;
                           case 0:
                           printf("Quitting\n");
                           gain = false;
                           break;
                           }
                         }
                       }
                     }
                   case 0:
                   printf("Quitting\n");// quitting manual
                   manual = false;
                   break;
                   }// manual switch 
                 } 
               }      
             }//manual whileloop
              break;
              case 3:
              //Shutter Priority
              write(fd, shutter_priority, sizeof(shutter_priority));
             
              shutterPriority = true;
              char inputSP[BUFFER_SIZE];
              int optSP;

              while (shutterPriority){
                printf("\n\n\n");
                printf("Shutter Priority Mode\n");
                printf("Please choose 1 of the following options\n");
                printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                if (fgets(inputSP,sizeof(inputSP),stdin) != NULL){
                  if(sscanf(inputSP,"%d",&optSP) == 1){
                    switch(optSP){
                    case 1:
                    printf("Resetting shutter\n");
                    write(fd,shutter_reset,sizeof(shutter_reset));
                    break;
                    case 2:
                    printf("Incrementing shutter\n");
                    write(fd,shutter_inc,sizeof(shutter_inc));
                    break;
                    case 3:
                    printf("Decrementing shutter\n");
                    write(fd,shutter_dec,sizeof(shutter_dec));
                    break;
                    case 0:
                    printf("Quitting\n");
                    shutterPriority = false;
                    break;
                    }
                  }
                }
              }
              break;
              case 4:
              //Iris Priority
              write(fd, iris_priority, sizeof(iris_priority));
              
              irisPriority = true;
              char inputIP[BUFFER_SIZE];
              int optIP;

              while (irisPriority){
                printf("\n\n\n");
                printf("Iris Priority Mode\n");
                printf("Please choose 1 of the following options\n");
                printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                if (fgets(inputIP,sizeof(inputIP),stdin) != NULL){
                  if(sscanf(inputIP,"%d",&optIP) == 1){
                    switch(optIP){
                    case 1:
                    printf("Resetting iris\n");
                    write(fd,iris_reset,sizeof(iris_reset));
                    break;
                    case 2:
                    printf("Incrementing iris\n");
                    write(fd,iris_inc,sizeof(iris_inc));
                    break;
                    case 3:
                    printf("Decrementing iris\n");
                    write(fd,iris_dec,sizeof(iris_dec));
                    break;
                    case 0:
                    printf("Quitting\n");
                    irisPriority = false;
                    break;
                    }
                  }
                }
              }
              break;
              case 0:
              printf("Quitting");
              ae = false;
              break;
              }
            }
          }
 
          }
          break;
          case 2:
         
          wb = true;
          char inpWB[BUFFER_SIZE];
          int optWB;
          
          while(wb){
          printf("\n\n\n");
          printf("Please choose one of the following options\n");
          printf("1) Auto\n");
          printf("2) ATW\n");
          printf("3) Indoor\n");
          printf("4) Outdoor\n");
          printf("5) One Push WB\n");
          printf("6) Manual\n");
          printf("7) One Push Trigger\n");
          printf("8) Outdoor Auto\n");
          printf("9) Sodium Vapor Lamp Auto\n");
          printf("10) Sodium Vapor Lamp\n");
          printf("11) Sodium Vapor Lamp OutdoorAuto\n");
          printf("0) Quit\n");
          
          if(fgets(inpWB, sizeof(inpWB),stdin) != NULL){
            if(sscanf(inpWB,"%d",&optWB)==1){
              switch(optWB){
                case 1:
                printf("Auto on\n");
                write(fd,wb_auto,sizeof(wb_auto));
                break;
                case 2:
                printf("ATW on\n");
                write(fd,wb_atw,sizeof(wb_atw));
                break;
                case 3:
                printf("Indoor on\n");
                write(fd,wb_indoor,sizeof(wb_indoor));
                break;
                case 4:
                printf("Outdoor on\n");
                write(fd,wb_outdoor,sizeof(wb_outdoor));
                break;
                case 5:
                printf("One Push WB on\n");
                write(fd,wb_onepush_wb,sizeof(wb_onepush_wb));
                break;
                case 6:
                printf("Manual on\n");
                write(fd, wb_manual_wb, sizeof(wb_manual_wb));
               
                wb_manual = true;
                char inpWBMan[BUFFER_SIZE];
                int optWBMan;
                
                while(wb_manual){
                printf("\n\n\n");
                printf("Please choose one of the following options\n");
                printf("1) RGain\n");
                printf("2) BGain\n");
                printf("0) Quit\n");
                if(fgets(inpWBMan, sizeof(inpWBMan),stdin) != NULL){
                  if(sscanf(inpWBMan, "%d", &optWBMan) == 1){
                    switch(optWBMan){
                    case 1:
             
                    rgain = true;
                    char inputRGain[BUFFER_SIZE];
                    int optRGain;

                    while (rgain){
                      printf("\n\n\n");
                      printf("RGain Mode\n");
                      printf("Please choose 1 of the following options\n");
                      printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                      if (fgets(inputRGain,sizeof(inputRGain),stdin) != NULL){
                        if(sscanf(inputRGain,"%d",&optRGain) == 1){
                          switch(optRGain){
                          case 1:
                          printf("Resetting Rgain\n");
                          write(fd,rgain_reset,sizeof(rgain_reset));
                          break;
                          case 2:
                          printf("Incrementing Rgain\n");
                          write(fd,rgain_inc,sizeof(rgain_inc));
                          break;
                          case 3:
                          printf("Decrementing Rgain\n");
                          write(fd,rgain_dec,sizeof(rgain_dec));
                          break;
                          case 0:
                          printf("Quitting\n");
                          rgain = false;
                          break;
                          }
                        }
                      }
                    }
                    break;
                    case 2:
                    printf("BGain Mode\n");
            
                    bgain = true;
                    char inputBGain[BUFFER_SIZE];
                    int optBGain;

                    while (bgain){
                      printf("\n\n\n");
                      printf("BGain Mode\n");
                      printf("Please choose 1 of the following options\n");
                      printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");
                      if (fgets(inputBGain,sizeof(inputBGain),stdin) != NULL){
                        if(sscanf(inputBGain,"%d",&optBGain) == 1){
                          switch(optBGain){
                          case 1:
                          printf("Resetting Bgain\n");
                          write(fd,bgain_reset,sizeof(bgain_reset));
                          break;
                          case 2:
                          printf("Incrementing Bgain\n");
                          write(fd,bgain_inc,sizeof(bgain_inc));
                          break;
                          case 3:
                          printf("Decrementing Bgain\n");
                          write(fd,bgain_dec,sizeof(bgain_dec));
                          break;
                          case 0:
                          printf("Quitting\n");
                          bgain = false;
                          break;
                          }
                        }
                      }
                    }
                    break;
                    case 0:
                    wb_manual = false;
                    printf("Quitting\n");
                    break;
                    }
                  }
                 }
                }     
                break;
                case 7:
                printf("One Push Trigger on\n");
                write(fd,wb_onepush_trigger,sizeof(wb_onepush_trigger));
                break;
                case 8:
                printf("Outdoor Auto on\n");
                write(fd,wb_outdoor_auto, sizeof(wb_outdoor_auto));
                break;
                case 9:
                printf("Sodium Vapor Lamp Auto on\n");
                write(fd,wb_sodlamp_auto,sizeof(wb_sodlamp_auto));
                break;
                case 10:
                printf("Sodium Vapor Lamp on\n");
                write(fd,wb_sodlamp,sizeof(wb_sodlamp));
                break;
                case 11:
                printf("Sodium Vapor Outdoor Lamp on\n");
                write(fd,wb_sodlamp_out_auto,sizeof(wb_sodlamp_out_auto));
                break;
                case 0:
                printf("Quitting");
                wb = false;
                break;
                }
              }
            }
          }
          break;
          case 0:
          expwb = false; // quitting EXP/WB
          break;
          
          }
        }
      
      }
    
    
    
    
    }
    
    }
    
   
    
