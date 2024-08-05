

/*----------------------------------------
IQ Function called in visca.c
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

// wide d / ve
unsigned char wideD_on[] = {0x81, 0x01, 0x04, 0x3D, 0x02, 0xFF};
unsigned char ve_on[] = {0x81, 0x01, 0x04, 0x3D, 0x06, 0xFF};
unsigned char wideVE_off[] = {0x81, 0x01, 0x04, 0x3D, 0x03, 0xFF};
unsigned char wideVE_param[] = {0x81, 0x01, 0x04, 0x2D, 0x00, 0x03, 0x02,
                                0x01, 0x00, 0x00, 0x00, 0x00, 0xFF};
unsigned char wideD_paramINQ[] = {0x81, 0x09, 0x04, 0x2D, 0xFF};
unsigned char ve_paramINQ[] = {0x81, 0x09, 0x04, 0x2D, 0xFF};

// defogf
unsigned char defog_on[] = {0x81, 0x01, 0x04, 0x37,
                            0x02, 0x01, 0xFF}; // low = 1 medium = 2 high = 3
unsigned char defog_off[] = {0x81, 0x01, 0x04, 0x37, 0x03, 0x00, 0xFF};
unsigned char defogINQ[] = {0x81, 0x09, 0x04, 0x37, 0xFF};

// ICR
unsigned char icr_on[] = {0x81, 0x01, 0x04,
                          0x01, 0x02, 0xFF}; // 8x 01 04 01 02 FF
unsigned char icr_off[] = {0x81, 0x01, 0x04, 0x01, 0x03, 0xFF};
unsigned char autoicr_on[] = {0x81, 0x01, 0x04, 0x51, 0x02, 0xFF};
unsigned char autoicr_color[] = {0x81, 0x01, 0x04, 0x51, 0x04, 0xFF};
unsigned char autoicr_off[] = {0x81, 0x01, 0x04, 0x51, 0x03, 0xFF};

// HLC
unsigned char hlc_param[] = {0x81, 0x01, 0x04, 0x14,
                             0x00, 0x00, 0xFF}; // 8x 01 04 14 0p 0q FF
unsigned char hlc_paramINQ[] = {0x81, 0x09, 0x04, 0x14, 0xFF}; // 8x 09 04 14 FF

// Stabilizer
unsigned char stab_on[] = {0x81, 0x01, 0x04, 0x34, 0x02, 0xFF};
unsigned char stab_off[] = {0x81, 0x01, 0x04, 0x34, 0x03, 0xFF};
unsigned char stab_hold[] = {0x81, 0x01, 0x04, 0x34, 0x00, 0xFF};
unsigned char stab_level[] = {0x81, 0x01, 0x7E, 0x04,
                              0x34, 0x02, 0xFF};               // param 2 or 3
unsigned char stab_modeINQ[] = {0x81, 0x09, 0x04, 0x34, 0xFF}; // 8x 09 04 34 FF
unsigned char stab_levelINQ[] = {0x81, 0x09, 0x7E, 0x04, 0x34, 0xFF};

// Aperture
unsigned char ap_reset[] = {0x81, 0x01, 0x04, 0x02, 0x00, 0xFF};
unsigned char ap_inc[] = {0x81, 0x01, 0x04, 0x02, 0x02, 0xFF};
unsigned char ap_dec[] = {0x81, 0x01, 0x04, 0x02, 0x03, 0xFF};
unsigned char apINQ[] = {0x81, 0x09, 0x04, 0x42, 0xFF};

// Noise Reduction
unsigned char nr_param[] = {0x81, 0x01, 0x04, 0x53, 0x00, 0xFF};
unsigned char nr_ind[] = {0x81, 0x01, 0x05, 0x53, 0x00, 0x00, 0xFF};
unsigned char nrINQ[] = {0x81, 0x09, 0x04, 0x53, 0xFF};
unsigned char nrindINQ[] = {0x81, 0x09, 0x05, 0x53, 0xFF};

// Gamma
unsigned char gamma_param[] = {0x81, 0x01, 0x04, 0x5B, 0x00, 0xFF}; // 0 or 1
unsigned char gamma_offset[] = {0x81, 0x01, 0x04, 0x1E, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00, 0xFF};
unsigned char gamma_offsetINQ[] = {0x81, 0x09, 0x04, 0x1E,
                                   0xFF}; // 8x 09 04 1E FF

// private functions
//  private wideVE function
void wideF(int fd) {
  printf("Wide D/ VE Mode\n");
  bool wideVE, wideD, dbl, bc, cl, ve, Vdbl, Vbc, Vcl;
  wideVE = true;
  char inputwideVE[BUFFER_SIZE];
  int optwideVE;

  while (wideVE) { // wide VE whilelo
    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) Wide D\n2) VE\n3) Off\n0) Quit\n");
    if (fgets(inputwideVE, sizeof(inputwideVE), stdin) != NULL) {
      if (sscanf(inputwideVE, "%d", &optwideVE) == 1) {
        switch (optwideVE) { // wide VE switch statement
        case 1:

          printf("Wide-D on\n");
          write(fd, wideD_on, sizeof(wideD_on));

          wideD = true;
          char inpWide[BUFFER_SIZE];
          int optWide;
          unsigned char buf[BUFFER_SIZE] = {0};
          // int rdlen = 0;

          // clearing the wideD_on command

          read(fd, buf, sizeof(buf) - 1); // read
          memset(buf, 0, sizeof(buf));

          while (wideD) {
            // set up buf to read input for every iteration
            memset(buf, 0, sizeof(buf));
            write(fd, wideD_paramINQ, sizeof(wideD_paramINQ)); // query
            read(fd, buf, sizeof(buf) - 1);
            // read(fd, buf, sizeof(buf) - 1); // read
            //  Debug: Print raw data
            /*     if(rdlen > 0){
                   printf("Raw data: ");
                         for (int i = 0; i < rdlen; i++) {
                     printf("%X(%d) ", buf[i], i);
                   }
                     printf("\n");
                 }*/

            printf("\n\n\n"); // show data after buf is set
            printf("Current Parameters for wide-d:\n");
            printf("-----------------------------------\n");
            printf("Display Brightness Level: %d\n", buf[3]);
            printf("Brightness Compensation: %d\n", buf[4]);
            printf("Compensation Level: %d\n", buf[5]);
            printf("\n\n\n");
            memset(buf, 0, sizeof(buf));

            printf("Please choose one of the following options\n");
            printf("1) Display Brightness Level\n2) Brightness Compensation "
                   "Selection\n3) Compensation level\n0) Quit\n");
            if (fgets(inpWide, sizeof(inpWide), stdin) != NULL) {
              if (sscanf(inpWide, "%d", &optWide) == 1) {
                switch (optWide) {
                case 1:

                  dbl = true;
                  char inpDBL[BUFFER_SIZE];
                  int optDBL;

                  while (dbl) {
                    // clear buffer and read in parameter
                    memset(buf, 0, sizeof(buf));
                    write(fd, wideD_paramINQ, sizeof(wideD_paramINQ)); // query
                    read(fd, buf, sizeof(buf) - 1);

                    printf("\n\n\n");
                    printf("0(Dark) ---> 6(Bright)\n");
                    printf("Enter an integer 0-6 to change Display Brightness "
                           "level: (9 to quit)\n");
                    if (fgets(inpDBL, sizeof(inpDBL), stdin) != NULL) {
                      if (sscanf(inpDBL, "%d", &optDBL)) {
                        if (optDBL == 9) {
                          dbl = false;

                        } else if (optDBL >= 0 && optDBL <= 6) {

                          wideVE_param[5] = 0x0 | optDBL; // change
                        }
                        // make change and clear buffer to read in parameter

                        write(fd, wideVE_param,
                              sizeof(wideVE_param));    // write change
                        read(fd, buf, sizeof(buf) - 1); // read
                        memset(buf, 0, sizeof(buf));    // reset buffer
                        write(fd, wideD_paramINQ,
                              sizeof(wideD_paramINQ)); // inquiry
                        // rdlen = read(fd, buf, sizeof(buf) - 1); // read into
                        // buffer
                        read(fd, buf, sizeof(buf) - 1); // read
                        printf("Changed Brightnes Level to: %X\n", buf[3]);
                      }
                    }
                  }

                  break;
                case 2:

                  bc = true;
                  char inpBC[BUFFER_SIZE];
                  int optBC;

                  while (bc) {
                    memset(buf, 0, sizeof(buf));
                    write(fd, wideD_paramINQ, sizeof(wideD_paramINQ)); // query
                    read(fd, buf, sizeof(buf) - 1);

                    printf("\n\n\n");
                    printf("0(Very Dark) ---> 3(Bright)\n");
                    printf("Enter an integer 0-3 to change Brightness "
                           "Compensation: (9 to quit)\n");
                    if (fgets(inpBC, sizeof(inpBC), stdin) != NULL) {
                      if (sscanf(inpBC, "%d", &optBC)) {
                        if (optBC == 9) {
                          bc = false;

                        } else if (optBC >= 0 && optBC <= 3) {

                          wideVE_param[6] = 0x0 | optBC; // change
                        }
                        // make change and clear buffer to read in parameter

                        write(fd, wideVE_param,
                              sizeof(wideVE_param));    // write change
                        read(fd, buf, sizeof(buf) - 1); // read
                        memset(buf, 0, sizeof(buf));    // reset buffer
                        write(fd, wideD_paramINQ,
                              sizeof(wideD_paramINQ)); // inquiry
                        // rdlen = read(fd, buf, sizeof(buf) - 1); // read into
                        // buffer
                        read(fd, buf, sizeof(buf) - 1); // read
                        printf("Changed Brightnes Compensation to: %X\n",
                               buf[4]);
                      }
                    }
                  }

                  break;
                case 3:

                  cl = true;
                  char inpCL[BUFFER_SIZE];
                  int optCL;

                  while (cl) {

                    memset(buf, 0, sizeof(buf));
                    write(fd, wideD_paramINQ, sizeof(wideD_paramINQ)); // query
                    read(fd, buf, sizeof(buf) - 1);

                    printf("\n\n\n");
                    printf("0(Low) ---> 2(High)\n");
                    printf("Enter an integer 0-2 to change Compensation Level: "
                           "(9 to quit)\n");
                    if (fgets(inpCL, sizeof(inpCL), stdin) != NULL) {
                      if (sscanf(inpCL, "%d", &optCL)) {
                        if (optCL == 9) {
                          cl = false;
                          write(fd, wideVE_param,
                                sizeof(wideVE_param)); // write change
                        } else if (optCL >= 0 && optCL <= 3) {

                          wideVE_param[7] = 0x0 | optCL; // change
                        }
                        write(fd, wideVE_param,
                              sizeof(wideVE_param));    // write change
                        read(fd, buf, sizeof(buf) - 1); // read
                        memset(buf, 0, sizeof(buf));    // reset buffer
                        write(fd, wideD_paramINQ,
                              sizeof(wideD_paramINQ)); // inquiry
                        // rdlen = read(fd, buf, sizeof(buf) - 1); // read into
                        // buffer
                        read(fd, buf, sizeof(buf) - 1); // read
                        printf("Changed Compensation Level to: %X\n", buf[5]);
                      }
                    }
                  }

                  break;
                case 0:
                  printf("Quitting Wide D\n");
                  wideD = false;
                  break;
                }
              }
            }
          }

          break;
        case 2:
          printf("VE on\n");
          write(fd, ve_on, sizeof(ve_on));

          ve = true;
          char inpVE[BUFFER_SIZE];
          int optVE;
          unsigned char bufV[BUFFER_SIZE] = {0};
          // int rdlenV = 0;

          // clear ve_on from buffer
          memset(bufV, 0, sizeof(bufV));
          read(fd, bufV, sizeof(bufV) - 1);

          while (ve) {
            // prepare buffer to be read
            memset(bufV, 0, sizeof(bufV));
            write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // query
            read(fd, bufV, sizeof(bufV) - 1);

            printf("\n\n\n"); // show data
            printf("Current Parameters for VE:\n");
            printf("-----------------------------------\n");
            printf("Display Brightness Level: %d\n", bufV[3]);
            printf("Brightness Compensation: %d\n", bufV[4]);
            printf("Compensation Level: %d\n", bufV[5]);
            printf("\n\n\n");
            memset(bufV, 0, sizeof(bufV));

            printf("Please choose one of the following options\n");
            printf("1) Display Brightness Level\n2) Brightness Compensation "
                   "Selection\n3) Compensation level\n0) Quit\n");
            if (fgets(inpVE, sizeof(inpVE), stdin) != NULL) {
              if (sscanf(inpVE, "%d", &optVE) == 1) {
                switch (optVE) {
                case 1:

                  Vdbl = true;
                  char inpVDBL[BUFFER_SIZE];
                  int optVDBL;

                  while (Vdbl) {
                    memset(bufV, 0, sizeof(bufV));
                    write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // query
                    read(fd, bufV, sizeof(bufV) - 1);

                    printf("\n\n\n");
                    printf("0(Dark) ---> 6(Bright)\n");
                    printf("Enter an integer 0-6 to change Display Brightness "
                           "level: (9 to quit)\n");
                    if (fgets(inpVDBL, sizeof(inpVDBL), stdin) != NULL) {
                      if (sscanf(inpVDBL, "%d", &optVDBL)) {
                        if (optVDBL == 9) {
                          Vdbl = false;

                        } else if (optVDBL >= 0 && optVDBL <= 6) {

                          wideVE_param[5] = 0x0 | optVDBL; // change
                        }

                        write(fd, wideVE_param,
                              sizeof(wideVE_param));      // write change
                        read(fd, bufV, sizeof(bufV) - 1); // read
                        memset(bufV, 0, sizeof(bufV));    // reset buffer
                        write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // inquiry
                        // rdlen = read(fd, bufV, sizeof(buf) - 1); // read into
                        // buffer
                        read(fd, bufV, sizeof(bufV) - 1); // read
                        printf("Changed Brightness Level to: %X\n", bufV[3]);
                      }
                    }
                  }

                  break;
                case 2:

                  Vbc = true;
                  char inpVBC[BUFFER_SIZE];
                  int optVBC;

                  while (Vbc) {
                    memset(bufV, 0, sizeof(bufV));
                    write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // query
                    read(fd, bufV, sizeof(bufV) - 1);
                    printf("\n\n\n");
                    printf("0(Very Dark) ---> 3(Bright)\n");
                    printf("Enter an integer 0-3 to change Brightness "
                           "Compensation: (9 to quit)\n");
                    if (fgets(inpVBC, sizeof(inpVBC), stdin) != NULL) {
                      if (sscanf(inpVBC, "%d", &optVBC)) {
                        if (optVBC == 9) {
                          Vbc = false;
                          write(fd, wideVE_param,
                                sizeof(wideVE_param)); // write change
                        } else if (optVBC >= 0 && optVBC <= 3) {

                          wideVE_param[6] = 0x0 | optVBC; // change
                        }
                        write(fd, wideVE_param,
                              sizeof(wideVE_param));      // write change
                        read(fd, bufV, sizeof(bufV) - 1); // read
                        memset(bufV, 0, sizeof(bufV));    // reset buffer
                        write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // inquiry
                        // rdlen = read(fd, bufV, sizeof(buf) - 1); // read into
                        // buffer
                        read(fd, bufV, sizeof(bufV) - 1); // read
                        printf("Changed Brightness Compensation to: %X\n",
                               bufV[4]);
                      }
                    }
                  }

                  break;
                case 3:

                  Vcl = true;
                  char inpVCL[BUFFER_SIZE];
                  int optVCL;

                  while (Vcl) {
                    memset(bufV, 0, sizeof(bufV));
                    write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // query
                    read(fd, bufV, sizeof(bufV) - 1);
                    printf("\n\n\n");
                    printf("0(Low) ---> 2(High)\n");
                    printf("Enter an integer 0-2 to change Compensation Level: "
                           "(9 to quit)\n");
                    if (fgets(inpVCL, sizeof(inpVCL), stdin) != NULL) {
                      if (sscanf(inpVCL, "%d", &optVCL)) {
                        if (optVCL == 9) {
                          Vcl = false;
                          write(fd, wideVE_param,
                                sizeof(wideVE_param)); // write change
                        } else if (optVCL >= 0 && optVCL <= 3) {

                          wideVE_param[7] = 0x0 | optVCL; // change
                        }
                        write(fd, wideVE_param,
                              sizeof(wideVE_param));      // write change
                        read(fd, bufV, sizeof(bufV) - 1); // read
                        memset(bufV, 0, sizeof(bufV));    // reset buffer
                        write(fd, ve_paramINQ, sizeof(ve_paramINQ)); // inquiry
                        // rdlen = read(fd, bufV, sizeof(buf) - 1); // read into
                        // buffer
                        read(fd, bufV, sizeof(bufV) - 1); // read
                        printf("Changed Compensation Level to: %X\n", bufV[5]);
                      }
                    }
                  }
                  break;
                case 0:
                  printf("Quitting VE\n");
                  ve = false;
                  break;
                }
              }
            }
          }

          break;
        case 3:
          printf("OFF\n");
          write(fd, wideVE_off, sizeof(wideVE_off));
          break;
        case 0:
          printf("Quitting wide VE\n");
          wideVE = false;
          break;
        }
      }
    }
  }
}

