
/* Program to display address information about the process */
/* Adapted from Gray, J., program 1.4 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/* Below is a macro definition */
#define SHW_ADR(ID, I) (printf("ID %s \t is at virtual address: %8X\n", ID, &I))   //выводит что где лежит; название, а потом место

extern int etext, edata, end; /* Global variables for process
                                 memory */

char *cptr = "This message is output by the function showit()\n"; /* Static */
char buffer1[25];
int showit(); /* Function prototype */

main() {
  int i = 0; /* Automatic variable */

  /* Printing addressing information */
  printf("\nAddress etext: %8X \n", &etext); //This is the first address past the end of the text segment (the program code).
  printf("Address edata: %8X \n", &edata);  //This is the first address past the end of the initialized data segment.
  printf("Address end  : %8X \n", &end);   //This is the first address past the end of the uninitialized data segment.

  SHW_ADR("main", main);         //где лежит функуия main
  SHW_ADR("showit", showit);     //где лежит функия showit
  SHW_ADR("cptr", cptr);         //где лежит статический массив символов 
  SHW_ADR("buffer1", buffer1);   //где лежит динамический массив символов
  SHW_ADR("i", i);               //где ледит int переменная
  strcpy(buffer1, "A demonstration\n");   /* Library function */     //кладем в буффер символы "A demonstration\n"
  write(1, buffer1, strlen(buffer1) + 1); /* System call */          //записывает в standard output содержание буфффера
  showit(cptr);    

} /* end of main function */

/* A function follows */
int showit(p) char *p;
{
  char *buffer2;
  SHW_ADR("buffer2", buffer2);
  if ((buffer2 = (char *)malloc((unsigned)(strlen(p) + 1))) != NULL) {
    printf("Alocated memory at %X\n", buffer2);
    strcpy(buffer2, p);    /* copy the string */
    printf("%s", buffer2); /* Didplay the string */
    free(buffer2);         /* Release location */
  } else {
    printf("Allocation error\n");
    exit(1);
  }
}
