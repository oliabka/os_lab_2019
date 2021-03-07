#include <stdio.h>
#include <stdlib.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  if (argc != 3) {     //checking if 2 arguments were given 
    printf("Usage: %s seed arraysize\n", argv[0]);
    return 1;
  }

  int seed = atoi(argv[1]); //first argument given
  if (seed <= 0) {
    printf("seed is a positive number\n");
    return 1;
  }

  int array_size = atoi(argv[2]);  //second argument given
  if (array_size <= 0) {
    printf("array_size is a positive number\n");
    return 1;
  }

  int *array = malloc(array_size * sizeof(int));
  GenerateArray(array, array_size, seed);
  int i;
  for (i = 0; i < array_size; i++)   
  {
    printf("%d ", array[i]);
  }
  printf("\n");
  struct MinMax min_max = GetMinMax(array, 0, array_size);
  free(array);

  printf("min: %d\n", min_max.min);
  printf("max: %d\n", min_max.max);

  return 0;
}
