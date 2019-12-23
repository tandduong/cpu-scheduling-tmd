#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

//This program is inspired on GeeksforGeeks.
int num_proc = 0;
int cs_overhead = 0;

void get_data(char *filename, int at[], int bt[]){
    FILE *fp;
    char buffer[80];
    int i = 0;
    //Open the file to read
    if((fp = fopen(filename, "r")) == NULL){
        perror("Could not open file");
        exit(0);
    }
    //Read the time quantum. It should be alone on the first line of the text file
    if(fgets(buffer, 80, fp) != NULL){
        //Just getting rid of the first line since it's a time quantum and only applies to the Round Robin program
    }
    //read context switch overhead
    if (fgets(buffer, 80, fp) != NULL) {
        if (!sscanf(buffer,"%d", &cs_overhead))
	    perror("Context switch overhead not read");
    }
    //Reads the processes and burst times. If it reads 30 processes then it stops.
    while((i < 30) && (fgets(buffer, 80, fp) != NULL)){
        //The number before the comma is burst time, the number at the comma is arrival time
	//Invalid lines ignored
        if(sscanf(buffer, "%d,%d", &bt[i], &at[i]) == 2)
            i++;
    }
    //Sets the number of processes to prevent looping over empty sections of the arrays
    num_proc = i;

    //We're going to be re-ordering the arrays, so we need a separate way of keeping track of the original process order
    // for(int j = 0; j < 30; j++){
    //     process[j] = j+1;
    // }
    //Closes the file
    fclose(fp);
}
int main(int argc, char *argv[])
{
    int at[30],bt[30],rt[31],endTime,i,smallest; 
    int remain=0,time,sum_wait=0,sum_turnaround=0;
    int switched = 0;
    int prev = -1;

    if(argc < 2){
        printf("Error: No file name given on command line.\n");
        exit(0);
    }
    else if(argc > 2){
        printf("Error: The only command line arguments should be the executable and the file name.\n");
        exit(0);
    }

    get_data(argv[1], at, bt);

    for(int j = 0; j < num_proc; j++){
        rt[j] = bt[j];
    }
    rt[30] = INT_MAX;

    printf("_________________________________________________________________________________________\n");
    printf("|                        SHORTEST TIME REMAINING FIRST ALGORITHM                        |\n");
    printf("_________________________________________________________________________________________\n");
    printf("| Process # | Arrival Time | Burst Time | Completion Time | Wait Time | Turnaround Time |\n");
    printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");

    //find a process have the least remaining time and arrival time is less than or equal to the current time. 
    //This condition is checked at every unit time (time++)
    for(time=0;remain<num_proc;time++)
    {
        smallest=30;
        for(i=0;i<num_proc;i++)
        {
            if(at[i]<=time && rt[i]<rt[smallest] && rt[i]>0)
            {
                smallest=i;
            }

        }

        //If processes switched add context switch overhead
        if(prev != -1 && prev != 30 && smallest != prev) {
            time += cs_overhead;
        }
        
	    prev = smallest;
        rt[smallest]--;
        if(rt[smallest]==0)
        {
	        prev = -1;
            remain++;
            endTime=time+1; //Start with 0 so the endtime equals time + 1
            //printf("\nP[%d]\t|\t%d\t|\t%d",smallest+1,endTime-at[smallest],endTime-bt[smallest]-at[smallest]);
            printf("|%10d |%13d |%11d |%16d |%10d |%16d |\n", smallest+1, at[smallest], bt[smallest], endTime, endTime-bt[smallest]-at[smallest], endTime-at[smallest]);
            printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");
            sum_wait+=endTime-bt[smallest]-at[smallest];
            sum_turnaround+=endTime-at[smallest];
        }
    }

    printf("|___________|______________|____________|_________________|___________|_________________|\n");
    printf("| Average Wait time: %6.2f             | Average Turnaround time: %6.2f               |\n", sum_wait*1.0/num_proc, sum_turnaround*1.0/num_proc);
    printf("|_______________________________________|_______________________________________________|\n");
    return 0;
}
