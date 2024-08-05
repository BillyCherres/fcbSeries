
/*----------------------------------------
Motion Detect Function called in visca.c
------------------------------------------*/

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

#include "visca.h"

// Motion detection and frames
unsigned char motion_on[] = {0x81, 0x01, 0x04, 0x1B, 0x02, 0xFF};
unsigned char motion_off[] = {0x81, 0x01, 0x04, 0x1B, 0x03, 0xFF};
unsigned char function_set_displayON[] = {0x81, 0x01, 0x04, 0x1C, 0x01, 0x0F,
                                          0x00, 0x1,  0x00, 0x01, 0xFF};
unsigned char function_set_displayOFF[] = {0x81, 0x01, 0x04, 0x1C, 0x00, 0x0F,
                                           0x00, 0x1,  0x00, 0x01, 0xFF};
unsigned char window_set[4][10] = {
    {0x81, 0x01, 0x04, 0x1D, 0x00, 0x00, 0x00, 0x08, 0x06, 0xFF},
    {0x81, 0x01, 0x04, 0x1D, 0x01, 0x08, 0x00, 0x10, 0x06, 0xFF},
    {0x81, 0x01, 0x04, 0x1D, 0x02, 0x00, 0x06, 0x08, 0x0C, 0xFF},
    {0x81, 0x01, 0x04, 0x1D, 0x03, 0x08, 0x06, 0x10, 0x0C, 0xFF}};

void motionDetect(int fd) {
  // motion detection option which allows for spot focus to activate on a
  // specific frame
  write(fd, motion_on, sizeof(motion_on));
  write(fd, function_set_displayON, sizeof(function_set_displayON));
  write(fd, window_set, sizeof(window_set));

  printf("Motion detection turned on\n");
  bool stop;
  stop = false;
  int frame_detect = 0;
  //    char inp[100];
  //   int answer;
  struct termios old_term, new_term;
  char c;

  // Disable Canonical mode and Echo
  tcgetattr(STDIN_FILENO, &old_term);
  new_term = old_term;
  new_term.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
  // keyboard stop start
  fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  flags |= O_NONBLOCK;
  fcntl(STDIN_FILENO, F_SETFL, flags);

  while (!stop) {
    write(fd, motion_on, sizeof(motion_on));
    unsigned char buf[256]; // buffer to store data from serial port
    int rdlen = 0;          // Variable to store the length of the data read

    rdlen = read(fd, buf, sizeof(buf) - 1);

    if (rdlen > 0) {          // if data was successfully read
      unsigned char *p = buf; // create a pointer to traverse buffer

      // traverse through the buffer
      for (p = buf; rdlen-- > 0; p++) {
        if (*p == 0x1B) {
          frame_detect = *(p + 1);
          if (frame_detect == 1 || frame_detect == 2) {
            frame_detect--;
          } else if (frame_detect == 4) {
            frame_detect = 2;
          } else if (frame_detect == 8) {
            frame_detect = 3;
          } else {
            printf("Motion detected in multiple screens\n");
            write(fd, function_set_displayOFF, sizeof(function_set_displayOFF));

            break;
          }

          printf("Motion detected in frame %d\n", frame_detect);
          write(fd, motion_off, sizeof(motion_off));
          write(fd, function_set_displayOFF, sizeof(function_set_displayOFF));

          write(fd, spot_focus_on, sizeof(spot_focus_on));
          write(fd, spot_focus_displayON, sizeof(spot_focus_displayON));
          printf("Focusing on frame %d\n", frame_detect);

          if (frame_detect == 0) {
            write(fd, spot_focus_topL, sizeof(spot_focus_topL));
          } else if (frame_detect == 1) {
            write(fd, spot_focus_topR, sizeof(spot_focus_topR));
          } else if (frame_detect == 2) {
            write(fd, spot_focus_bottomL, sizeof(spot_focus_bottomL));
          } else if (frame_detect == 3) {
            write(fd, spot_focus_bottomR, sizeof(spot_focus_bottomR));
          }
          // offer a focus option for frame of choice
          //  printf("Would you like to focus on frame %d?. Enter 1 for yes and
          //  2 for no\n",frame_detect);
          /*    if(fgets(inp, sizeof(inp), stdin) != NULL){
                if(sscanf(inp, "%d", &answer) == 1){
                  if (answer != 1){
                    break;
                  }
                  write(fd,spot_focus_on,sizeof(spot_focus_on));
                  write(fd,spot_focus_displayON,sizeof(spot_focus_displayON));

                  printf("Focusing on frame %d\n", frame_detect);
                  if (frame_detect == 0){
                  write(fd,spot_focus_topL,sizeof(spot_focus_topL));
                  }else if (frame_detect == 1){
                  write(fd,spot_focus_topR,sizeof(spot_focus_topR));
                  }else if(frame_detect == 2){
                  write(fd,spot_focus_bottomL,sizeof(spot_focus_bottomL));
                  }else if(frame_detect == 3){
                  write(fd,spot_focus_bottomR,sizeof(spot_focus_bottomR));
                  }

                }
              }  */

          // break
        }
      }
    } else if (rdlen < 0) {
      printf("Error from read: %d: %s\n", rdlen, strerror(errno));
    }

    if (read(STDIN_FILENO, &c, 1) > 0) {
      stop = true;
      break;
    }
  }

  // turn off motion detection
  write(fd, motion_off, sizeof(motion_off));
  write(fd, spot_focus_off, sizeof(spot_focus_off));
  write(fd, spot_focus_displayOFF, sizeof(spot_focus_displayOFF));
  printf("Motion detection turned off\n");
  tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
  flags &= ~O_NONBLOCK;
  fcntl(STDIN_FILENO, F_SETFL, flags);
}

