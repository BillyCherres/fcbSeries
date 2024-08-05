
/*----------------------------------------
Extend Function called in visca.c
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

#define BUFFER_SIZE 256

// EX Comp Extend
unsigned char exComp_on[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x01, 0xFF};
unsigned char exComp_off[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x00, 0xFF};
unsigned char exComp_reset[] = {0x81, 0x01, 0x04, 0x1F, 0x0E,
                                0x00, 0x00, 0xFF}; // 8x 01 04 1F 0E 00 00 FF
unsigned char exComp_inc[] = {0x81, 0x01, 0x04, 0x1F, 0x0E, 0x02, 0x02, 0xFF};
unsigned char exComp_dec[] = {0x81, 0x01, 0x04, 0x1F, 0x0E, 0x03, 0x02, 0xFF};
unsigned char exCompINQ[] = {0x81, 0x09, 0x04,
                             0x1F, 0x4E, 0xFF}; // 8x 09 04 1F 4E FF

// Aperture extend

unsigned char exApe_on[] = {0x81, 0x01, 0x04, 0x24,
                            0x5F, 0x00, 0x02, 0xFF}; // 81 01 04 24 5F 00 02 FF
unsigned char exApe_off[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x00, 0xFF};
unsigned char exApe_reset[] = {0x81, 0x01, 0x04, 0x1F, 0x02,
                               0x00, 0x00, 0xFF}; // 8x 01 04 1F 02 00 00 FF
unsigned char exApe_inc[] = {0x81, 0x01, 0x04, 0x1F,
                             0x02, 0x02, 0x02, 0xFF}; // 8x 01 04 1F 02 02 pp FF
unsigned char exApe_dec[] = {0x81, 0x01, 0x04, 0x1F,
                             0x02, 0x03, 0x02, 0xFF}; // 8x 01 04 1F 02 03 pp FF
unsigned char exApeINQ[] = {0x81, 0x09, 0x04,
                            0x1F, 0x42, 0xFF}; // 8x 09 04 1F 42 FF

// Auto ICR Extend
unsigned char exIcr_on[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x08, 0xFF};
unsigned char exIcr_off[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x00, 0xFF};
unsigned char exIcr_thresh[] = {
    0x81, 0x01, 0x04, 0x1F, 0x21,
    0x00, 0x00, 0x00, 0x00, 0xFF}; // 8x 01 04 1F 21 00 00 0p 0q FF
unsigned char exIcr_level[] = {
    0x81, 0x01, 0x04, 0x1F, 0x21,
    0x01, 0x00, 0x00, 0x00, 0xFF}; // 8x 01 04 1F 21 01 00 0r 0s FF
unsigned char exIcr_threshINQ[] = {0x81, 0x09, 0x04, 0x1F,
                                   0x21, 0x00, 0xFF}; // 8x 09 04 1F 21 00 FF
unsigned char exIcr_levelINQ[] = {0x81, 0x09, 0x04, 0x1F,
                                  0x21, 0x01, 0xFF}; // 8x 09 04 1F 21 01 FF

// color Gain/Hue extend
unsigned char exColor_on[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x04, 0xFF};
unsigned char exColor_off[] = {0x81, 0x01, 0x04, 0x24, 0x5F, 0x00, 0x00, 0xFF};
unsigned char exColor_gain[] = {
    0x81, 0x01, 0x04, 0x1F, 0x49,
    0x00, 0x00, 0x00, 0x00, 0xFF}; // 8x 01 04 1F 49 00 00 0p 0q FF
unsigned char exColor_hue[] = {0x81, 0x01, 0x04, 0x1F, 0x4F,
                               0x00, 0x00, 0x00, 0x00, 0xFF};
unsigned char exColor_gainINQ[] = {0x81, 0x09, 0x04, 0x1F,
                                   0x49, 0x00, 0xFF}; // 8x 09 04 1F 49 00 FF
unsigned char exColor_hueINQ[] = {0x81, 0x09, 0x04, 0x1F,
                                  0x4F, 0x00, 0xFF}; // 8x 09 04 1F 4F 00 FF

// private functions

// Extend Comp
void compF(int fd) {
  bool compEX;
  compEX = true;

  char inpCompex[BUFFER_SIZE];
  int optCompex;

  unsigned char bufC[BUFFER_SIZE] = {0};
  //   int rdlenC = 0;

  while (compEX) {

    write(fd, exComp_on, sizeof(exComp_on));
    //		 rdlenC = read(fd, bufC, sizeof(bufC)); // Read response
    read(fd, bufC, sizeof(bufC));
    memset(bufC, 0, sizeof(bufC)); // reset buffer i
    write(fd, exCompINQ, sizeof(exCompINQ));
    //	 rdlenC = read(fd, bufC, sizeof(bufC)); // Read response
    read(fd, bufC, sizeof(bufC));
    printf("Current Comp Status: %X%X\n", bufC[4], bufC[5]);
    memset(bufC, 0, sizeof(bufC)); // reset buffer i

    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) On\n2) Off\n0) Quit\n");
    if (fgets(inpCompex, sizeof(inpCompex), stdin) != NULL) {
      if (sscanf(inpCompex, "%d", &optCompex) == 1) {
        switch (optCompex) {
        case 1:
          printf("On\n");

          write(fd, exComp_on, sizeof(exComp_on));
          write(fd, exComp_inc, sizeof(exComp_inc));
          //		rdlenC = read(fd, bufC, sizeof(bufC)); // Read response
          read(fd, bufC, sizeof(bufC));
          memset(bufC, 0, sizeof(bufC)); // reset buffer i

          bool compOn;
          compOn = true;

          char inpCompon[BUFFER_SIZE];
          int optCompon;
          while (compOn) {

            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("1) Increment\n2) Decrement\n3) Reset\n0) Quit\n");
            if (fgets(inpCompon, sizeof(inpCompon), stdin) != NULL) {
              if (sscanf(inpCompon, "%d", &optCompon) == 1) {
                switch (optCompon) {
                case 1:

                  printf("Increment\n");

                  write(fd, exCompINQ, sizeof(exCompINQ));
                  //		rdlenC = read(fd, bufC, sizeof(bufC)); // Read
                  //response
                  read(fd, bufC, sizeof(bufC));

                  if (bufC[4] == 0x0F && bufC[5] == 0x0F) {
                    printf("Comp is at max capacity\n");

                  } else {
                    write(fd, exComp_inc, sizeof(exComp_inc));
                    //			rdlenC = read(fd, bufC, sizeof(bufC)); // Read
                    //response
                    read(fd, bufC, sizeof(bufC));
                    memset(bufC, 0, sizeof(bufC)); // reset buffer i
                  }
                  write(fd, exCompINQ, sizeof(exCompINQ));
                  //			rdlenC = read(fd, bufC, sizeof(bufC)); // Read
                  //response
                  read(fd, bufC, sizeof(bufC));
                  printf("Comp Incremented to: %X%X\n", bufC[4], bufC[5]);
                  memset(bufC, 0, sizeof(bufC)); // reset buffer i

                  break;
                case 2:
                  printf("Decrement\n");
                  write(fd, exCompINQ, sizeof(exCompINQ));
                  //	rdlenC = read(fd, bufC, sizeof(bufC)); // Read response
                  read(fd, bufC, sizeof(bufC));
                  if (bufC[4] == 0x00 && bufC[5] == 0x00) {
                    printf("Comp is at max capacity\n");

                  } else {
                    write(fd, exComp_dec, sizeof(exComp_dec));
                    //				rdlenC = read(fd, bufC, sizeof(bufC)); // Read
                    //response
                    read(fd, bufC, sizeof(bufC));
                    memset(bufC, 0, sizeof(bufC)); // reset buffer i
                  }
                  write(fd, exCompINQ, sizeof(exCompINQ));
                  //			rdlenC = read(fd, bufC, sizeof(bufC)); // Read
                  //response
                  read(fd, bufC, sizeof(bufC));
                  printf("Comp Decremented to: %X%X\n", bufC[4], bufC[5]);
                  memset(bufC, 0, sizeof(bufC)); // reset buffer i

                  break;
                case 3:
                  printf("Reset\n");
                  write(fd, exComp_reset, sizeof(exComp_reset));
                  // rdlenC = read(fd, bufC, sizeof(bufC)); // Read response
                  read(fd, bufC, sizeof(bufC));
                  memset(bufC, 0, sizeof(bufC)); // reset buffer i
                  write(fd, exCompINQ, sizeof(exCompINQ));
                  // rdlenC = read(fd, bufC, sizeof(bufC)); // Read response
                  read(fd, bufC, sizeof(bufC));
                  printf("Comp Reset to: %X%X\n", bufC[4], bufC[5]);
                  memset(bufC, 0, sizeof(bufC)); // reset buffer i
                  break;
                case 0:
                  printf("Quitting\n");
                  write(fd, exComp_off, sizeof(exComp_off));
                  compOn = false;
                  break;
                }
              }
            }
          }
          break;
        case 2:
          printf("Off\n");
          write(fd, exComp_off, sizeof(exComp_off));

          break;
        case 0:
          printf("Quitting\n");
          compEX = false;

          break;
        }
      }
    }
  }
}

// Extend Aperture
void apeExF(int fd) {
  bool apeEX;
  apeEX = true;
  char inpApeex[BUFFER_SIZE];
  int optApeex;

  unsigned char bufA[BUFFER_SIZE] = {0};
  // int rdlenA = 0;

  while (apeEX) {

    write(fd, exApe_on, sizeof(exApe_on));
    // rdlenA = read(fd, bufA, sizeof(bufA)); // Read response
    read(fd, bufA, sizeof(bufA));
    memset(bufA, 0, sizeof(bufA)); // reset buffer i
    write(fd, exApeINQ, sizeof(exApeINQ));
    //	 rdlenA = read(fd, bufA, sizeof(bufA)); // Read response
    read(fd, bufA, sizeof(bufA));
    printf("Current Aperture Status: %X%X\n", bufA[4], bufA[5]);
    memset(bufA, 0, sizeof(bufA)); // reset buffer i

    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) On\n2) Off\n0) Quit\n");
    if (fgets(inpApeex, sizeof(inpApeex), stdin) != NULL) {
      if (sscanf(inpApeex, "%d", &optApeex) == 1) {
        switch (optApeex) {
        case 1:
          printf("On\n");
          write(fd, exApe_on, sizeof(exApe_on));
          bool apeOn;
          apeOn = true;
          char inpApeon[BUFFER_SIZE];
          int optApeon;
          while (apeOn) {

            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("1) Increment\n2) Decrement\n3) Reset\n0) Quit\n");
            if (fgets(inpApeon, sizeof(inpApeon), stdin) != NULL) {
              if (sscanf(inpApeon, "%d", &optApeon) == 1) {
                switch (optApeon) {
                case 1:

                  printf("Increment\n");

                  write(fd, exApeINQ, sizeof(exApeINQ));
                  //		rdlenA = read(fd, bufA, sizeof(bufA)); // Read
                  //response
                  read(fd, bufA, sizeof(bufA));

                  if (bufA[4] == 0x0F && bufA[5] == 0x0F) {
                    printf("Aperture is at max capacity\n");

                  } else {
                    write(fd, exApe_inc, sizeof(exApe_inc));
                    //			rdlenA = read(fd, bufA, sizeof(bufA)); // Read
                    //response
                    read(fd, bufA, sizeof(bufA));

                    memset(bufA, 0, sizeof(bufA)); // reset buffer i
                  }
                  write(fd, exApeINQ, sizeof(exApeINQ));
                  //		rdlenA = read(fd, bufA, sizeof(bufA)); // Read
                  //response
                  read(fd, bufA, sizeof(bufA));
                  printf("Aperture Incremented to: %X%X\n", bufA[4], bufA[5]);
                  memset(bufA, 0, sizeof(bufA)); // reset buffer i

                  break;
                case 2:

                  printf("Decrement\n");

                  write(fd, exApeINQ, sizeof(exApeINQ));
                  //						rdlenA = read(fd, bufA,
                  //sizeof(bufA)); // Read response
                  read(fd, bufA, sizeof(bufA));

                  if (bufA[4] == 0x00 && bufA[5] == 0x00) {
                    printf("Aperture is at max capacity\n");

                  } else {
                    write(fd, exApe_dec, sizeof(exApe_dec));
                    //	rdlenA = read(fd, bufA, sizeof(bufA)); // Read response
                    read(fd, bufA, sizeof(bufA));
                    memset(bufA, 0, sizeof(bufA)); // reset buffer i
                  }
                  write(fd, exApeINQ, sizeof(exApeINQ));
                  //				rdlenA = read(fd, bufA, sizeof(bufA)); //
                  //Read response
                  read(fd, bufA, sizeof(bufA));
                  printf("Aperture Incremented to: %X%X\n", bufA[4], bufA[5]);
                  memset(bufA, 0, sizeof(bufA)); // reset buffer i

                  /*

                                  // Debug: Print raw data
if(rdlenA > 0){
printf("Raw data DECREMENTED: ");
  for (int i = 0; i < rdlenA; i++) {
printf("%X(%d) ", bufA[i], i);
}
printf("\n");
}
*/

                  break;
                case 3:
                  printf("Reset\n");
                  write(fd, exApe_reset, sizeof(exApe_reset));
                  break;
                case 0:
                  printf("Quitting\n");
                  write(fd, exApe_off, sizeof(exApe_off));
                  apeOn = false;
                  break;
                }
              }
            }
          }
          break;
        case 2:
          printf("Off\n");
          write(fd, exApe_off, sizeof(exApe_off));

          break;
        case 0:
          printf("Quitting\n");
          apeEX = false;

          break;
        }
      }
    }
  }
}