// private defog function
void defogF(int fd) {

  bool defog, DefogOn;
  defog = true;
  char inpDefog[BUFFER_SIZE];
  int optDefog;
  unsigned char bufD[BUFFER_SIZE] = {0};
  //  int rdlenD = 0;

  while (defog) {
    printf("\n\n\n");
    printf("Defog mode\n");
    printf("Please select one of the following options\n");
    printf("1) Defog on\n2) Defog off\n0) Quit\n");
    if (fgets(inpDefog, sizeof(inpDefog), stdin) != NULL) {
      if (sscanf(inpDefog, "%d", &optDefog) == 1) {
        switch (optDefog) {
        case 1:

          DefogOn = true;
          char inpDefogOn[BUFFER_SIZE];
          int optDefogOn;

          while (DefogOn) {
            printf("\n\n\n");

            memset(bufD, 0, sizeof(bufD));
            write(fd, defogINQ, sizeof(defogINQ));
            // rdlenD = read(fd, bufD, sizeof(bufD) - 1);
            read(fd, bufD, sizeof(bufD) - 1);

            /*  // Debug: Print raw data
  if(rdlenD > 0){
    printf("Raw data: ");
          for (int i = 0; i < rdlenD; i++) {
      printf("%X(%d) ", bufD[i], i);
    }
      printf("\n");
  }*/
            printf("Defog Status\n");
            if (bufD[3] == 0) {
              printf("Defog is Off or not set yet\n");
            } else if (bufD[3] == 1) {
              printf("Defog is Low\n");
            } else if (bufD[3] == 2) {
              printf("Defog is Medium\n");
            } else if (bufD[3] == 3) {
              printf("Defog is High\n");
            }

            printf("Choose one of the following options\n");
            printf("1) low\n2) medium\n3) high\n0) Quit\n");
            if (fgets(inpDefogOn, sizeof(inpDefogOn), stdin) != NULL) {
              if (sscanf(inpDefogOn, "%d", &optDefogOn) == 1) {
                if (optDefogOn == 0) {
                  DefogOn = false;
                } else if (optDefogOn >= 1 && optDefogOn <= 3) {
                  defog_on[5] = 0x0 | optDefogOn;

                  write(fd, defog_on, sizeof(defog_on));
                  read(fd, bufD, sizeof(bufD) - 1);
                  memset(bufD, 0, sizeof(bufD)); // reset buffer
                  write(fd, defogINQ, sizeof(defogINQ));
                  // rdlenD = read(fd, bufD, sizeof(bufD) - 1);
                  read(fd, bufD, sizeof(bufD) - 1);
                }
              }
            }
          }

          break;
        case 2:
          printf("Defog Off\n");
          write(fd, defog_off, sizeof(defog_off));
          break;
        case 0:
          printf("Quitting defog\n");
          defog = false;
          break;
        }
      }
    }
  }
}

