#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
int common =1;

struct Server {
  char ip[255];
  int port;
};

struct AllData
{
    uint64_t begin;
    uint64_t end;
    uint64_t mod;
    int *c;
    char ip[255];
    uint64_t port;
};

uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);       //Convert the string to a value of type unsigned long int
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

void ServerHanddler(void* data)
{
    
    struct AllData d=*((struct AllData*)data);
    int f;

    //пока копирую из примера
    struct hostent *hostname = gethostbyname(d.ip);   //returns a pointer to the hostent structure described above.
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n",d.ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(d.port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }
    char task[sizeof(uint64_t) * 3];
    printf("begin = %llu, end = %llu, mod = %llu\n",d.begin, d.end, d.mod);

    /*char value[sizeof(uint64_t*)];
    snprintf(value, sizeof value, "%d", d.begin);
    write(STDOUT_FILENO, value, strlen(value));*/

    memcpy(task, &(d.begin), sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &(d.end), sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &(d.mod), sizeof(uint64_t));
    //printf("begin = %d, end = %d, mod = %d\n",d.begin, d.end, d.mod);
    //printf("%s",task);

    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
    printf("for calculation from %d to %d recieved result is: %llu\n", d.begin, d.end-1,answer);
    //-------------------------
    pthread_mutex_lock(&mut);
    f=*(d.c);
    f=MultModulo(f,answer,d.mod);
    *(d.c) = f;
     printf("for calculation from %d to %d afer multiplication: %d\n", d.begin, d.end-1,*(d.c));
    pthread_mutex_unlock(&mut);
    
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
            if (k <= 0) {
                printf("k is a positive number\n");
                return 1;
            }
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
            if (mod <= 0) {
                printf("mod is a positive number\n");
                return 1;
            }
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  /*unsigned int servers_num = 1;
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  // TODO: delete this and parallel work between servers
  to[0].port = 20001;
  memcpy(to[0].ip, "127.0.0.1", sizeof("127.0.0.1"));   //функция стандартной библиотеки языка программирования Си, 
                                                        //копирующая содержимое одной области памяти в другую.

  // TODO: work continiously, rewrite to make parallel
  for (int i = 0; i < servers_num; i++) {
    struct hostent *hostname = gethostbyname(to[i].ip);   //returns a pointer to the hostent structure described above.
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to[i].port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck, (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    uint64_t begin = 1;
    uint64_t end = k;

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck, task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }

    char response[sizeof(uint64_t)];
    if (recv(sck, response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Recieve failed\n");
      exit(1);
    }

    // TODO: from one server
    // unite results
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
    printf("answer: %llu\n", answer);

    close(sck);
  }*/

  //обработка файла (считаем ко-во сток, создаем массив серверов, записываем в него содержание строк)
  char buffer[255];
  int servers_num=0;
  FILE *fp;
  fp=fopen(servers, "r");
  while (fgets(buffer, sizeof(buffer), fp))
  {
      servers_num++;
  }
  fclose(fp);

  struct AllData* to = malloc(sizeof(struct AllData) * servers_num);
  fp=fopen(servers, "r");
  int it =0;
  while (fgets(buffer, sizeof(buffer), fp))
  {
    ConvertStringToUI64(buffer,&(to[it].port));
    memcpy(to[it].ip,"127.0.0.1", sizeof("127.0.0.1"));
    it++;
  }
  fclose(fp);

//создаю массив тредов, потом запускаю каждый из них для функции,
// которая обрабатывает определенный численный интервал на определенном сервере
if (servers_num>k)
    servers_num=k;
int sizeforthread = servers_num <= k ? (k / servers_num) : 1;
  pthread_t threads[servers_num];
  for (uint32_t i = 0; i < servers_num; i++) {
    to[i].begin = i * sizeforthread + 1;
    printf("First element in thred %d: %d\n", i, to[i].begin);
    if (i != (servers_num - 1))
    {
        to[i].end = to[i].begin + sizeforthread;
    }
    else {
        to[i].end = k + 1;
    }
    printf("Last element in thred %d: %d\n", i,to[i].end - 1);
    to[i].mod = mod;
    to[i].c=&common;
    if (pthread_create(&threads[i], NULL, (void*)ServerHanddler, (void*)&to[i])) {
        printf("Error: pthread_create failed!\n");
        return 1;
    }
  }
  for (uint32_t i = 0; i < servers_num; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
    perror("pthread_join");
    exit(1);
    }
  }
  free(to);
  printf("All done, counter = %d\n", common);
  return 0;
}