// Eztend ICR
void icrExF(int fd) {
  bool icrEX;
  icrEX = true;
  char inpIcrex[BUFFER_SIZE];
  int optIcrex;
  unsigned char bufT[BUFFER_SIZE] = {0};
  //   int rdlenT = 0;
  unsigned char bufL[BUFFER_SIZE] = {0};
  // int rdlenL = 0;

  while (icrEX) {
    // initialize
    write(fd, exIcr_on, sizeof(exIcr_on));
    // rdlenT = read(fd, bufT, sizeof(bufT)); // Read response
    read(fd, bufT, sizeof(bufT));  // Read response
    memset(bufT, 0, sizeof(bufT)); // reset buffer i

    printf("\n\n\n");
    // prepare threshold buffer
    memset(bufT, 0, sizeof(bufT)); // reset buffer i
    write(fd, exIcr_threshINQ, sizeof(exIcr_threshINQ));
    // rdlenT = read(fd, bufT, sizeof(bufT)); // Read response
    read(fd, bufT, sizeof(bufT)); // Read response
    // prepare level buffer
    memset(bufL, 0, sizeof(bufL)); // reset buffer i
    write(fd, exIcr_levelINQ, sizeof(exIcr_levelINQ));
    // rdlenL = read(fd, bufL, sizeof(bufL)); // Read response
    read(fd, bufL, sizeof(bufL)); // Read response

    printf("Current Threshold: %X%X \n", bufT[4], bufT[5]);
    printf("Current Level: %X%X \n", bufL[4], bufL[5]);

    printf("Please choose one of the following options\n");
    printf("1) Threshold\n2) Level \n0) Quit\n");

    if (fgets(inpIcrex, sizeof(inpIcrex), stdin) != NULL) {
      if (sscanf(inpIcrex, "%d", &optIcrex) == 1) {
        switch (optIcrex) {
        case 1:
          printf("Threshold Mode\n");
          bool thr;
          thr = true;
          char inpThr[BUFFER_SIZE];
          int optThr;
          // prepare threshold buffer
          write(fd, exIcr_on, sizeof(exIcr_on));
          read(fd, bufT, sizeof(bufT));  // Read response
          memset(bufT, 0, sizeof(bufT)); // reset buffer i

          while (thr) {
            // prepare threshold buffer
            memset(bufT, 0, sizeof(bufT)); // reset buffer i
            write(fd, exIcr_threshINQ, sizeof(exIcr_threshINQ));
            //	 rdlenT = read(fd, bufT, sizeof(bufT)); // Read response
            read(fd, bufT, sizeof(bufT)); // Read response

            printf("\n\n\n");
            printf("1) Increment\n2) Decrement\n0) Quit\n");
            if (fgets(inpThr, sizeof(inpThr), stdin) != NULL) {
              if (sscanf(inpThr, "%d", &optThr)) {
                switch (optThr) {
                case 1:
                  /*		      // Debug: Print raw data
if(rdlenT > 0){
printf("Raw data: ");
  for (int i = 0; i < rdlenT; i++) {
printf("%X(%d) ", bufT[i], i);
}
printf("\n");
}*/

                  // printf("Current Threshold: %X%X \n", bufT[4],bufT[5]);

                  // exIcr_thresh
                  if (bufT[4] == 0x0F && bufT[5] == 0x0F) {
                    printf("Threshold is already at maximum value\n");
                  } else {
                    // Increment threshold value in buffer
                    if (bufT[5] == 0x0F) {
                      bufT[4]++;
                      bufT[5] = 0x00;
                    } else {
                      bufT[5]++;
                    }
                    exIcr_thresh[7] = bufT[4];
                    exIcr_thresh[8] = bufT[5];
                  }

                  write(fd, exIcr_thresh, sizeof(exIcr_thresh));
                  //	rdlenT = read(fd, bufT, sizeof(bufT)); // Read response
                  read(fd, bufT, sizeof(bufT));  // Read response
                  memset(bufT, 0, sizeof(bufT)); // reset buffer i
                  printf("Threshold incremented to: %X%X\n", exIcr_thresh[7],
                         exIcr_thresh[8]);

                  break;
                case 2:

                  /*				// Debug: Print raw data
if(rdlenT > 0){
printf("Raw data: ");
for (int i = 0; i < rdlenT; i++) {
printf("%X(%d) ", bufT[i], i);
}
printf("\n");
}*/

                  // exIcr_thresh
                  if (bufT[4] == 0x00 && bufT[5] == 0x00) {
                    printf("Threshold is already at minimum value\n");
                  } else {
                    // decrement threshold value in buffer
                    if (bufT[5] == 0x00) {
                      bufT[4]--;
                      bufT[5] = 0x0F;
                    } else {
                      bufT[5]--;
                    }
                    exIcr_thresh[7] = bufT[4];
                    exIcr_thresh[8] = bufT[5];
                  }

                  write(fd, exIcr_thresh, sizeof(exIcr_thresh));
                  // rdlenT = read(fd, bufT, sizeof(bufT)); // Read response
                  read(fd, bufT, sizeof(bufT));  // Read response
                  memset(bufT, 0, sizeof(bufT)); // reset buffer i

                  printf("Threshold decremented to: %X%X\n", exIcr_thresh[7],
                         exIcr_thresh[8]);

                  break;
                case 0:
                  printf("Quitting\n");

                  thr = false;
                  break;
                }
              }
            }
          }

          break;
        case 2:
          printf("Level mode\n");

          bool lvl;
          lvl = true;
          char inplvl[BUFFER_SIZE];
          int optlvl;
          // prepare lvl buffer
          write(fd, exIcr_on, sizeof(exIcr_on)); // on
          read(fd, bufL, sizeof(bufL));          // Read response
          memset(bufL, 0, sizeof(bufL));         // reset buffer i

          while (lvl) {
            // prepare lvl buffer
            memset(bufL, 0, sizeof(bufL)); // reset buffer i
            write(fd, exIcr_levelINQ, sizeof(exIcr_levelINQ));
            // rdlenL = read(fd, bufL, sizeof(bufL)); // Read response
            read(fd, bufL, sizeof(bufL)); // Read response

            printf("\n\n\n");
            printf("1) Increment\n2) Decrement\n0) Quit\n");
            if (fgets(inplvl, sizeof(inplvl), stdin) != NULL) {
              if (sscanf(inplvl, "%d", &optlvl)) {
                switch (optlvl) {
                case 1:


                  // exIcr_lvl
                  if (bufL[4] == 0x01 && bufL[5] == 0x0C) {
                    printf("Level is already at maximum value\n");
                  } else {
                    // Increment lvl value in buffer
                    if (bufL[5] == 0x0F) {
                      bufL[4]++;
                      bufL[5] = 0x00;
                    } else {
                      bufL[5]++;
                    }
                    exIcr_level[7] = bufL[4];
                    exIcr_level[8] = bufL[5];
                  }

                  write(fd, exIcr_level, sizeof(exIcr_level));
                  // rdlenL = read(fd, bufL, sizeof(bufL)); // Read response
                  read(fd, bufL, sizeof(bufL));  // Read response
                  memset(bufL, 0, sizeof(bufL)); // reset buffer i
                  printf("Level incremented to: %X%X\n", exIcr_level[7],
                         exIcr_level[8]);

                  break;
                case 2:

                  // exIcr_thresh
                  if (bufL[4] == 0x00 && bufL[5] == 0x00) {
                    printf("Level is already at minimum value\n");
                  } else {
                    // decrement lvl value in buffer
                    if (bufL[5] == 0x00) {
                      bufL[4]--;
                      bufL[5] = 0x0F;
                    } else {
                      bufL[5]--;
                    }
                    exIcr_level[7] = bufL[4];
                    exIcr_level[8] = bufL[5];
                  }

                  write(fd, exIcr_level, sizeof(exIcr_level));
                  // rdlenL = read(fd, bufL, sizeof(bufL)); // Read response
                  read(fd, bufL, sizeof(bufL));  // Read response
                  memset(bufL, 0, sizeof(bufL)); // reset buffer i

                  printf("Level decremented to: %X%X\n", exIcr_level[7],
                         exIcr_level[8]);

                  break;
                case 0:
                  printf("Quitting\n");
                  lvl = false;
                  break;
                }
              }
            }
          }

          break;
        case 0:
          printf("Quitting\n");
          write(fd, exIcr_off, sizeof(exIcr_off));
          icrEX = false;
          break;
        }
      }
    }
  }
}

