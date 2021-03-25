#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <unistd.h>

struct parameters 
{
    int begin;
    int end;
    int p;
};



pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
int common =0;

void calculation(void* param)
{
    //считает для всех элементов начиная с начала и до конца, не включая его
    //https://www.cyberforum.ru/cpp-beginners/thread2043433.html
    struct parameters* params;
    int f;
    params = (struct parameters*)param;
    pthread_mutex_lock(&mut);
    f=1;
    int n=(*(params)).end-1;
    for(; n>=(*(params)).begin;n--)
    {
        f=(f*n)%(*(params)).p;
    }
    pthread_mutex_unlock(&mut);
};


int main(int argc, char **argv) {
/*-----------------Reading the parameters-----------------------------------------*/
  uint32_t k = -1;
  uint32_t p_num = -1;
  uint32_t mod = -1;

  int current_optind = optind ? optind : 1;
   while (1) {
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"p_num", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index); 

    if (c == -1) break;   
    switch (c) {
      case 0:        
        switch (option_index) {    
          case 0:
            k = atoi(optarg);     
            if (k <= 0) {
                printf("k is a positive number\n");
                return 1;
            }
            break;
          case 1:
            p_num = atoi(optarg);
            if (p_num <= 0) {
                printf("seed is a positive number\n");
                 return 1;
            }
            break;
          case 2:
            mod = atoi(optarg);        
            if (mod <= 0) { 
                printf("mod is a positive number\n");
                return 1;
            }
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case '?':       
        break; 
      default:
        printf("getopt returned character code 0%o?\n", c);
    }
   }

  if (k == -1 || p_num == -1 || mod == -1) {
    printf("Usage: %s --k \"num\" --p_num \"num\" --mod \"num\" \n", argv[0]);
    return 1;
  }
/*------------Working out the algorithm-----------------------------------------------
получается чтобы посчитать факториал по частям, нужно разделить наш массив чисел 
от 1 до n разделить на более-менее равные части */
int sizeforthread = p_num<=k ? (k/p_num) : 1;  //находим размер одного подмножества
struct parameters params[p_num];

//СТРУКТУРА В СТРУКТУРЕ??? ГДЕ МАССИВ ПАРАМЕТРОВ И common!!!!!!!!!!!!!

/*------------Working with threads---------------------------------------------------*/
//https://stackoverflow.com/questions/266168/simple-example-of-threading-in-c
 pthread_t threads[p_num];
 for (uint32_t i = 1; i <= p_num; i++) {
      params[i].begin = i * sizeforthread;
      if (i!=(p_num-1))
      {
          params[i].end = params[i].begin + sizeforthread;          
      }
      else{
          params[i].end = k;
      }
      params[i].p=mod;
      if (pthread_create(&threads[i], NULL, calculation, (void*)&params[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
      }
  }


return 0;
}