// private ICR function
void icrF(int fd) {
  bool icr, icrM, icrA;
  icr = true;
  char inpIcr[BUFFER_SIZE];
  int optIcr;
  while (icr) {
    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) Manual ICR \n2) Auto ICR\n0) Quit\n");
    if (fgets(inpIcr, sizeof(inpIcr), stdin) != NULL) {
      if (sscanf(inpIcr, "%d", &optIcr) == 1) {
        switch (optIcr) {
        case 1:
          printf("Manual ICR");

          icrM = true;
          char inpIcrM[BUFFER_SIZE];
          int optIcrM;
          while (icrM) {
            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("1) On\n2) Off\n0) Quit\n");
            if (fgets(inpIcrM, sizeof(inpIcrM), stdin) != NULL) {
              if (sscanf(inpIcrM, "%d", &optIcrM) == 1) {
                switch (optIcrM) {
                case 1:
                  printf("ICR On\n");
                  write(fd, icr_on, sizeof(icr_on));

                  break;
                case 2:
                  printf("ICR Off\n");
                  write(fd, icr_off, sizeof(icr_off));
                  break;
                case 0:
                  printf("Quitting\n");
                  icrM = false;
                  break;
                }
              }
            }
          }

          break;
        case 2:
          printf("Auto ICR");

          icrA = true;
          char inpIcrA[BUFFER_SIZE];
          int optIcrA;
          while (icrA) {
            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("1) On\n2) On (Color)\n3) Off\n0) Quit\n");
            if (fgets(inpIcrA, sizeof(inpIcrA), stdin) != NULL) {
              if (sscanf(inpIcrA, "%d", &optIcrA) == 1) {
                switch (optIcrA) {
                case 1:
                  printf("ICR Auto On\n");
                  write(fd, autoicr_on, sizeof(autoicr_on));

                  break;
                case 4:
                  printf("ICR Auto Color On\n");
                  write(fd, autoicr_color, sizeof(autoicr_color));
                  break;
                case 3:
                  printf("ICR Auto Off\n");
                  write(fd, autoicr_off, sizeof(autoicr_off));
                  break;
                case 0:
                  printf("Quitting\n");
                  icrA = false;
                  break;
                }
              }
            }
          }
          break;
        case 0:
          printf("Quitting\n");
          icr = false;
          break;
        }
      }
    }
  }
}

