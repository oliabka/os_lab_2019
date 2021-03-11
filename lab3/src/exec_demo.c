#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
                                        // https://askdev.ru/q/ya-ne-ponimayu-kak-execlp-rabotaet-v-linux-120427/                                                  
int main(int argc, char* argv[])                          // https://studfile.net/preview/2774686/page:18/
{   
	int pid = fork();
    if (pid<0)
    {
        printf("An error has occured\n");
    }
	if ( pid == 0 ) {                                
        execv("sequential_min_max", argv);
    }
    else{
        wait(NULL);
        printf( "Finished executing the parent process\n");
    }
    return 0;
}