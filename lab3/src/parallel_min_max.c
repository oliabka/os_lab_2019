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
        //printf("started case c-0\n");
        switch (option_index) {    //проходимся по списку options и для первых трех считываем значения обязательных аргументов
          case 0:
            //printf("startted case 0\n");
            seed = atoi(optarg);     // atoi() используется для приведения (конвертации) строки в числовой вид
            // your code here
            // error handling
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            break;
          case 1:
            //printf("startted case 1\n");
            array_size = atoi(optarg);
            // your code here
            // error handling
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                 return 1;
            }
            break;
          case 2:
            //printf("startted case 2\n");
            pnum = atoi(optarg);        //кол-во потоков
            // your code here
            // error handling
            if (pnum <= 0) { 
                printf("pnum is a positive number\n");
                return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        printf("Working with files: ON\n");       //если ввели f, работаем с файлами
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
  int i;
  for (i = 0; i < array_size; i++)   //выводим массив
  {
    printf("%d ", array[i]);
  }
  printf("\n");
  int sub_array_size = array_size / pnum;  //находим длинну отрезка массива, который будем проходить в каждом процессе

  int active_child_processes = 0;

  //переменная времени начала
  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  //создаем массив "ключей" для труб 
  int *array_of_pipes_read = malloc(sizeof(int) * pnum);

 //подключение многопоточности
  for (int i = 0; i < pnum; i++) {
    int my_pipe[2];                     //создаем "ключ" для трубы и проверяем на ошибки при создании
    if (!with_files){                   
        if (pipe(my_pipe)==-1){
            printf("An error occured when opening the pipe\n");
            return 2;
        }
    }
    pid_t child_pid = fork();      // https://www.youtube.com/watch?v=9seb8hddeK4
    if (child_pid >= 0) {
      // successful fork 
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        struct MinMax local_min_max;
        // parallel somehow
        if (i!=(pnum-1))                    //создаем структуру с локальным максимумом и минимумом для кажного отрезка
        {
            local_min_max = GetMinMax(array, i * sub_array_size, (i+1) * sub_array_size); 
        }
        else local_min_max = GetMinMax(array, i * sub_array_size, array_size);

        if (with_files) {          //если работаем с файлами
          // use files here
          char* str = (char*)malloc(16*sizeof(char));
          sprintf(str, "data_%d.txt", i);
          FILE * fp = fopen (str, "a");         //создаем очередной файл
          if (fp==0)
            {
                printf( "Could not open file\n" );
                return -1;
            }
            else
            {
                 fwrite(&local_min_max, sizeof(struct MinMax), 1, fp);       //записываем структуру в файл
            }
        } else {                   // https://www.youtube.com/watch?v=Mqb2dVRe0uo
          // use pipe here
            close(my_pipe[0]);    //закрыли конец трубы для чтения
            write(my_pipe[1],&local_min_max,sizeof(struct MinMax));  //засовываем число в трубу
            close(my_pipe[1]);    //закрыли другой конец
        }
        return 0;
      }
      else{                 //процесс-родитель
        array_of_pipes_read[i] = my_pipe[0];    //сохраняем данные о трубе для данного процесса
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {            // пока есть активные процессы-дети, для каждого что-то выполняем  и он 
    // your code here                             //заканчивает работу, после чего учитываем это в числе активных детей
    wait(NULL);               //ждем окончания выполнения процесса-ребенка
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {      //ищем максимальный и минимальный элемент
    struct MinMax process_min_max;
    process_min_max.min = INT_MAX;
    process_min_max.max = INT_MIN;

    if (with_files) {
      // read from files
        char* str = (char*)malloc(16*sizeof(char));      
        sprintf(str, "data_%d.txt", i);
        FILE * fp = fopen (str, "r");          //открываем очередной файл
        if (fp==0){
            printf( "Could not open file\n" );
            return 1;
        }
        else
        {
            fread(&process_min_max, sizeof(struct MinMax), 1, fp);   //считываем структуру из файла
        }
    } else {
      // read from pipes                //считываем локальный минимум и максимум из трубы для сравнения
        read(array_of_pipes_read[i],&process_min_max,sizeof(struct MinMax));
        close(array_of_pipes_read[i]);
    }

    if (process_min_max.min < min_max.min) min_max.min = process_min_max.min;
    if (process_min_max.max > min_max.max) min_max.max = process_min_max.max;
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