// private HLC function
void hlcF(int fd) {
  bool hlc, level, mask;
  hlc = true;
  char inpHlc[BUFFER_SIZE];
  int optHlc;
  unsigned char bufH[BUFFER_SIZE] = {0};
  // int rdlenH = 0;

  // Debug: Print raw data
  // bufH[2] = level
  // bufH[3] = mask
  /*    if(rdlenH > 0){
        printf("Raw data: ");
              for (int i = 0; i < rdlenH; i++) {
          printf("%X(%d) ", bufH[i], i);
        }
          printf("\n");
      }*/

  while (hlc) {

    memset(bufH, 0, sizeof(bufH));
    write(fd, hlc_paramINQ, sizeof(hlc_paramINQ)); // query
    // rdlenH = read(fd,bufH,sizeof(bufH)-1);//read
    read(fd, bufH, sizeof(bufH) - 1); // read

    // Debug: Print raw data
    // bufH[2] = level
    // bufH[3] = mask
    /*             if(rdlenH > 0){
                    printf("Raw data: ");
                          for (int i = 0; i < rdlenH; i++) {
                      printf("%X(%d) ", bufH[i], i);
                    }
                      printf("\n");
                  }*/

    printf("\n\n\n");

    printf("Current Level and Mask values\n");
    printf("----------------------------------\n");
    printf("Level: %d \n", bufH[2]);
    printf("Mask: %d \n", bufH[3]);

    printf("Please choose one of the following options\n");
    printf("1) Level\n2) Mask\n0) Quit\n");

    if (fgets(inpHlc, sizeof(inpHlc), stdin) != NULL) {
      if (sscanf(inpHlc, "%d", &optHlc) == 1) {
        switch (optHlc) {
        case 1:

          level = true;
          char inpLevel[BUFFER_SIZE];
          int optLevel;
          while (level) {
            printf("\n\n\n");
            printf("Please choose an option below\n");
            printf("(0: OFF, 1: Low, 2: Mid, 3: High, 9: Quit)\n");
            if (fgets(inpLevel, sizeof(inpLevel), stdin) != NULL) {
              if (sscanf(inpLevel, "%d", &optLevel) == 1) {
                if (optLevel == 9) {
                  level = false;
                } else if (optLevel >= 0 && optLevel <= 3) {
                  hlc_param[4] = 0x0 | optLevel;
                }

                write(fd, hlc_param, sizeof(hlc_param));
                read(fd, bufH, sizeof(bufH) - 1);              // read
                memset(bufH, 0, sizeof(bufH));                 // reset buffer
                write(fd, hlc_paramINQ, sizeof(hlc_paramINQ)); // query
                // rdlenH = read(fd,bufH,sizeof(bufH)-1);//read
                read(fd, bufH, sizeof(bufH) - 1); // read
                printf("Current HLC Level: %X\n", bufH[2]);
                memset(bufH, 0, sizeof(bufH)); // reset buffer
              }
            }
          }
          break;
        case 2:

          mask = true;
          char inpMask[BUFFER_SIZE];
          int optMask;
          while (mask) {
            printf("\n\n\n");
            printf("Please choose an option below\n");
            printf("(0: OFF, 1: Low, 2: Mid, 3: High, 9: Quit)\n");
            if (fgets(inpMask, sizeof(inpMask), stdin) != NULL) {
              if (sscanf(inpMask, "%d", &optMask) == 1) {
                if (optMask == 9) {
                  mask = false;
                } else if (optMask >= 0 && optMask <= 3) {
                  hlc_param[5] = 0x0 | optMask;
                }

                write(fd, hlc_param, sizeof(hlc_param));
                read(fd, bufH, sizeof(bufH) - 1);              // read
                memset(bufH, 0, sizeof(bufH));                 // reset buffer
                write(fd, hlc_paramINQ, sizeof(hlc_paramINQ)); // query
                // rdlenH = read(fd,bufH,sizeof(bufH)-1);//read
                read(fd, bufH, sizeof(bufH) - 1); // read
                printf("Current HLC Mask: %X\n", bufH[3]);
                memset(bufH, 0, sizeof(bufH)); // reset buffer
              }
            }
          }
          break;
        case 0:
          printf("Quitting\n");
          hlc = false;
          break;
        }
      }
    }
  }
}

