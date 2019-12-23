(This Project was created and finished by a group of three members (I was one of them))

(I tried adding a makefile to make it simpler, but I don't think it was supported in cygwin for some reason)

Compile in this order: <br>
    gcc rr.c -o rr<br>
    gcc fcfs.c -o fcfs<br>
    gcc sjf.c -o sjf<br>
    gcc strf.c -o strf<br>
    gcc TMD_Scheduler.c -o TMD<br>
    
Run the executable (TMD.exe) with your input file as a command line argument (ex: ./TMD test4.txt).
Each individual program can also be run the same way, but TMD allows you to choose which one to run.
It also only allows you to have 1 command line argument in addition to the executtable.
If you have more or less than one, it will give an error.

The format of the input file should have the quantum on the first line and the context switch overhead on the second line.
After that it will read burst time, then a comma, then arrival time for each line.

Example:

2<br>
0<br>
1,0<br>
3,2<br>
5,1<br>
8,6<br>

The numbers on the left (1, 3, 5, 8) are burst times and the numbers on the right (0, 2, 1, 6) are the arrival times for the corresponding processes.

The formatting on the output is based on the shell being more than 100 or so wide. So it wont format properly unless you have it fullscreened or at the very least enlarged.
