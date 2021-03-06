#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);  // https://habr.com/ru/post/55665/

    if (c == -1) break;   //короче эта штука считывает введеннные пользователем параметры и в зависимости от введенных символов
                          // определяет как работать с функцией 

    switch (c) {
      case 0:         // если getopt_long() вернул 0, то работаем с fork() (я так поняла, это если мы не ввели f и не вызвали ошибку)
        switch (option_index) {    //проходимся по списку options и для первых трех считываем значения обязательных аргументов
          case 0:
            seed = atoi(optarg);     // atoi() используется для приведения (конвертации) строки в числовой вид
            // your code here
            // error handling
            break;
          case 1:
            array_size = atoi(optarg);
            // your code here
            // error handling
            break;
          case 2:
            pnum = atoi(optarg);
            // your code here
            // error handling
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':          //если ввели f, работаем с файлами
        with_files = true;
        break;

      case '?':          //если ошибка, выводим код считанного ошибочного символа
        break; 

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  // проверка полученных данных для создания массива
  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }
  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  //формирование массива
  int *array = malloc(sizeof(int) * array_size); 
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  //переменная времени
  struct timeval start_time;
  gettimeofday(&start_time, NULL);

 //подключение многопоточности
  for (int i = 0; i < pnum; i++) {
    pid_t child_pid = fork();      // https://www.youtube.com/watch?v=9seb8hddeK4
    if (child_pid >= 0) {
      // successful fork 
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process

        // parallel somehow

        if (with_files) {          //если работаем с файлами
          // use files here
        } else {
          // use pipe here
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {            // пока есть активные процессы-дети, для каждого что-то выполдняем  и он 
    // your code here                             //заканчивает работу, после чего учитываем это в числе активных детей

    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      // read from files
    } else {
      // read from pipes
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

   //структура времени завершения
  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);
  // подсчет прошедшего времени
  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  //очистка массива и вывод данных
  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}