// private stabilizer function
void stabF(int fd) {
  bool stab, mode, levelS;
  stab = true;
  char inpStab[BUFFER_SIZE];
  int optStab;
  unsigned char bufL[BUFFER_SIZE] = {0};
  unsigned char bufM[BUFFER_SIZE] = {0};
  // int rdlenL = 0;
  //  int rdlenM = 0;

  while (stab) {
    memset(bufL, 0, sizeof(bufL));
    memset(bufM, 0, sizeof(bufM));

    write(fd, stab_levelINQ, sizeof(stab_levelINQ)); // query
    //  rdlenL = read(fd,bufL,sizeof(bufL)-1);//read
    read(fd, bufL, sizeof(bufL) - 1);              // read
    write(fd, stab_modeINQ, sizeof(stab_modeINQ)); // query
    //  rdlenM = read(fd,bufM,sizeof(bufM)-1);//read
    read(fd, bufM, sizeof(bufM) - 1); // read
    printf("\n\n\n");
    // Debug: Print raw data
    /*       if(rdlenL > 0){
              printf("Raw data: ");
                    for (int i = 0; i < rdlenL; i++) {
                printf("%X(%d) ", bufL[i], i);
              }
                printf("\n");
            }*/

    printf("Current Level and Mode values\n");
    printf("----------------------------------\n");

    if (bufL[2] == 2) {
      printf("Level: Super\n");
    } else if (bufL[2] == 3) {
      printf("Level: Super+(FHD only)\n");
    }

    if (bufM[2] == 2) {
      printf("Mode: On\n");
    } else if (bufM[2] == 3) {
      printf("Mode: Off\n");
    } else if (bufM[2] == 0) {
      printf("Mode: Hold\n");
    }

    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) Mode\n2) Level\n0) Quit\n");
    if (fgets(inpStab, sizeof(inpStab), stdin) != NULL) {
      if (sscanf(inpStab, "%d", &optStab) == 1) {
        switch (optStab) {
        case 1:

          mode = true;
          char inpMode[BUFFER_SIZE];
          int optMode;

          while (mode) {
            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("1) On\n2) Hold\n3) Off\n0) Quit \n");
            if (fgets(inpMode, sizeof(inpMode), stdin) != NULL) {
              if (sscanf(inpMode, "%d", &optMode) == 1) {
                switch (optMode) {
                case 1:
                  printf("Stabilizer On\n");
                  write(fd, stab_on, sizeof(stab_on));
                  read(fd, bufM, sizeof(bufM) - 1);              // read
                  memset(bufM, 0, sizeof(bufM));                 // reset buffer
                  write(fd, stab_modeINQ, sizeof(stab_modeINQ)); // query
                  // rdlenM = read(fd,bufM,sizeof(bufM)-1);//read
                  read(fd, bufM, sizeof(bufM) - 1); // read
                  break;
                case 2:
                  printf("Stabilizer Hold\n");
                  write(fd, stab_hold, sizeof(stab_hold));
                  read(fd, bufM, sizeof(bufM) - 1);              // read
                  memset(bufM, 0, sizeof(bufM));                 // reset buffer
                  write(fd, stab_modeINQ, sizeof(stab_modeINQ)); // query
                  //	rdlenM = read(fd,bufM,sizeof(bufM)-1);//read
                  read(fd, bufM, sizeof(bufM) - 1); // read
                  break;
                case 3:
                  printf("Stabilizer Off\n");
                  write(fd, stab_off, sizeof(stab_off));
                  read(fd, bufM, sizeof(bufM) - 1);              // read
                  memset(bufM, 0, sizeof(bufM));                 // reset buffer
                  write(fd, stab_modeINQ, sizeof(stab_modeINQ)); // query
                  //	rdlenM = read(fd,bufM,sizeof(bufM)-1);//read
                  read(fd, bufM, sizeof(bufM) - 1); // read
                  break;
                case 0:
                  printf("Quitting\n");
                  mode = false;
                  break;
                }
              }
            }
          }

          break;
        case 2:

          levelS = true;
          char inpLevels[BUFFER_SIZE];
          int optLevels;

          while (levelS) {
            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("2)Super 3)Super+ (FHD only) 0) Quit\n");
            if (fgets(inpLevels, sizeof(inpLevels), stdin) != NULL) {
              if (sscanf(inpLevels, "%d", &optLevels) == 1) {
                if (optLevels == 0) {
                  printf("Quitting\n");
                  levelS = false;
                } else if (optLevels == 2 || optLevels == 3) {
                  stab_level[5] = 0x0 | optLevels;
                }

                write(fd, stab_level, sizeof(stab_level));
                read(fd, bufL, sizeof(bufL) - 1);                // read
                memset(bufL, 0, sizeof(bufL));                   // reset buffer
                write(fd, stab_levelINQ, sizeof(stab_levelINQ)); // query
                // rdlenL = read(fd,bufL,sizeof(bufL)-1);//read
                read(fd, bufL, sizeof(bufL) - 1); // read
              }
            }
          }

          break;
        case 0:
          printf("Quitting\n");
          stab = false;
          break;
        }
      }
    }
  }
}
// private Aperature function
void apeF(int fd) {
  bool ape;
  ape = true;
  char inpApe[BUFFER_SIZE];
  int optApe;
  unsigned char bufA[BUFFER_SIZE] = {0};
  //  int rdlenA = 0;

  while (ape) {
    memset(bufA, 0, sizeof(bufA)); // reset buffer
    write(fd, apINQ,
          sizeof(apINQ)); // query
                          // rdlenA = read(fd,bufA,sizeof(bufA)-1);//read
    read(fd, bufA, sizeof(bufA) - 1); // read

    /*    	  // Debug: Print raw data
   if(rdlenA > 0){
      printf("Raw data: ");
            for (int i = 0; i < rdlenA; i++) {
        printf("%X(%d) ", bufA[i], i);
      }
        printf("\n");
    }*/

    printf("\n\n\n");
    printf("Current Aperture:\n");
    printf("---------------------\n");
    printf("Aperture: %X%X\n", bufA[4], bufA[5]);
    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) Reset\n2) Increment\n3) Decrement\n0) Quit\n");

    if (fgets(inpApe, sizeof(inpApe), stdin) != NULL) {
      if (sscanf(inpApe, "%d", &optApe) == 1) {
        switch (optApe) {
        case 1:
          printf("Resetting\n");
          write(fd, ap_reset, sizeof(ap_reset));
          read(fd, bufA, sizeof(bufA) - 1);
          memset(bufA, 0, sizeof(bufA)); // reset buffer
          write(fd, apINQ, sizeof(apINQ));
          read(fd, bufA, sizeof(bufA) - 1);
          break;
        case 2:
          printf("Incrementing\n");
          write(fd, ap_inc, sizeof(ap_inc));
          read(fd, bufA, sizeof(bufA) - 1);
          memset(bufA, 0, sizeof(bufA)); // reset buffer
          write(fd, apINQ, sizeof(apINQ));
          read(fd, bufA, sizeof(bufA) - 1);
          break;
        case 3:
          printf("Decrementing\n");
          write(fd, ap_dec, sizeof(ap_dec));
          read(fd, bufA, sizeof(bufA) - 1);
          memset(bufA, 0, sizeof(bufA)); // reset buffer
          write(fd, apINQ, sizeof(apINQ));
          read(fd, bufA, sizeof(bufA) - 1);
          break;
        case 0:
          printf("Quitting \n");
          ape = false;
          break;
        }
      }
    }
  }
}

