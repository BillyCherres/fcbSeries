
/*----------------------------------------
Lens Function called in visca.c
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


    //Camera zoom
    unsigned char var_tele[] = {0x81,0x01,0x04,0x07,0x25,0xFF};//zoom in to the middle of the screen fast
    unsigned char var_wide[] = {0x81,0x01,0x04,0x07,0x35,0xFF};//zoom out of the middle of the screen fast
    unsigned char stop_zoom[] = {0x81,0x01,0x04,0x07,0x00,0xFF};
    
    //Camera Dzoom
    unsigned char dZoom_on[] = {0x81,0x01,0x04,0x06,0x02,0xFF};
    unsigned char dZoom_off[] = {0x81,0x01,0x04,0x06,0x03,0xFF};
    unsigned char combine[] = {0x81,0x01,0x04,0x36,0x00,0xFF};
    unsigned char separate[] = {0x81,0x01,0x04,0x36,0x01,0xFF};
    unsigned char var_Dtele[] = {0x81,0x01,0x04,0x06,0x25,0xFF};//zoom in to the middle of the screen fast 
    unsigned char var_Dwide[] = {0x81,0x01,0x04,0x06,0x35,0xFF};//zoom out of the middle of the screen fast
    unsigned char stop_Dzoom[] = {0x81,0x01,0x04,0x06,0x00,0xFF};   

    
    // Focus
    unsigned char stop_focus[] = {0x81,0x01,0x04,0x08,0x00,0xFF}; 
    unsigned char far_focus[] = {0x81,0x01,0x04,0x08,0x02,0xFF}; 
    unsigned char near_focus[] = {0x81,0x01,0x04,0x08,0x03,0xFF}; 
    unsigned char auto_focus[] = {0x81,0x01,0x04,0x38,0x02,0xFF}; 
    unsigned char manual_focus[] = {0x81,0x01,0x04,0x38,0x03,0xFF}; 
    unsigned char onePush[] = {0x81,0x01,0x04,0x18,0x01,0xFF}; 
    unsigned char fullScan[] = {0x81,0x01,0x04,0x18,0x03,0xFF}; 
    
    
    
    //Auto focus 
    unsigned char normalAF[] = {0x81,0x01,0x04,0x57,0x00,0xFF};
    unsigned char intervalAF[] = {0x81,0x01,0x04,0x57,0x01,0xFF};
    unsigned char zoom_triggerAF[] = {0x81,0x01,0x04,0x57,0x02,0xFF};

    //continous commands
    unsigned char contZoomOn[]={0x81,0x01,0x04,0x69,0x02,0xFF};
    unsigned char contZoomOff[]={0x81,0x01,0x04,0x69,0x03,0xFF};
    unsigned char contFocusOn[]={0x81,0x01,0x04,0x16,0x02,0xFF};
    unsigned char contFocusOff[]={0x81,0x01,0x04,0x16,0x03,0xFF};    
    
    
  void lens(int fd){
    struct termios old_term, new_term;
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    char c;
    bool lens;
    lens = true;
    char inputLens[256];
    int optLens;
    int max_fd,ret;
    write(fd,dZoom_on,sizeof(dZoom_on));
    while(lens){

      printf("\n\n\n");
      printf("Please select one of the following options\n");
      printf("1) Zoom\n2) Focus\n0) Quit\n");
     
      if(fgets(inputLens, sizeof(inputLens),stdin) != NULL ){
      	if(sscanf(inputLens, "%d", &optLens)==1){
      	  switch(optLens){
      	  case 1:
      	  printf("Zoom Mode\n");
      	  bool zoom;
      	  zoom = true;
      	  char inputZoom[256];
      	  int optZoom;
      	  while(zoom){
      	  printf("\n\n\n");
      	    printf("Choose one of the following options\n");
      	    printf("1) Combine Zoom\n2) Separate Zoom\n0) Quit\n");
      	    if(fgets(inputZoom, sizeof(inputZoom), stdin) != NULL ){
      	      if(sscanf(inputZoom, "%d", &optZoom) == 1){
      	        switch(optZoom){
      	        case 1:
      	        printf("Combine Mode On\n");
      	      	write(fd,combine ,sizeof(combine));
      	        printf("When clicking Tele or Wide, understand that the zoom will not stop until you press a key on the keyboard or until the max limit of Tele/Wide is reached\n");
      	        bool nZoom;
      	        nZoom = true;
      	        char inputNzoom[256];
      	        int optNzoom;
      	        while(nZoom){
      	        printf("\n\n\n");
      	          printf("Choose one of the following options\n");
      	          printf("1) Tele\n2) Wide\n0) Quit\n");
      	          if(fgets(inputNzoom, sizeof(inputNzoom), stdin) != NULL ){
      	      	    if(sscanf(inputNzoom, "%d", &optNzoom) == 1){
      	      	      switch(optNzoom){
      	      	      case 1:
      	      	      printf("Tele Activated\n");
      	      	      printf("Press any key to stop\n");
      	      	      bool tele;
      	      	      tele = true;
      	      	      
                      unsigned char buf[256] = {0}; 
                      int rdlen = 0;
      	              write(fd,var_tele,sizeof(var_tele));
      	              write(fd,contZoomOn,sizeof(contZoomOn));
      	              
                      //Disable Canonical mode and Echo
                      tcgetattr(STDIN_FILENO, &old_term);
                      new_term = old_term;
                      new_term.c_lflag &= ~(ICANON | ECHO);
                      tcsetattr(STDIN_FILENO,TCSANOW,&new_term);
                      // keyboard stop start
                      fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO,F_GETFL)|O_NONBLOCK); 
      	              
      	              flags |= O_NONBLOCK;
      	              fcntl(STDIN_FILENO, F_SETFL, flags);
      	              
        	      while (tele) {
        		fd_set read_fds;
        		FD_ZERO(&read_fds);
        		FD_SET(STDIN_FILENO, &read_fds);
        		FD_SET(fd, &read_fds);

        		max_fd = (STDIN_FILENO > fd) ? STDIN_FILENO : fd;

        		ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        		if (ret == -1) {
            		  perror("select");
            		  break;
        		}

        		if (FD_ISSET(STDIN_FILENO, &read_fds)  ) {
            		  if (read(STDIN_FILENO, &c, 1) > 0) {
                	    write(fd, stop_zoom, sizeof(stop_zoom));
                	    tele = false;
                	    break;
            		  }
        		}

        		if (FD_ISSET(fd, &read_fds)) {
            		  rdlen = read(fd, buf, sizeof(buf) - 1);
            		  if (rdlen > 0) {
                	    // Debug: Print raw data
                	    
                	/*    printf("Raw data: ");
                	    for (int i = 0; i < rdlen; i++) {
                    	      printf("%X(%d) ", buf[i], i);
                	      }
                	    printf("\n");*/
                	    
                	     // Check if buf[6] is 7
          		    if (buf[6] == 7) {
             		      write(fd, stop_zoom, sizeof(stop_zoom));
             		      tele = false;
               		      break;
            		    }
        		  }
    			}
    	               }
    	              write(fd,contZoomOff,sizeof(contZoomOff));
      	              // reset keyboard settings
      	              tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    	    	      flags &= ~O_NONBLOCK;
             	      fcntl(STDIN_FILENO,F_SETFL,flags);
      	      	      break;
      	      	      case 2:
      	      	      printf("Wide Activated\n");
      	      	      printf("Press any key to stop\n");
      	      	      bool wide;
      	      	      wide = true;
      	      	      unsigned char bufA[256] = {0}; 
                      int rdlenA = 0;
          	      write(fd,contZoomOn,sizeof(contZoomOn));
          	      write(fd,var_wide,sizeof(var_wide));
                      //Disable Canonical mode and Echo
                      tcgetattr(STDIN_FILENO, &old_term);
                      new_term = old_term;
                      new_term.c_lflag &= ~(ICANON | ECHO);
                      tcsetattr(STDIN_FILENO,TCSANOW,&new_term);
                      // keyboard stop start
                      fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO,F_GETFL)|O_NONBLOCK); 
      	              
      	              flags |= O_NONBLOCK;
      	              fcntl(STDIN_FILENO, F_SETFL, flags);

      	                while (wide) {
        		fd_set read_fds;
        		FD_ZERO(&read_fds);
        		FD_SET(STDIN_FILENO, &read_fds);
        		FD_SET(fd, &read_fds);

        		max_fd = (STDIN_FILENO > fd) ? STDIN_FILENO : fd;

        		ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        		if (ret == -1) {
            		  perror("select");
            		  break;
        		}

        		if (FD_ISSET(STDIN_FILENO, &read_fds)  ) {
            		  if (read(STDIN_FILENO, &c, 1) > 0) {
                	    write(fd, stop_zoom, sizeof(stop_zoom));
                	    wide = false;
                	    break;
            		  }
        		}

        		if (FD_ISSET(fd, &read_fds)) {
            		  rdlenA = read(fd, bufA, sizeof(bufA) - 1);
            		  if (rdlenA > 0) {
                	 /*   // Debug: Print raw data
                	    
                	    printf("Raw data: ");
                	    for (int i = 0; i < rdlenA; i++) {
                    	      printf("%X(%d) ", bufA[i], i);
                	      }
                	    printf("\n"); */
                	    
                	     // Check if buf[6] is 7
          		    if (bufA[6] == 0 && bufA[7] == 0 && bufA[8] == 0 && bufA[9] == 0) {
             		      write(fd, stop_zoom, sizeof(stop_zoom));
             		      wide = false;
               		      break;
            		    }
        		  }
    			}
    	               }
      	              
      	               write(fd,contZoomOff,sizeof(contZoomOff));
      	              
      	              
      	              
      	              // reset keyboard settings
      	              tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
    	    	      flags &= ~O_NONBLOCK;
             	      fcntl(STDIN_FILENO,F_SETFL,flags);
      	      	      break;
      	      	      case 0:
      	      	      printf("Quitting\n");
      	      	      nZoom = false;
      	      	      break;
      	      	      }
      	      	    }
      	      	  }
      	        }
      	       
      	        break;
      	        case 2:
      	        printf("Separate Mode");
      	        write(fd,separate ,sizeof(separate));
      	        printf("When clicking Tele or Wide, understand that the zoom will not stop until you press a key on the keyboard or until the max limit of Tele/Wide is reached\n");
      	        
      	        bool dZoom;
      	        dZoom = true;
      	        char inputDzoom[256];
      	        int optDzoom;
      	        while(dZoom){
      	        printf("\n\n\n");
      	        printf("Please choose one of the following options\n");
      	        printf("1) Tele\n2) Wide\n0) Quit\n");
      	          if(fgets(inputDzoom, sizeof(inputDzoom), stdin) != NULL ){
      	      	    if(sscanf(inputDzoom, "%d", &optDzoom) == 1){
      	      	    switch(optDzoom){
      	      	    case 1:

      	      	    printf("D-Tele Activated\n");
      	      	    printf("Press any key to stop\n");
      	      	    bool Dtele;
      	      	    Dtele = true;
      	            unsigned char bufB[256] = {0}; 
                    int rdlenB = 0;
          	    write(fd,contZoomOn,sizeof(contZoomOn));
        	    write(fd,var_Dtele,sizeof(var_Dtele));
        	    
                    //Disable Canonical mode and Echo
                    tcgetattr(STDIN_FILENO, &old_term);
                    new_term = old_term;
                    new_term.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO,TCSANOW,&new_term);
                    // keyboard stop start
                    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO,F_GETFL)|O_NONBLOCK); 
      	            
      	            flags |= O_NONBLOCK;
      	            fcntl(STDIN_FILENO, F_SETFL, flags);
      	            
      	            
                    while (Dtele) {
        		fd_set read_fds;
        		FD_ZERO(&read_fds);
        		FD_SET(STDIN_FILENO, &read_fds);
        		FD_SET(fd, &read_fds);

        		max_fd = (STDIN_FILENO > fd) ? STDIN_FILENO : fd;

        		ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        		if (ret == -1) {
            		  perror("select");
            		  break;
        		}

        		if (FD_ISSET(STDIN_FILENO, &read_fds)  ) {
            		  if (read(STDIN_FILENO, &c, 1) > 0) {
                	    write(fd, stop_Dzoom, sizeof(stop_Dzoom));
                	    Dtele = false;
                	    break;
            		  }
        		}

        		if (FD_ISSET(fd, &read_fds)) {
            		  rdlenB = read(fd, bufB, sizeof(bufB) - 1);
            		  if (rdlenB > 0) {
                	    // Debug: Print raw data
                	    
                	 /*   printf("Raw data: ");
                	    for (int i = 0; i < rdlenB; i++) {
                    	      printf("%X(%d) ", bufB[i], i);
                	      }
                	    printf("\n"); */ 
                	    
                	     // Check if buf[6] is 7
          		    if (bufB[5] == 8) {
             		      write(fd, stop_Dzoom, sizeof(stop_Dzoom));
             		      Dtele = false;
               		      break;
            		    }
        		  }
    			 }
    	               }
      	            
      	            
      	             write(fd,contZoomOff,sizeof(contZoomOff));

      	            
      	            // reset keyboard settings
                    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  	    	    flags &= ~O_NONBLOCK;
             	    fcntl(STDIN_FILENO,F_SETFL,flags);
      	      	    break;

      	      	    case 2:
      	      	    
      	      	    printf("D-Wide Activated\n");
      	      	    printf("Press any key to stop\n");
      	      	    bool Dwide;
      	      	    Dwide = true;
      	      	    unsigned char bufC[256] = {0}; 
                    int rdlenC = 0;
      	            write(fd,var_Dwide,sizeof(var_Dwide));
                    write(fd,contZoomOn,sizeof(contZoomOn));
                    //Disable Canonical mode and Echo
                    tcgetattr(STDIN_FILENO, &old_term);
                    new_term = old_term;
                    new_term.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO,TCSANOW,&new_term);
                    // keyboard stop start
                    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO,F_GETFL)|O_NONBLOCK); 
      	            
      	            flags |= O_NONBLOCK;
      	            fcntl(STDIN_FILENO, F_SETFL, flags);
      	            
      	            
      	            
      	            
      	            
      	            
      	            
                    while (Dwide) {
        		fd_set read_fds;
        		FD_ZERO(&read_fds);
        		FD_SET(STDIN_FILENO, &read_fds);
        		FD_SET(fd, &read_fds);

        		max_fd = (STDIN_FILENO > fd) ? STDIN_FILENO : fd;

        		ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        		if (ret == -1) {
            		  perror("select");
            		  break;
        		}

        		if (FD_ISSET(STDIN_FILENO, &read_fds)  ) {
            		  if (read(STDIN_FILENO, &c, 1) > 0) {
                	    write(fd, stop_Dzoom, sizeof(stop_Dzoom));
                	    Dwide = false;
                	    break;
            		  }
        		}

        		if (FD_ISSET(fd, &read_fds)) {
            		  rdlenC = read(fd, bufC, sizeof(bufC) - 1);
            		  if (rdlenC > 0) {
                	    // Debug: Print raw data
                	    
                	   
                	    
                	     // Check if buf[6] is 7
          		    if (bufC[6] == 0 && bufC[7] == 0 && bufC[8] == 0 && bufC[9] == 0) {
             		      write(fd, stop_Dzoom, sizeof(stop_Dzoom));
             		      Dwide = false;
               		      break;
            		     }
        		   }
    			 }
    	               }
      	            
      	            
      	            
      	            
      	            
      	            // reset keyboard settings
                    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  	    	    flags &= ~O_NONBLOCK;
             	    fcntl(STDIN_FILENO,F_SETFL,flags);
      	      	    write(fd,contZoomOff,sizeof(contZoomOff));
      	      	    break;
      	      	    case 0:
      	      	    printf("Quitting\n");
      	      	    dZoom = false;
      	      	    write(fd,dZoom_off,sizeof(dZoom_off));
      	      	    break;  
      	      	    }
      	      	  }
      	        }
      	      } 
      	        break;
      	        case 0:
      	        printf("Quitting\n");
      	        zoom = false;
      	        break;
      	        }
      	      }
      	    }
      	  }
      	  
      	  
      	  break;
      	  case 2:
      	  printf("Focus Mode\n");
      	  bool focus;
      	  focus = true;
      	  char inputFocus[256];
      	  int optFocus;
      	  while(focus){
      	  printf("\n\n\n");
      	  printf("Please choose one of the following options\n");
      	  printf("1) Auto Focus\n2) Manual Focus\n0) Quit\n");
      	  
      	  if(fgets(inputFocus, sizeof(inputFocus), stdin) != NULL ){
      	    if(sscanf(inputFocus, "%d", &optFocus) == 1){
      	      switch(optFocus){
      	      case 1:
      	      write(fd,auto_focus, sizeof(auto_focus));// auto focus on
      	      printf("Auto Mode\n");
      	      bool autoM = true;
      	      char inputAuto[256];
      	      int optAuto;
      	      while(autoM){
      	        printf("\n\n\n");
      	        printf("Please choose one of the following options\n");
      	        printf("1) Normal AF\n2) Interval AF\n3) Zoom Trigger AF\n0) Quit\n");
      	        if(fgets(inputAuto,sizeof(inputAuto), stdin) != NULL){
      	          if (sscanf(inputAuto, "%d", &optAuto) == 1){
      	            switch(optAuto){
      	            case 1:
      	            printf("Normal AF\n");
      	            write(fd,normalAF ,sizeof(normalAF));
      	            break;
      	            case 2:
      	            printf("Interval AF\n");
      	            write(fd, intervalAF, sizeof(intervalAF));
      	            break;
      	            case 3:
      	            printf("Zoom Trigger AF\n");
      	            write(fd, zoom_triggerAF, sizeof(zoom_triggerAF));
      	            break;
      	            case 0:
      	            printf("Quitting\n");
      	            autoM = false;
      	            break;
      	            }
      	          }
      	        }
      	      }
      	      
      	      break;
      	      case 2:
      	      printf("Manual Mode\n");
      	      write(fd,manual_focus,sizeof(manual_focus));// manual focus on
      	      bool manual;
      	      manual = true;
      	      char inputMan[256];
      	      int optMan;
      	      while(manual){
      	        printf("\n\n\n");
      	        printf("Please choose one of the following options\n");
      	        printf("1) One Push\n2) Full Scan\n3) Far\n4) Near\n0) Quit\n");
      	        if(fgets(inputMan, sizeof(inputMan), stdin) != NULL ){
      	          if(sscanf(inputMan, "%d", &optMan) == 1){
      	            switch(optMan){
      	            case 1:
      	            printf("One Push\n");
      	            write(fd,onePush, sizeof(onePush));
      	            break;
      	            case 2:
      	            printf("Full Scan\n");
      	            write(fd,fullScan, sizeof(fullScan));
      	            break;
      	            case 3:
      	            printf("Far Focus\n");
      	            printf("Press any key to stop focussing\n");
      	            bool far;
      	            far = true;
      	            unsigned char bufD[256] = {0}; 
                    int rdlenD = 0;
                    write(fd,contFocusOn,sizeof(contFocusOn));
        	    write(fd,far_focus,sizeof(far_focus));
                    //Disable Canonical mode and Echo
                    tcgetattr(STDIN_FILENO, &old_term);
                    new_term = old_term;
                    new_term.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO,TCSANOW,&new_term);
                    // keyboard stop start
                    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO,F_GETFL)|O_NONBLOCK); 
      	            
      	            flags |= O_NONBLOCK;
      	            fcntl(STDIN_FILENO, F_SETFL, flags);
      	            
      	            
                    while (far) {
        		fd_set read_fds;
        		FD_ZERO(&read_fds);
        		FD_SET(STDIN_FILENO, &read_fds);
        		FD_SET(fd, &read_fds);

        		max_fd = (STDIN_FILENO > fd) ? STDIN_FILENO : fd;

        		ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        		if (ret == -1) {
            		  perror("select");
            		  break;
        		}

        		if (FD_ISSET(STDIN_FILENO, &read_fds)  ) {
            		  if (read(STDIN_FILENO, &c, 1) > 0) {
                	    write(fd, stop_focus, sizeof(stop_focus));
                	    far = false;
                	    break;
            		  }
        		}

        		if (FD_ISSET(fd, &read_fds)) {
            		  rdlenD = read(fd, bufD, sizeof(bufD) - 1);
            		  if (rdlenD > 0) { 
                	    // Debug: Print raw data
                	    
                	/*    printf("Raw data: ");
                	    for (int i = 0; i < rdlenD; i++) {
                    	      printf("%X(%d) ", bufD[i], i);
                	      }
                	    printf("\n"); */
                	    
                	     // Check if buf[6] is 7
          		    if (bufD[6] == 1 && bufD[7] == 0 && bufD[8] == 0 && bufD[9] == 0) {
             		      write(fd, stop_focus, sizeof(stop_focus));
             		      far = false;
               		      break;
            		     }
        		   }
    			 }
    	               }
      	            
      	            
      	            
      	            
      	            
      	            
      	            // reset keyboard settings
                    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  	    	    flags &= ~O_NONBLOCK;
             	    fcntl(STDIN_FILENO,F_SETFL,flags);
      	      	    write(fd,contFocusOff,sizeof(contZoomOff));
      	            break;
      	            case 4:
      	            printf("Near Focus\n");
      	            printf("Press any key to stop the focus\n");
      	            
      	            bool near;
      	            near = true;
      	            unsigned char bufE[256] = {0}; 
                    int rdlenE = 0;
                    write(fd,contFocusOn,sizeof(contFocusOn));
                    write(fd,near_focus,sizeof(near_focus));
                    //Disable Canonical mode and Echo
                    tcgetattr(STDIN_FILENO, &old_term);
                    new_term = old_term;
                    new_term.c_lflag &= ~(ICANON | ECHO);
                    tcsetattr(STDIN_FILENO,TCSANOW,&new_term);
                    // keyboard stop start
                    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO,F_GETFL)|O_NONBLOCK); 
      	            
      	            flags |= O_NONBLOCK;
      	            fcntl(STDIN_FILENO, F_SETFL, flags);
      	            
      	            
                    while (near) {
        		fd_set read_fds;
        		FD_ZERO(&read_fds);
        		FD_SET(STDIN_FILENO, &read_fds);
        		FD_SET(fd, &read_fds);

        		max_fd = (STDIN_FILENO > fd) ? STDIN_FILENO : fd;

        		ret = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        		if (ret == -1) {
            		  perror("select");
            		  break;
        		}

        		if (FD_ISSET(STDIN_FILENO, &read_fds)  ) {
            		  if (read(STDIN_FILENO, &c, 1) > 0) {
                	    write(fd, stop_focus, sizeof(stop_focus));
                	    near = false;
                	    break;
            		  }
        		}

        		if (FD_ISSET(fd, &read_fds)) {
            		  rdlenE = read(fd, bufE, sizeof(bufE) - 1);
            		  if (rdlenE > 0) {
                	    // Debug: Print raw data
                	    
                	 /*   printf("Raw data: ");
                	    for (int i = 0; i < rdlenE; i++) {
                    	      printf("%X(%d) ", bufE[i], i);
                	      }
                	    printf("\n"); */ 
                	    
                	     // Check if buf[6] is 7
          		    if (bufE[7] == 0 && bufE[8] == 0 && bufE[9] == 0) {
             		      write(fd, stop_focus, sizeof(stop_focus));
             		      near = false;
               		      break;
            		     }
        		   }
    			 }
    	               }
      	            // reset keyboard settings
                    tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  	    	    flags &= ~O_NONBLOCK;
             	    fcntl(STDIN_FILENO,F_SETFL,flags);
             	    write(fd,contFocusOff,sizeof(contFocusOff));
      	            break;
      	            case 0:
      	            printf("Quitting\n");
      	            manual = false;
      	            break;
      	            }
      	          }
      	        }
      	      }
      	            	      
      	      break;
      	      case 0:
      	      printf("Quitting\n");
      	      focus = false;
      	      break;
      	      }
      	    
      	    }
      	  }
      	  
      	  
      	  }
      	  
      	  break;
      	  case 0:
      	  printf("Quitting");
      	  lens = false;
      	  break;
	  }
	}
      }
    }
  }
	
   
