

/*----------------------------------------

Main Program that controls the main menu

-------------------------------------------*/

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

char *port_name;

// Function to check if a directory entry represents a USB serial device
int is_serial_device(const char *name) {
  return strncmp(name, "ttyUSB", 6) == 0;
}

// Function to search for /dev/ttyUSB devices and print them
void find_serial_ports() {
  DIR *dir;
  struct dirent *entry;

  // Open the directory containing serial device files
  dir = opendir("/dev");
  if (dir == NULL) {
    perror("opendir");
    return;
  }

  // Iterate over directory entries
  while ((entry = readdir(dir)) != NULL) {
    if (is_serial_device(entry->d_name)) {
      printf("/dev/%s\n", entry->d_name);
      port_name = entry->d_name;
    }
  }

  // Close the  directory
  closedir(dir);
}

int set_interface_attribs(int fd, int speed) {
  struct termios tty;

  if (tcgetattr(fd, &tty) < 0) {
    printf("Error from tcgetattr: %s\n", strerror(errno));
    return -1;
  }

  cfsetospeed(&tty, (speed_t)speed);
  cfsetispeed(&tty, (speed_t)speed);

  tty.c_cflag |= (CLOCAL | CREAD); /* ignore modem controls */
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;      /* 8-bit characters */
  tty.c_cflag &= ~PARENB;  /* no parity bit */
  tty.c_cflag &= ~CSTOPB;  /* only need 1 stop bit */
  tty.c_cflag &= ~CRTSCTS; /* no hardware flowcontrol */

  /* setup for non-canonical mode */
  tty.c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tty.c_oflag &= ~OPOST;

  /* fetch bytes as they become available */
  tty.c_cc[VMIN] = 10;
  tty.c_cc[VTIME] = 1;

  if ((tcsetattr(fd, TCSANOW, &tty) != 0) |
      (tcsetattr(fd, TCSAFLUSH, &tty) != 0)) {
    printf("Error from tcsetattr: %s\n", strerror(errno));
    return -1;
  }

  return 0;
}

int CheckHexString(char *Str) {
  const char *command = Str;

  while (*command != 0) {
    if (('A' <= *command && *command <= 'F') ||
        ('a' <= *command && *command <= 'f') ||
        ('0' <= *command && *command <= '9')) {
      ++command;
    } else {
      return 0;
    }
  }
  return 1;
}

/*
        convert hexstring to len bytes of data
        returns 0 on success, -1 on error
        data is a buffer of at least len bytes
        hexstring is upper or lower case hexadecimal, NOT prepended with "0x"
*/
int BytesFromString(unsigned char *data, const char *string, unsigned int len) {
  const char *pos = string;
  char *endptr;
  size_t count = 0;

  printf("Command:%s\n", string);
  if ((string[0] == '\0') || (strlen(string) % 2)) {
    // hexstring contains no data
    // or hexstring has an odd length
    return 1;
  }

  for (count = 0; count < len; count++) {
    char buf[5] = {'0', 'x', pos[0], pos[1], 0};
    data[count] = strtol(buf, &endptr, 0);
    pos += 2 * sizeof(char);

    if (endptr[0] != '\0') {
      // non-hexadecimal character encountered
      return 1;
    }
  }

  return 0;
}

int ChangeDevicePermission(const char *port) {
  char command[50];

  // Construct the sudo command to change permissions
  snprintf(command, sizeof(command), "sudo chmod 666 /dev/%s", port);

  // Execute the sudo command
  int status = system(command);
  if (status == -1) {
    perror("system");
    return 1;
  } else if (WIFEXITED(status)) {
    int exit_status = WEXITSTATUS(status);
    if (exit_status != 0) {
      fprintf(stderr, "Command failed with exit status %d\n", exit_status);
      return 1;
    }
  } else {
    fprintf(stderr, "Command did not terminate normally\n");
    return 1;
  }

  printf("Permissions of /dev/%s changed successfully.\n", port);

  return 0;
}

int main(int argc, char *argv[]) {
  int fd;
  char device_path[20];

  /*       	  if(argc != 2)
          {
                  printf("Usage: %s <command>\n", argv[0]);
                  printf("	<command example>: 8101043802ff\n");
                  return 0;
          }

  */
  // printf("Serial ports:\n");
  find_serial_ports();

  // Ensure that port_name has been set
  if (port_name == NULL) {
    printf("No serial port found.\n");
    return 1;
  }

  ChangeDevicePermission(port_name);

  snprintf(device_path, sizeof(device_path), "/dev/%s", port_name);

  if (chmod(device_path,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) == 0) {
    printf("Permission for %s changed successfully.\n", device_path);
  }

  fd = open(device_path, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    // printf("Error opening %s: %s\n", portname, strerror(errno));
    printf("Error opening %s: %s\n", device_path, strerror(errno));
    return 1;
  }

  /*baudrate 9600, 8 bits, no parity, 1 stop bit */
  set_interface_attribs(fd, B9600);

  /*	int len = strlen(argv[1]);
          unsigned char variable[len];

          if(CheckHexString(argv[1]) == 0){
                  printf("Please input HEX command\n");
                  return 1;
          }

          BytesFromString(variable, argv[1], len);

          if (((variable[0] >>4) ==0x8) && (variable[(len/2)-1] ==0xff)){

                  for(int i = 0; i < len; i++){
                          len = write(fd, variable, len);
                  }

                  if (len != sizeof(variable))
                  printf("Error from write: %d, %d\n", len, errno);

          }
          else{
                  printf("Please input an appropriate command\n");
                  return 1;
          }
          */
  tcdrain(fd); /* delay for output */

  // menu mode where user can choose different features
  char input[100];
  int option;

  while (1) {

    camera_information(fd);

    printf("\n");
    printf("Please select 1 of the following options\n");
    printf("1) Camera\n2) Lens\n3) EXP/WB\n4) Spot Focus\n5) Motion "
           "Detection\n6) IQ\n7) Extend\n0) Quit\n");

    // Read the user input
    if (fgets(input, sizeof(input), stdin) != NULL) {
      // Parse the input
      if (sscanf(input, "%d", &option) == 1) {
        // Handle the options
        switch (option) {
          // FCB 9500 Series Controller Implementation
        case 1:

          camera(fd);
          break;
        case 2:
          lens(fd);
          break;
        case 3:
          expwb(fd);
          break;
        case 4:
          spotFocus(fd);
          break;
        case 5:
          motionDetect(fd);
          break;
        case 6:
          iq(fd);
          break;
        case 7:
          extend(fd);
          break;
        case 0: // final quit
          printf("Quitting\n");
          write(fd, reset, sizeof(reset));
          return 0;
        }
      }
    }
  }

  /*     unsigned char buf[256];
       int rdlen, cnt=0;

       rdlen = read(fd, buf, sizeof(buf) - 1);

       if (rdlen > 0) {

               unsigned char   *p;

               printf("Inquiry:");
               for (p = buf; rdlen-- > 0; p++){
               printf("%x", *p);
                       if (*p==0xff){
                               printf("  ");
                               cnt++;
                               if((variable[1]==1) && (cnt==2)){  //Command
                                       break;
                               }
                               else if((variable[1]==9) && (cnt==1)){//Inquiry
                                       break;
                               }

                       }

               }
               printf("\n");
       } else if (rdlen < 0) {
           printf("Error from read: %d: %s\n", rdlen, strerror(errno));
       } else {  // rdlen == 0
           printf("Timeout from read\n");
       }
       */
  fflush(stdin);
  close(fd);
}