// private noise reduction function
void nrF(int fd) {
  bool nr;
  nr = true;
  char inpNR[BUFFER_SIZE];
  int optNR;
  unsigned char bufN[BUFFER_SIZE] = {0};

  unsigned char bufNI[BUFFER_SIZE] = {0};

  while (nr) {
    write(fd, nr_param, sizeof(nr_param));

    memset(bufN, 0, sizeof(bufN)); // reset buffer

    write(fd, nrINQ, sizeof(nrINQ)); // query

    read(fd, bufN, sizeof(bufN) - 1); // read

    write(fd, nr_ind, sizeof(nr_ind));     // i
    memset(bufNI, 0, sizeof(bufNI));       // reset buffer i
    write(fd, nrindINQ, sizeof(nrindINQ)); // query i
    read(fd, bufNI, sizeof(bufNI) - 1);    // read i

    printf("\n\n\n");
    printf("NR Status:\n");
    printf("------------\n");

    if (bufN[8] == 0) {
      printf("Off\n");
    } else if (bufN[8] >= 1 && bufN[8] <= 5) {
      printf("Level: %X\n", bufN[8]);
    } else if (bufN[8] == 0x7F) {
      printf("Independent\n");
      if (bufNI[8] == 0) {
        printf("2d Off\n");
      } else if (bufNI[8] >= 1 && bufNI[8] <= 5) {
        printf("2d Level: %X\n", bufNI[8]);
      }
      if (bufNI[9] == 0) {
        printf("3d Off\n");
      } else if (bufNI[9] >= 1 && bufNI[9] <= 5) {
        printf("3d Level: %X\n", bufNI[9]);
      }
    }

    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("0) Off\n1) Level 1 ---> 5) Level 5\n6) 2d/3d NR Independant\n9) "
           "Quit\n");
    if (fgets(inpNR, sizeof(inpNR), stdin) != NULL) {
      if (sscanf(inpNR, "%d", &optNR) == 1) {
        if (optNR == 9) {
          printf("Quitting\n");
          nr = false;

        } else if (optNR >= 0 && optNR <= 5) {
          nr_param[4] = 0x0 | optNR;
          write(fd, nr_param, sizeof(nr_param));
          memset(bufN, 0, sizeof(bufN));    // reset buffer
          write(fd, nrINQ, sizeof(nrINQ));  // query
          read(fd, bufN, sizeof(bufN) - 1); // read

        } else if (optNR == 6) {
          nr_param[4] = 0x7F;
          write(fd, nr_param, sizeof(nr_param));
          memset(bufN, 0, sizeof(bufN));    // reset buffer
          write(fd, nrINQ, sizeof(nrINQ));  // query
          read(fd, bufN, sizeof(bufN) - 1); // read

          bool t;
          t = true;
          char inpT[BUFFER_SIZE];
          int optT;
          while (t) {
            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("1) 2d level\n2) 3d level\n0) Quit\n");
            if (fgets(inpT, sizeof(inpT), stdin) != NULL) {
              if (sscanf(inpT, "%d", &optT) == 1) {
                switch (optT) {
                case 1:
                  printf("2d Level\n");
                  bool tl;
                  tl = true;
                  char inpTL[BUFFER_SIZE];
                  int optTL;

                  while (tl) {
                    printf("\n\n\n");
                    printf("Please choose one of the following options\n");
                    printf("1) level 1 ---> 5) level 5\n");
                    printf("0) Quit\n");
                    if (fgets(inpTL, sizeof(inpTL), stdin) != NULL) {
                      if (sscanf(inpTL, "%d", &optTL) == 1) {
                        if (optTL == 0) {
                          printf("Quitting\n");
                          tl = false;
                        } else if (optTL >= 1 && optTL <= 5) {
                          nr_ind[4] = 0x0 | optTL;           // change 4 and 5
                          write(fd, nr_ind, sizeof(nr_ind)); // i
                          memset(bufNI, 0, sizeof(bufNI));   // reset buffer i
                          write(fd, nrindINQ, sizeof(nrindINQ)); // query i
                          read(fd, bufNI, sizeof(bufNI) - 1);    // read i
                        }
                      }
                    }
                  }
                  break;
                case 2:
                  printf("3d Level\n");
                  bool thl;
                  thl = true;
                  char inpTHL[BUFFER_SIZE];
                  int optTHL;

                  while (thl) {
                    printf("\n\n\n");
                    printf("Please choose one of the following options\n");
                    printf("1) level 1 ---> 5) level 5\n");
                    printf("0) Quit\n");
                    if (fgets(inpTHL, sizeof(inpTHL), stdin) != NULL) {
                      if (sscanf(inpTHL, "%d", &optTHL) == 1) {
                        if (optTHL == 0) {
                          printf("Quitting\n");
                          thl = false;
                        } else if (optTHL >= 1 && optTHL <= 5) {
                          nr_ind[5] = 0x0 | optTHL;          // change 4 and 5
                          write(fd, nr_ind, sizeof(nr_ind)); // i
                          memset(bufNI, 0, sizeof(bufNI));   // reset buffer i
                          write(fd, nrindINQ, sizeof(nrindINQ)); // query i
                          read(fd, bufNI, sizeof(bufNI) - 1);    // read i
                        }
                      }
                    }
                  }
                  break;
                case 0:
                  printf("Quitting\n");
                  t = false;
                  break;

                  // nr_ind[] = {0x81,0x01,0x05,0x53,0x00,0x00,0xFF};
                }
              }
            }
          }
        }
      }
    }
  }
}