// Extend color
void colorF(int fd) {
  bool color;
  color = true;
  char inpCol[BUFFER_SIZE];
  int optCol;

  unsigned char bufG[BUFFER_SIZE] = {0};
  // int rdlenG = 0;
  unsigned char bufH[BUFFER_SIZE] = {0};
  // int rdlenH = 0;

  while (color) {

    // initialize
    write(fd, exColor_on, sizeof(exColor_on));
    // rdlenG = read(fd, bufG, sizeof(bufG)); // Read response
    read(fd, bufG, sizeof(bufG));  // Read response
    memset(bufG, 0, sizeof(bufG)); // reset buffer i

    printf("\n\n\n");
    // prepare gain buffer
    memset(bufG, 0, sizeof(bufG)); // reset buffer i
    write(fd, exColor_gainINQ, sizeof(exColor_gainINQ));
    // rdlenG = read(fd, bufG, sizeof(bufG)); // Read response
    read(fd, bufG, sizeof(bufG)); // Read response
    // prepare hue buffer
    memset(bufH, 0, sizeof(bufH)); // reset buffer i
    write(fd, exColor_hueINQ, sizeof(exColor_hueINQ));
    // rdlenH = read(fd, bufH, sizeof(bufH)); // Read response
    read(fd, bufH, sizeof(bufH)); // Read response
    printf("Current Gain: %X%X \n", bufG[2], bufG[3]);
    printf("Current Hue: %X%X \n", bufH[2], bufH[3]);

    printf("Please Select one of the following options\n");
    printf("1) Gain\n2) Hue\n0) Quit\n");
    write(fd, exColor_on, sizeof(exColor_on));

    if (fgets(inpCol, sizeof(inpCol), stdin) != NULL) {
      if (sscanf(inpCol, "%d", &optCol) == 1) {
        switch (optCol) {
        case 1:
          printf("Gain on");

          bool gain;
          gain = true;
          char inpG[BUFFER_SIZE];
          int optG;

          while (gain) {
            printf("\n\n\n");
            printf("Please Select on of the following options\n");
            printf("1) Increment\n2) Decrement\n0) Quit\n");
            if (fgets(inpG, sizeof(inpG), stdin) != NULL) {
              if (sscanf(inpG, "%d", &optG) == 1) {
                switch (optG) {
                case 1:

                  // exIcr_thresh
                  if (bufG[2] == 0x0F && bufG[3] == 0x0F) {
                    printf("Gain is already at maximum value\n");
                  } else {
                    // decrement threshold value in buffer
                    if (bufG[3] == 0x0F) {
                      bufG[2]++;
                      bufG[3] = 0x00;
                    } else {
                      bufG[3]++;
                    }
                    exColor_gain[7] = bufG[2];
                    exColor_gain[8] = bufG[3];
                  }
                  write(fd, exColor_gain, sizeof(exColor_gain));
                  // rdlenG = read(fd, bufG, sizeof(bufG)); // Read response
                  read(fd, bufG, sizeof(bufG));  // Read response
                  memset(bufG, 0, sizeof(bufG)); // reset buffer i
                  write(fd, exColor_gainINQ, sizeof(exColor_gainINQ));
                  // rdlenG = read(fd, bufG, sizeof(bufG)); // Read response
                  read(fd, bufG, sizeof(bufG)); // Read response
                  printf("Gain Incremented to: %X%X\n", exColor_gain[7],
                         exColor_gain[8]);

                  break;
                case 2:

                  if (bufG[2] == 0x00 && bufG[3] == 0x00) {
                    printf("Gain is already at minimum value\n");
                  } else {
                    // decrement threshold value in buffer
                    if (bufG[3] == 0x00) {
                      bufG[2]--;
                      bufG[3] = 0x0F;
                    } else {
                      bufG[3]--;
                    }
                    exColor_gain[7] = bufG[2];
                    exColor_gain[8] = bufG[3];
                  }
                  write(fd, exColor_gain, sizeof(exColor_gain));
                  //	rdlenG = read(fd, bufG, sizeof(bufG)); // Read response
                  read(fd, bufG, sizeof(bufG));  // Read response
                  memset(bufG, 0, sizeof(bufG)); // reset buffer i
                  write(fd, exColor_gainINQ, sizeof(exColor_gainINQ));
                  // rdlenG = read(fd, bufG, sizeof(bufG)); // Read response
                  read(fd, bufG, sizeof(bufG)); // Read response
                  printf("Gain Decremented to: %X%X\n", exColor_gain[7],
                         exColor_gain[8]);

                  break;
                case 0:
                  printf("Quitting\n");
                  gain = false;
                  break;
                }
              }
            }
          }

          break;
        case 2:

          printf("Hue Mode\n");

          bool hue;
          hue = true;
          char inpH[BUFFER_SIZE];
          int optH;

          while (hue) {
            printf("\n\n\n");
            printf("Please Select on of the following options\n");
            printf("1) Increment\n2) Decrement\n0) Quit\n");
            if (fgets(inpH, sizeof(inpH), stdin) != NULL) {
              if (sscanf(inpH, "%d", &optH) == 1) {
                switch (optH) {
                case 1:

                  if (bufH[2] == 0x0F && bufH[3] == 0x0F) {
                    printf("Hue is already at maximum value\n");
                  } else {
                    // decrement threshold value in buffer
                    if (bufH[3] == 0x0F) {
                      bufH[2]++;
                      bufH[3] = 0x00;
                    } else {
                      bufH[3]++;
                    }
                    exColor_hue[7] = bufH[2];
                    exColor_hue[8] = bufH[3];
                  }
                  write(fd, exColor_hue, sizeof(exColor_hue));
                  // rdlenH = read(fd, bufH, sizeof(bufH)); // Read response
                  read(fd, bufH, sizeof(bufH));  // Read response
                  memset(bufH, 0, sizeof(bufH)); // reset buffer i
                  write(fd, exColor_hueINQ, sizeof(exColor_hueINQ));
                  // rdlenH = read(fd, bufH, sizeof(bufH)); // Read response
                  read(fd, bufH, sizeof(bufH)); // Read response
                  printf("Hue Incremented to: %X%X\n", exColor_hue[7],
                         exColor_hue[8]);

                  break;
                case 2:

                  if (bufH[2] == 0x00 && bufH[3] == 0x00) {
                    printf("Hue is already at minimum value\n");
                  } else {
                    // decrement threshold value in buffer
                    if (bufH[3] == 0x00) {
                      bufH[2]--;
                      bufH[3] = 0x0F;
                    } else {
                      bufH[3]--;
                    }
                    exColor_hue[7] = bufH[2];
                    exColor_hue[8] = bufH[3];
                  }
                  write(fd, exColor_hue, sizeof(exColor_hue));
                  // rdlenH = read(fd, bufH, sizeof(bufH)); // Read response
                  read(fd, bufH, sizeof(bufH));  // Read response
                  memset(bufH, 0, sizeof(bufH)); // reset buffer i
                  write(fd, exColor_hueINQ, sizeof(exColor_hueINQ));
                  // rdlenH = read(fd, bufH, sizeof(bufH)); // Read response
                  read(fd, bufH, sizeof(bufH)); // Read response
                  printf("Hue Decremented to: %X%X\n", exColor_hue[7],
                         exColor_hue[8]);

                  break;
                case 0:
                  printf("Quitting\n");
                  hue = false;
                  break;
                }
              }
            }
          }

          break;
        case 0:
          printf("Quitting\n");
          write(fd, exColor_off, sizeof(exColor_off));
          color = false;
          break;
        }
      }
    }
  }
}

// Sub Menu used in main Method/Menu
void extend(int fd) {
  bool extend;
  extend = true;
  char inpEX[BUFFER_SIZE];
  int optEX;

  while (extend) {
    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) Extend Exp Comp\n2) Extend Aperture\n3) Extend Auto ICR\n4) "
           "Extend Color\n0) Quit\n");
    if (fgets(inpEX, sizeof(inpEX), stdin) != NULL) {
      if (sscanf(inpEX, "%d", &optEX) == 1) {
        switch (optEX) {
        case 1:
          compF(fd);
          break;
        case 2:
          apeExF(fd);
          break;
        case 3:
          icrExF(fd);
          break;
        case 4:
          colorF(fd);
          break;
        case 0:
          printf("Quiting\n");
          extend = false;
          break;
        }
      }
    }
  }
}

