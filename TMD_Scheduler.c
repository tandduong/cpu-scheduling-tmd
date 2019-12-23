#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Error: No file name given on command line.\n");
        exit(0);
    }
    else if(argc > 2){
        printf("Error: The only command line arguments should be the executable and the file name.\n");
        exit(0);
    }

    int selection = 0;
    pid_t pid = 0;

    printf("Please enter the number of the algorithm you would like to use:\n");
    printf("----------------------------------\n");
    printf("First Come First Served        (1)\n");
    printf("----------------------------------\n");
    printf("Shortest Job Remaining         (2)\n");
    printf("----------------------------------\n");
    printf("Shortest Time Remaining First  (3)\n");
    printf("----------------------------------\n");
    printf("Round Robin                    (4)\n");
    printf("----------------------------------\n");

    printf("\nEnter:");
    scanf("%d", &selection);

    pid = fork();
    if(pid == 0){
        switch(selection){
            case 1:
                execlp("./fcfs.exe", "fcfs", argv[1], NULL);
                break;
            case 2:
                execlp("./sjf.exe", "sjf", argv[1], NULL);
                break;
            case 3:
                execlp("./strf.exe", "strf", argv[1], NULL);
                break;
            case 4:
                execlp("./rr.exe", "rr", argv[1], NULL);
                break;
            default:
                printf("You must enter a number between 1 and 4.\n");
                break;
        }
    }
    else{
        wait(NULL);
    }

    return 0;
}