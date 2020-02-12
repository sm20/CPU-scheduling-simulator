FUNCTIONALITY

Simulates Round Robin (RR) or Shortest-job-first (SJF) CPU scheduling algorithms in the command line when provided with an input file containing formatted descriptions of processes.  
If the user wishes to simulate RR then they must provide an integer quantum as the third argument.  
If the user wishes to simulate SJF then no third argument is required.  
The program outputs a table depicting the time spent by each individual process as a function of the time. It also
provides statistics on process waiting time.  
  
INSTRUCTIONS  

1. compile: g++ scheduler.cpp  
2. If simulating RR run: ./a.out &lt;text file> RR &lt;quantum>  
    -Where text file is the process description file  and quantum is an integer time slice-  
3. If simulating SJF run: ./a.out <text file> SJF
