#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <getopt.h>


//#define BUFSIZE 100
#define SADDR struct sockaddr
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
    int BUFSIZE = -1;
    while (1){
        int current_optind = optind ? optind : 1;

        static struct option options[] = {{"BUFSIZE", required_argument, 0, 0},
                                        {0, 0, 0, 0}};

        int option_index = 0;
        int c = getopt_long(argc, argv, "", options, &option_index);

        if (c == -1)
        break;

        switch (c) {
            case 0: {
            switch (option_index) {
            case 0:
                BUFSIZE = atoi(optarg);
                if (BUFSIZE <= 0) {
                    printf("port is a positive number\n");
                    return 1;
                }
                break;
            default:
                printf("Index %d is out of options\n", option_index);
            }
        } break;

        case '?':
        printf("Unknown argument\n");
        break;
        default:
        fprintf(stderr, "getopt returned character code 0%o?\n", c);
        }
    } 

  if (BUFSIZE == -1) {
    fprintf(stderr, "Using: %s ip_adress port --BUFSIZE 100\n", argv[0]);
    return 1;
  }
  int fd;
  int nread;
  char buf[BUFSIZE];
  struct sockaddr_in servaddr;
  printf("argc = %d\n",argc);
  if (argc < 5) {
    printf("Too few arguments \n");
    exit(1);
  }

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket creating");
    exit(1);
  }

  memset(&servaddr, 0, SIZE);
  servaddr.sin_family = AF_INET;

  printf("argv[1] = %s\n",argv[1]);
  printf("argv[2] = %s\n",argv[2]);
  printf("argv[3] = %s\n",argv[3]);
  printf("argv[4] = %s\n",argv[4]);
  if (inet_pton(AF_INET, argv[3], &servaddr.sin_addr) <= 0) {
    perror("bad address");
    exit(1);
  }

  servaddr.sin_port = htons(atoi(argv[4]));

  if (connect(fd, (SADDR *)&servaddr, SIZE) < 0) {
    perror("connect");
    exit(1);
  }

  write(1, "Input message to send\n", 22);
  while ((nread = read(0, buf, BUFSIZE)) > 0) {
    if (write(fd, buf, nread) < 0) {
      perror("write");
      exit(1);
    }
  }

  close(fd);
  exit(0);
}
