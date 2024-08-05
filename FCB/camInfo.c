

/*--------------------------------------------
Camera Information Function Called in visca.c
----------------------------------------------*/

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



    unsigned char cam_versionINQ[] = {0x81,0x09,0x00,0x02,0xFF};// cam version
    unsigned char baud_rate[] = {0x81,0x09,0x04,0x24,0x00,0xFF}; // cam baud rate

    //function
    // Buffer to store data from serial port
    unsigned char bufR[256] = {0}; 
    int rdlenR = 0; // Variable to store the length of the data read
    unsigned char bufZ[256] = {0};
    int rdlenZ = 0;
    char model[5] = {0}; 
    char rom[5] = {0};   
    char socket[3] = {0}; 
    char baudrate[10] = {0}; 
    
    
  void camera_information(int fd){

    
    //  inquiry cam version
    write(fd, cam_versionINQ, sizeof(cam_versionINQ));
    // read cam version
    rdlenR = read(fd, bufR, sizeof(bufR) - 1);
   
    
    //send register command baud rate
    write(fd,baud_rate,sizeof(baud_rate));
    
    // read baud rate
    rdlenZ = read(fd,bufZ,sizeof(bufZ));
  
   // printf("Capturing response\n");
    if (rdlenZ>0){
      /*  printf("Raw data: ");
        for (int i = 0; i < rdlenZ; i++) {
            printf("%02X ", bufZ[i]);
        }*/
        printf("\n");
       // 9600
       if (bufZ[7]== 0){
         baudrate[0] = '9';
         baudrate[1] = '6';
         baudrate[2] = '0';
         baudrate[3] = '0';
         baudrate[4] = '\0';
         //19200
       }else if(bufZ[7]==1){
         baudrate[0] = '1';
         baudrate[1] = '9';
         baudrate[2] = '2';
         baudrate[3] = '0';
         baudrate[4] = '0';
         baudrate[5] = '\0';
         //38400
       }else if(bufZ[7]==2){
         baudrate[0] = '3';
         baudrate[1] = '8';
         baudrate[2] = '4';
         baudrate[3] = '0';
         baudrate[4] = '0';
         baudrate[5] = '\0';
         //115200
       }else if(bufZ[7]==3){
         baudrate[0] = '1';
         baudrate[1] = '1';
         baudrate[2] = '5';
         baudrate[3] = '2';
         baudrate[4] = '0';
         baudrate[5] = '0';
         baudrate[6] = '\0';
       } 
    }
    
    if (rdlenR > 0) { // if response was captured
  
     /*   // Debug: Print raw data
        printf("Raw data: ");
        for (int i = 0; i < rdlenR; i++) {
            printf("%02X ", bufR[i]);
        }
        printf("\n");*/
        
        // Get model code, ROM, and Socket
        if (rdlenR >= 10 && bufR[1] == 0x50) { // Check the reply header
            // Extracting model code
            snprintf(model, sizeof(model), "%02X%02X", bufR[4], bufR[5]);
            
            // Extracting ROM version
            snprintf(rom, sizeof(rom), "%02X%02X", bufR[6], bufR[7]);
            
            // Extracting socket number
            snprintf(socket, sizeof(socket), "%02X", bufR[8]);
        } 
    } else {
        perror("Error reading from file descriptor or no data received");
    }

    	// Printing Camera Information
	printf("\n\n\n");
        printf("Camera Information:\n-------------------\n");
        
        if (strcmp(model, "070F") == 0) {
        printf("Model: EW9500H\n");
        } else if(strcmp(model, "0710") == 0){
        printf("Model: ER9500\n");
        } else if(strcmp(model, "070E") == 0){
        printf("Model: EV9500L\n");
        }else if(strcmp(model, "0711") == 0){
        printf("Model: EV9520L\n");
        }
        
        printf("ROM Version: %s\n", rom);
        printf("Socket Number: %s\n", socket);
        printf("Baud Rate: %s\n", baudrate);
      
    
   
  
  }