// private gamma function
void gammaF(int fd) {
  bool gamma;
  gamma = true;
  char inpGam[BUFFER_SIZE];
  int optGam;

  unsigned char bufGO[BUFFER_SIZE] = {0};
  // int rdlenGO = 0;

  while (gamma) {

    // Write and read the initial gamma offset
    write(fd, gamma_offset, sizeof(gamma_offset));       // Write gamma offset
    read(fd, bufGO, sizeof(bufGO));                      // Read response
    memset(bufGO, 0, sizeof(bufGO));                     // Reset buffer
    write(fd, gamma_offsetINQ, sizeof(gamma_offsetINQ)); // Query device
    read(fd, bufGO, sizeof(bufGO));                      // Read response
    if (bufGO[5] == 0x00) {
      printf("Current Offset: %X%X\n", bufGO[6], bufGO[7]);
    } else if (bufGO[5] == 0x01) {
      printf("Current Offset: -%X%X\n", bufGO[6], bufGO[7]);
    }

    printf("\n\n\n");
    printf("Please choose on of the following options\n");
    printf("0) Standard\n1) Straight\n3) Offset\n9) Quit\n");
    if (fgets(inpGam, sizeof(inpGam), stdin) != NULL) {
      if (sscanf(inpGam, "%d", &optGam) == 1) {
        if (optGam == 9) {
          printf("Quitting\n");
          gamma = false;
        } else if (optGam == 0 || optGam == 1) {
          gamma_param[4] = 0x0 | optGam;
          write(fd, gamma_param, sizeof(gamma_param));
          // clear
          read(fd, bufGO, sizeof(bufGO));
          memset(bufGO, 0, sizeof(bufGO));

        } else if (optGam == 3) {
          bool offset;
          offset = true;
          char inpOff[BUFFER_SIZE];
          int optOff;
          while (offset) {

            printf("\n\n\n");
            printf("Please choose one of the following options\n");
            printf("0) Postitive Polarity\n1) Negative Polarity\n2) Increment "
                   "offset\n3) Decrement Offset\n9) Quit\n");
            if (fgets(inpOff, sizeof(inpOff), stdin) != NULL) {
              if (sscanf(inpOff, "%d", &optOff) == 1) {
                if (optOff == 9) {
                  printf("Quitting\n");
                  offset = false;
                } else if (optOff == 0 || optOff == 1) {
                  gamma_offset[7] = 0x0 | optOff;
                  write(fd, gamma_offset, sizeof(gamma_offset)); // i
                  read(fd, bufGO, sizeof(bufGO) - 1);            // read i
                  memset(bufGO, 0, sizeof(bufGO)); // reset buffer i
                  write(fd, gamma_offsetINQ, sizeof(gamma_offsetINQ)); // query
                                                                       // i
                  read(fd, bufGO, sizeof(bufGO) - 1); // read i
                  if (bufGO[5] == 0x00) {
                    printf("Offset : %X%X\n", bufGO[6], bufGO[7]);
                  } else if (bufGO[5] == 0x01) {
                    printf("Offset : -%X%X\n", bufGO[6], bufGO[7]);
                  }
                } else if (optOff == 2) {
                  if (gamma_offset[8] == 4 && gamma_offset[9] == 0) {
                    printf("Gamma Offset is already at maximum value\n");
                  } else {
                    if (gamma_offset[9] == 0x0F) {
                      gamma_offset[9] = 0x00;
                      gamma_offset[8]++;

                    } else {
                      gamma_offset[9]++;
                    }
                  }
                  write(fd, gamma_offset, sizeof(gamma_offset));
                  read(fd, bufGO, sizeof(bufGO) - 1); // read i
                  memset(bufGO, 0, sizeof(bufGO));    // reset buffer i
                  write(fd, gamma_offsetINQ, sizeof(gamma_offsetINQ)); // query
                                                                       // i
                  read(fd, bufGO, sizeof(bufGO) - 1); // read i
                  if (bufGO[5] == 0x00) {
                    printf("Offset incremented to: %X%X\n", bufGO[6], bufGO[7]);
                  } else if (bufGO[5] == 0x01) {
                    printf("Offset incremented to: -%X%X\n", bufGO[6],
                           bufGO[7]);
                  }
                } else if (optOff == 3) {
                  // Decrement offset
                  if (gamma_offset[8] == 0x00 && gamma_offset[9] == 0x00) {
                    printf("Offset is already at minimum value.\n");

                  } else {
                    if (gamma_offset[9] == 0x00) {
                      gamma_offset[9] = 0x0F;
                      gamma_offset[8]--;
                    } else {
                      gamma_offset[9]--;
                    }
                    write(fd, gamma_offset, sizeof(gamma_offset));

                    read(fd, bufGO, sizeof(bufGO) - 1); // read i
                    memset(bufGO, 0, sizeof(bufGO));    // reset buffer i
                    write(fd, gamma_offsetINQ,
                          sizeof(gamma_offsetINQ));     // query i
                    read(fd, bufGO, sizeof(bufGO) - 1); // read i
                    if (bufGO[5] == 0x00) {
                      printf("Offset decremented to: %X%X\n", bufGO[6],
                             bufGO[7]);
                    } else if (bufGO[5] == 0x01) {
                      printf("Offset decremented to: -%X%X\n", bufGO[6],
                             bufGO[7]);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

// Sub Menu IQ function called in main/menu
void iq(int fd) {

  bool iq;
  iq = true;
  char inputIQ[BUFFER_SIZE];
  int optIQ;

  while (iq) {
    printf("\n\n\n");
    printf("Please choose one of the following options\n");
    printf("1) WideD/VE\n2) Defog \n3) ICR \n4) HLC\n5) Stabilizer\n6) "
           "Aperture\n7) Noise Reduction\n8) Gamma\n0) Quit\n");
    if (fgets(inputIQ, sizeof(inputIQ), stdin) != NULL) {
      if (sscanf(inputIQ, "%d", &optIQ) == 1) {
        switch (optIQ) {
        case 1:
          wideF(fd);
          break;
        case 2:
          defogF(fd);

          break;
        case 3:
          icrF(fd);

          break;
        case 4:
          hlcF(fd);

          break;
        case 5:
          stabF(fd);

          break;
        case 6:
          apeF(fd);
          break;
        case 7:
          nrF(fd);

          break;
        case 8:
          gammaF(fd);

          break;

        case 0:
          printf("Quitting\n");
          iq = false;
          break;
        }
      }
    }
  }
}

