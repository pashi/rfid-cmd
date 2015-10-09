#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>

#include<arpa/inet.h>
#include<sys/socket.h>


#define ON_KEY_PRESS 1
#define ON_KEY_RELEASE 0
#define BUFLEN 512
#define SERVER "127.0.0.1"
#define PORT 8888

static int keepRunning = 1;

void handler (int sig)
{
  printf ("\nexiting...(%d)n", sig);
  keepRunning = 0;
}
 
void perror_exit (char *error)
{
  perror (error);
  handler (9);
}

int ourkey(int n) {
	int ret = -1;
	if (n > 1) {
		if (n<11)
			ret=(n-1);
		if (n == 11)
			ret=0;
	}
	return ret;
}
 
int main (int argc, char *argv[])
{
  struct input_event event;
  int fd; 

  char name[256] = "Unknown";
  char *device = NULL;
  int counter;
 
  //Setup check
  if (argv[1] == NULL){
      printf("Please specify (on the command line) the path to the dev event interface device\n");
      exit (0);
    }
 
  if (argc > 1)
    device = argv[1];


  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  char message[BUFLEN];
 
  if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror_exit("socket");
  }

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
     
  if (inet_aton(SERVER , &si_other.sin_addr) == 0)
  {
        perror_exit("inet_aton() failed");
  }
 
  signal(SIGINT, handler);

  //Open Device
  if ((fd = open (device, O_RDONLY)) == -1)
    printf ("%s is not a vaild device.\n", device);
 
  ioctl (fd, EVIOCGNAME (sizeof (name)), name);
  printf ("Reading From : %s (%s)\n", device, name);
 
  // only we are allow to read this device
  ioctl (fd, EVIOCGRAB, 1);

  counter=0;
  int code[10];
  char x[10];
  //count = 
  while (read(fd, &event, sizeof(struct input_event)) > 0 && keepRunning) {
    if (event.type != EV_KEY) continue;
    if (event.value == ON_KEY_PRESS) {
       if (event.code == KEY_ENTER) {
                int i;
		printf("ENTER\n");
                for (i=0; i<10; i++) {
                  printf ("%d", code[i]);
                }
                printf ("\n");

                sendto(s, code, sizeof(code) , 0 , (struct sockaddr *) &si_other, slen);

		counter=0;
		memset(&code,0,10);
       }
       int key = ourkey(event.code);
       if (key>-1 && key < 10 && counter<10) {
           code[counter] = key;
	   counter++;
       }

    } 
  }
  printf ("Exit..\n");
  close(s);
  close(fd);
  return 0;
} 
