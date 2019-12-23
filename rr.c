#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 80

//A maximum of 30 processes can be added
int burst[30];
int arrival[30];
int process[30];
//Keeps track of the number of processes
int num_proc = 0;
//The time quantum for how long a process can run before switching
int quantum = 0;
int cs_overhead = 0;
//Stores the wait, turnaround, and completion times for each process
int wait[30];
int turnaround[30];
int complete[30];
//Stores average times
float avg_wait = 0;
float avg_turnaround = 0;


//Function that reads the data from a provided filename
void get_data(char *filename){

    printf("%s\n", filename);
    FILE *fp;
    char buffer[MAX_LEN];
    int i = 0;
    //Open the file to read
    if((fp = fopen(filename, "r")) == NULL){
        perror("Could not open file");
        exit(0);
    }
    //Read the time quantum. It should be alone on the first line of the text file
    if(fgets(buffer, MAX_LEN, fp) != NULL){
        if(!sscanf(buffer, "%d", &quantum))
	    perror("Time quantum not read");
    }
    if(fgets(buffer, MAX_LEN, fp) != NULL){
        if(!sscanf(buffer, "%d", &cs_overhead))
	    perror("Context switch overhead not read");
    }
    //Reads the processes and burst times. If it reads 30 processes then it stops.
    while((i < 30) && (fgets(buffer, MAX_LEN, fp) != NULL)){
        //The number before the comma is burst time, the number at the comma is arrival time
	//Invalid lines ignored
        if(sscanf(buffer, "%d,%d", &burst[i], &arrival[i]) == 2)
            i++;
    }
    //Sets the number of processes to prevent looping over empty sections of the arrays
    num_proc = i;

    //We're going to be re-ordering the arrays, so we need a separate way of keeping track of the original process order
    for(int j = 0; j < 30; j++){
        process[j] = j+1;
    }
    //Closes the file
    fclose(fp);
}

//Prints all the data
void print_data(){
    FILE *fp;
    if((fp = fopen("output.txt", "w")) == NULL){
        perror("Could not open file");
        exit(0);
    }

    printf("_________________________________________________________________________________________\n");
    printf("|                               ROUND ROBIN ALGORITHM                                   |\n");
    printf("_________________________________________________________________________________________\n");
    printf("| Process # | Arrival Time | Burst Time | Completion Time | Wait Time | Turnaround Time |\n");
    printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");
    for(int i = 1; i <= num_proc; i++){
        for(int j = 0; j < num_proc; j++){
            if(process[j] == i){
                printf("|%10d |%13d |%11d |%16d |%10d |%16d |\n", process[j], arrival[j], burst[j], complete[j], wait[j], turnaround[j]);
                printf("|-----------|--------------|------------|-----------------|-----------|-----------------|\n");
            }
        }
    }
    printf("|___________|______________|____________|_________________|___________|_________________|\n");
    printf("| Average Wait time: %6.2f             | Average Turnaround time: %6.2f               |\n", avg_wait, avg_turnaround);
    printf("|_______________________________________|_______________________________________________|\n");
}
//Function that sorts the processes ascending according to arrival time
void sort(){
    //Keeps track of the current process's arrival time
    int key_a = 0, key_b = 0, key_p = 0, j = 0;

    //Sort arrival times using insertion sort
    for(int i= 1; i < num_proc; i++){
        key_a = arrival[i];
        key_b = burst[i];
        key_p = process[i];
        j = i - 1;

        while(j >= 0 && arrival[j] > key_a){
            arrival[j+1] = arrival[j];
            process[j+1] = process[j];
            burst[j+1] = burst[j];
            j--;
        }
        arrival[j+1] = key_a;
        burst[j+1] = key_b;
        process[j+1] = key_p;
    }
}

//Function that does the actual time calculations
void robin(){
    int time = 0, check_time = 0, done = 0, i = 0, sum = 0;
    int remain[num_proc];
    int space_num = 0;

    //Copies the burst times in order to keep track of remaining burst time
    for(i = 0; i < num_proc; i++){
        remain[i] = burst[i];
        //printf("%d, %d\n", )
    }

    for(int k = 0; k < 250; k++){
        printf("_");
    }
    printf("\n");
    printf("|");
    //This entire loop runs through the processes and finds wait, turnaround, and completion time for each process
    while(!done){
        done = 1;
        check_time = time;
        //Loop through each process
        for(i = 0; i < num_proc; i++){
            //If there is time remaining in the process
            if(remain[i] > 0){
                if(arrival[i] <= time){
                    //If there is less burst time remaining than the quantum
                    if(remain[i] < quantum){
                        space_num = remain[i] - 1;
                        if(quantum == 1){
                            space_num = 0;
                        }
                        for(int k = 0; k < space_num; k++){
                            printf("  ");
                        }
                        printf("%2d|", process[i]);
                        //Then it only adds the remaining time, rather than the full quantum
                        time += remain[i];
                        remain[i] = 0;
                    }
                    else{
                        //Otherwise it adds the full quantum to time, and removes it from remaining burst time
                        for(int k = 0; k < quantum-1; k++){
                            printf("  ");
                        }
                        printf("%2d|", process[i]);
                        time += quantum;
                        remain[i] -= quantum;
                    }

                    //If the process completed during this time slice
                    if(remain[i] == 0){
                        //Then calculate wait, turnaround, and completion time
                        complete[i] = time;
                        turnaround[i] = time - arrival[i];
                        wait[i] = turnaround[i] - burst[i];
                        if(wait[i] < 0){
                            wait[i] = 0;
                        }
                    }
                    else{
                        //If there is remaining time, then don't exit the while loop
                        done = 0;
			time += cs_overhead;
                    }
                }
                done = 0;
            }
        }
        //If time hasn't progressed but done is zero, then no current processes had time left
        //But there are still processes that haven't arrived yet. So we increment time and loop again
        if(check_time == time && done == 0){
            printf("  |");
            time++;
            done = 0;
        }
    }

    //Finds the average wait time across all processes
    for(i = 0; i < num_proc; i++){
        sum += wait[i];
    }
    avg_wait = (float)sum / num_proc;
    sum = 0;
    //Finds the average turnaround time across all processes
    for(i = 0; i < num_proc; i++){
        sum += turnaround[i];
    }
    avg_turnaround = (float)sum / num_proc;

    printf("\n");
    for(int k = 0; k < 250; k++){
        printf("-");
    }
    printf("\n");
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

    get_data(argv[1]);
    sort();
    robin();
    print_data();


    return 0;
}
