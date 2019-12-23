#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define N 30 //Maximum of 30 processes
#define MAX_LEN 80 //Maximum length of array

int burstTime[N]={0}, arrivalTime[N]={0},
    taTime[N]={0}, wTime[N]={0}, ct[N]={0};
int totalProcess = 0, tBT = 0, arrived = 0;
float tTATime = 0, tWTime = 0;

int shortest_job();
void get_data(char* file);
void calc_sjf();
void print_table();

//lines are process arrival time and burst time delimited by a comma
void get_data(char* file){
    int i, j, t, t1,
	total = 0;
    FILE *f;
    char buff[MAX_LEN];
    f = fopen(file, "r");
    if (f == NULL) {
	perror ("error opening file");
	exit(0);
    }
    //first two lines ignored
    fgets(buff, MAX_LEN, f);
    fgets(buff, MAX_LEN, f);
    //read arrival time and burst time, invalid lines are ignored
    while (fgets(buff, MAX_LEN, f) != NULL && total < N){
	if (sscanf(buff,"%d,%d", &burstTime[total], &arrivalTime[total]) == 2)
	    total++;
    }

    fclose(f);
    totalProcess = total;

    //Swapping if user has not entered arrival time in ascending order
    for (i = 0; i < totalProcess; i++){
        for (j = i + 1 ; j < totalProcess; j++){
           if (arrivalTime[i] > arrivalTime[j]){
                t = arrivalTime[i];
                t1 = burstTime[i];
                arrivalTime[i] = arrivalTime[j];
                burstTime[i] = burstTime[j];
                arrivalTime[j] = t;
                burstTime[j] = t1;
              }
        }
    }
}

//returns index of shortest job
//a return value of -1 means all processes are completed
int shortest_job(){
	int min = INT_MAX,
	    index = -1,
	    i;

	//update number of processes arrived
	while (arrived < totalProcess && arrivalTime[arrived] <= tBT){
            arrived++;
    	}

	//decrements instead of increments to simulate adding to head of queue
	for (i = arrived - 1; i >= 0 ; i--){
	    //process has arrived and not completed
	    if (ct[i] == 0 && burstTime[i] < min){
            min = burstTime[i];
            index = i;
	    }
	}
	return index;
}

void calc_sjf(){
    int i, k;
    //first arrival
    tBT = arrivalTime[0];
    i = shortest_job();

    //Part of output for gantt chart
    for(int k = 0; k < 250; k++){
        printf("_");
    }
    printf("\n");
    printf("|");


    while (i != -1) {
        for(int j = 0; j < burstTime[i]-1; j++){
            printf("  ");
        }
        tBT += burstTime[i];
        ct[i] = tBT;
        i = shortest_job();
        printf("%2d|", i+1);
        //skip dead time
        if (i == -1 && arrived < totalProcess){
            printf("  ");
            tBT = arrivalTime[arrived];
            i = shortest_job();
        }
    }

	//calculate turnaround time and total times
    for (k = 0; k < totalProcess; k++){
        taTime[k] = ct[k]- arrivalTime[k]; //Here, ta-time = completion time (ct) - arrival time
        wTime[k] = taTime[k] - burstTime[k]; // calculate waiting time
        tTATime+= taTime[k]; //calculate the sum of ta-time
        tWTime+= wTime[k]; //calculate the sum of waiting time
    }

    //Part of output for gantt chart
    printf("\n");
    for(int k = 0; k < 250; k++){
        printf("-");
    }
    printf("\n");
}

//printing table
void print_table(){
	int i;
	//printf("P#\t\tAT\t\tBT\t\tCT\t\tTAT\t\tWT\t\n\n");
	printf("_________________________________________________________________________________________\n");
    printf("|                          SHORTEST JOB FIRST ALGORITHM                                 |\n");
	printf("_________________________________________________________________________________________\n");
    printf("| Process # | Arrival Time | Burst Time | Completion Time | Wait Time | Turnaround Time |\n");
    printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");

	for (i = 0; i < totalProcess; i++){
		//printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",i+1,arrivalTime[i],burstTime[i],ct[i],taTime[i],wTime[i]);
		printf("|%10d |%13d |%11d |%16d |%10d |%16d |\n", i+1, arrivalTime[i], burstTime[i], ct[i], wTime[i], taTime[i]);
        printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");
	}
	// printf("------------------------------------------------------------------------------------");
	// printf("\n\t\t\t\t\t\t\t\t%.2f\t\t%.2f\n",tTATime,tWTime);

	// printf("\n\nAverage Turnaround Time = %f\n",tTATime/totalProcess); //ave. ta-time
	// printf("Average WTime = %f\n\n",tWTime/totalProcess); //ave. waiting time
    printf("|___________|______________|____________|_________________|___________|_________________|\n");
    printf("| Average Wait time: %6.2f             | Average Turnaround time: %6.2f               |\n", tWTime/totalProcess, tTATime/totalProcess);
    printf("|_______________________________________|_______________________________________________|\n");
}

int main(int argc, char **argv){

    if(argc < 2){
        printf("Error: No file name given on command line.\n");
        exit(0);
    }
    else if(argc > 2){
        printf("Error: The only command line arguments should be the executable and the file name.\n");
        exit(0);
    }

	get_data(argv[1]);
	calc_sjf();
	print_table();
}
