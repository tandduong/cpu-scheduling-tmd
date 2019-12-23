#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 30 //define the array with 10 values. 

int num_proc = 0;

//Function that reads the data from a provided filename
void get_data(char *filename, int bt[], int at[]){
    FILE *fp;
    char buffer[80];
    int i = 0;
    //Open the file to read
    if((fp = fopen(filename, "r")) == NULL){
        perror("Could not open file");
        exit(0);
    }
    //Ignore first two lines
    fgets(buffer, 80, fp);
    fgets(buffer, 80, fp);
    //Reads the processes and burst times. If it reads 30 processes then it stops. Invalid lines are ignored.
    while((i < 30) && (fgets(buffer, 80, fp) != NULL)){
        //The number before the comma is burst time, the number at the comma is arrival time
	//Invalid lines ignored
        if(sscanf(buffer, "%d,%d", &bt[i], &at[i]) == 2)
           i++;
    }
    //Sets the number of processes to prevent looping over empty sections of the arrays
    num_proc = i;

    //Closes the file
    fclose(fp);
}

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Error: No file name given on command line.\n");
        exit(0);
    }
    else if(argc > 2){
        printf("Error: The only command line arguments should be the executable and the file name.\n");
        exit(0);
    }

	int i, j, k, t, t1, busrtTime[N]={0}, arrivalTime[N]={0}, taTime[N]={0}, wTime[N]={0}, ct[N]={0};
	int totalProcess,tBT = 0;
	float tTATime = 0, tWTime = 0;

	get_data(argv[1], busrtTime, arrivalTime);

    //Swapping if user has not entered arrival time in ascending order
	for(i = 0; i < num_proc; i++){
            for(j = i + 1 ; j < num_proc; j++){
              if(arrivalTime[i] > arrivalTime[j]){
                t = arrivalTime[i];
                t1 = busrtTime[i];
                arrivalTime[i] = arrivalTime[j];
                busrtTime[i] = busrtTime[j];
                arrivalTime[j] = t;
                busrtTime[j] = t1;
              }
        }
    }

    //Part of output for gantt chart
    for(int k = 0; k < 250; k++){
        printf("_");
    }
    printf("\n");
    printf("|");

	//calculate completion time of processes
    tBT = arrivalTime[0];
	for( j = 0; j < num_proc; j++){
        for(int k = 0; k < busrtTime[j]-1; k++){
            printf("  ");
        }
        printf("%2d|", j+1);
		tBT += busrtTime[j];
		ct[j] = tBT;
        
		//skip dead time
		if(tBT < arrivalTime[j+1]){
			tBT = arrivalTime[j+1];
            printf("  |");
        }
	}

    //Part of output for gantt chart
    printf("\n");
    for(int k = 0; k < 250; k++){
        printf("-");
    }
    printf("\n");

	//calculate turnaround time and waiting times
	for(k = 0; k < num_proc; k++){
		taTime[k] = ct[k]- arrivalTime[k]; //Here, ta-time = completion time (ct) - arrival time
		wTime[k] = taTime[k] - busrtTime[k]; //Here, waiting time = ta-time - burst time
		tTATime+= taTime[k]; //calculate the sum of ta-time
        tWTime+= wTime[k]; //calculate the sum of waiting time
	}

    printf("_________________________________________________________________________________________\n");
    printf("|                        FIRST COME FIRST SERVE ALGORITHM                               |\n");
	printf("_________________________________________________________________________________________\n");
    printf("| Process # | Arrival Time | Burst Time | Completion Time | Wait Time | Turnaround Time |\n");
    printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");

	for(i = 0; i < num_proc; i++){
		//printf("P%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",i+1,arrivalTime[i],busrtTime[i],ct[i],taTime[i],wTime[i]);
		printf("|%10d |%13d |%11d |%16d |%10d |%16d |\n", i+1, arrivalTime[i], busrtTime[i], ct[i], wTime[i], taTime[i]);
        printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");
	}
	// printf("\n\nAverage Turnaround Time = %f\n",tTATime/num_proc); //ave. ta-time
	// printf("Average WTime = %f\n\n",tWTime/num_proc); //ave. waiting time

    printf("|___________|______________|____________|_________________|___________|_________________|\n");
    printf("| Average Wait time: %6.2f             | Average Turnaround time: %6.2f               |\n", tWTime/num_proc, tTATime/num_proc);
    printf("|_______________________________________|_______________________________________________|\n");

	return 0;
}
