#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>

using namespace std;

string filename;
ifstream procIn;
int curTime = 0;




//contains per process information
class Process
{
public:
	string name;
	char state;
	int arrive;
	int burst;
	int wait;

};





//check if a file can be opened
bool checkFile(string file)
{
	procIn.open(file);
	return ( procIn.is_open() );
}

 




int partition (std::vector<Process*> & vect, int low, int high)  
{  
	int pivot = vect[high]->burst;
    //int pivot = vect[high].burst; // pivot

    int i = (low - 1); // Index of smaller element  
  
    for (int j = low; j <= high - 1; j++)  
    {  
        // If current element is smaller than the pivot 
        if (vect[j]->burst < pivot)  
        { 

            i++; // increment index of smaller element  
            swap(vect[i], vect[j]);

        }  
    }

    swap(vect[i+1], vect[high]);
    return (i + 1);
} 

// adapted from https://www.geeksforgeeks.org/quick-sort/
// Changed significantly to work for vector of pointers,
// and objects with integer attributes.
void quickSort(std::vector<Process*> & vect, int low, int high)  
{  
    if (low < high)  
    {  
        /* pi is partitioning index, arr[p] is now  
        at right place */
        int pi = partition(vect, low, high);  

        // Separately sort elements before  
        // partition and after partition  
        quickSort(vect, low, pi - 1);  
        quickSort(vect, pi + 1, high);  
    }  
}








//print line data an increment wait times
void printTable(std::vector<Process> & v)
{
		cerr << curTime;

		//print state for each process
		for (int q=0; q < v.size(); q++)
		{
			cerr << "\t" << v[q].state;
			//incrment wait time
			if (v[q].state == '.')
			{
				v[q].wait++;
			}
		}
		cerr << endl;
}







int main( int argc, char ** argv)
{

	//Error Checks and initialization
	if (argc < 2 || argc > 4) 
	{
		cerr << "Provide 1 or 2 or 3 arguments" << endl;
		return -1;
	}
	if (argc == 2)
	{
		cerr << "Provide additional arguments: RR or SJF" << endl;
		return -1;
	}
	if (argc == 3)
	{
		if (std::string(argv[2]) ==  "RR")
		{
			cerr << "Provide quantum as an additional argument for RR" <<  endl;
			return -1;
		}
		if (std::string(argv[2]) !=  "SJF") 
		{
			cerr << "Enter SJF or RR" <<  endl;
			return -1;
		}
		else
		{	
			filename = argv[1];
			//Check that the file can be opened
			if (checkFile(filename) == false)
			{
				cerr << "Could not open file" <<  endl;
	  		exit(-1);
			}








			//SJF Scheduler
			else
			{
				//Initialize processes data to vector
				cerr << "Time\t";

				std::vector<Process> procQ;						//vector to store Process objects

				int in = 0;
				string ss;

				//create each Process object in txt file and assign its initial attributes
				while(procIn >> ss)
				{

					procQ.push_back( Process() );
					cerr << "P" + to_string(in) + "\t";

					procQ[in].state = ' ';

					procQ[in].arrive = stoi(ss);
					procIn >> ss;

					procQ[in].burst = stoi(ss);

					procQ[in].wait = 0;

					in++;
				}



				
				cerr << endl
				 << "------------------------------------------------------------"
				 << endl;





				 std::vector<Process*> readyQ;	//readyQ stores pointers to Process objects
				 std::vector<Process*> cpu;		//cpu stores pointers to process objects

				 int jobsRemain = procQ.size();
				 int nextJob = 0;				//index of next job in process queue to arrive
				 int cpuState = 0;				//0 idle; 1 running

				 //run scheduling algorithm
				 while (1)
				 {
				 	
				 	

				 	if (jobsRemain == 0) {break;}

				 	//process on CPU finished
				 	if (cpuState != 0 && cpu[0]->burst == 0 )
				 	{
				 		cpu[0]->state = ' ';	//change state to finished
				 		cpu.pop_back();			//remove proc from CPU
				 		cpuState--;				//cpu state to idle
				 		jobsRemain--;
				 		continue;
				 	}

				 	//if next job in proc queue has arrived, add it to ReadyQueue
				 	if (procQ[nextJob].arrive == curTime)
				 	{

				 		//check for all jobs with a tie in arriving time
				 		for (int q=nextJob; q < procQ.size(); q++)
				 		{
				 			if (procQ[q].arrive == curTime)
				 			{
				 				readyQ.push_back(&procQ[q]);
				 				readyQ.back() ->state = '.';
				 				nextJob++;
				 			}

				 		}
				 		//sort readyQueue Min to Max
				 		quickSort( readyQ, 0, readyQ.size()-1 );
				 		

				 	}

				 	//if CPU is idle and RQ !empty then add proc to CPU
				 	if(cpuState == 0  && readyQ.size() != 0) //
				 	{
				 		cpu.push_back(readyQ[0]);		//move from RQ to CPU
				 		readyQ.erase(readyQ.begin());	//Remove from RQ
				 		cpu[0]->state = '#';			//state of moved proc to running
				 		cpuState++;						//set CPU to running
				
				 	}

				 	//run one cycle of CPU
				 	if (cpuState != 0)
				 	{
				 		cpu[0]->burst--;
				 	}
				 	
				 	//print data and increment time
				 	printTable(procQ);
				 	curTime++;

				 }

				//Calculate and Print Waiting Times:
				 cerr 
				 << "------------------------------------------------------------"
				 << endl;
				 float averageWait=0;
				 for (int q=0; q < procQ.size(); q++)
				 {
				 	cerr << "P" << q << "  "<< "waited " << std::setprecision(4) << procQ[q].wait << " sec.\n";
				 	averageWait += procQ[q].wait;
				 }

				 averageWait = averageWait / procQ.size();
				 

				 cerr << "\n" << "Average waiting time = " << std::setprecision(4) << averageWait << " sec.\n"; 



			}


			procIn.close();
			return -1;
		}









	}
	if (argc == 4)
	{
		if (std::string(argv[2]) ==  "SJF")
		{
			cerr << "SJF does not take a quantum. Enter 1 less argument." <<  endl;
			return -1;	
		}
		if (std::string(argv[2])!= "RR")
		{
			cerr << "Enter RR or SJF" <<  endl;
			return -1;	
		}
		else
		{
			filename = argv[1];
			//Check that the file can be opened
			if (checkFile(filename) == false)
			{
				cerr << "Could not open file" <<  endl;
				exit(-1);
			}











			//RR scheduler
			else
			{
				int quantum = atoi(argv[3]);




				//Initialize processes data to vector
				cerr << "Time\t";

				std::vector<Process> procQ;						//vector to store Process objects

				int in = 0;
				string ss;

				//create each Process object in txt file and assign its initial attributes
				while(procIn >> ss)
				{

					procQ.push_back( Process() );
					cerr << "P" + to_string(in) + "\t";

					procQ[in].state = ' ';

					procQ[in].arrive = stoi(ss);
					procIn >> ss;

					procQ[in].burst = stoi(ss);

					procQ[in].wait = 0;

					in++;
				}



				
				cerr << endl
				 << "------------------------------------------------------------"
				 << endl;





				 std::vector<Process*> readyQ;	//readyQ stores pointers to Process objects
				 std::vector<Process*> cpu;		//cpu stores pointers to process objects

				 int jobsRemain = procQ.size();
				 int nextJob = 0;				//index of next job in process queue to arrive
				 int cpuState = 0;				//0 idle; 1 running

				 //run scheduling algorithm
				 while (1)
				 {
				 	
				 	
				 	
				 	if (jobsRemain == 0) {break;}

				 	if (cpuState != 0) 
				 	{

				 					int remBurst = cpu[0]->burst - quantum;

				 					//run process on CPU for time slice
								 	while (remBurst < cpu[0]->burst)
								 	{
										 			//process on CPU finished
												 	if (cpu[0]->burst == 0 )
												 	{

												 		
												 		cpu[0]->state = ' ';	//change state to finished
												 		cpu.pop_back();			//remove proc from CPU
												 		cpuState--;				//cpu state to idle
												 		jobsRemain--;
												 		break;
												 		
												 	}

												 	//if next job in proc queue has arrived, add it to ReadyQueue
												 	if (procQ[nextJob].arrive == curTime)
												 	{

												 		//check for all jobs with a tie in arriving time
												 		for (int q=nextJob; q < procQ.size(); q++)
												 		{
												 			if (procQ[q].arrive == curTime)
												 			{

												 				readyQ.push_back(&procQ[q]);
												 				readyQ.back() ->state = '.';
												 				nextJob++;
												 			}

												 		}
												 		
												 	}

													//run one cycle of CPU
												 	cpu[0]->burst--;
												 	printTable(procQ);
												 	curTime++;	

									}
					}





				 	//if first job in proc queue has arrived, add it to ReadyQueue
				 	if (procQ[0].arrive == curTime)
				 	{

				 				readyQ.push_back(&procQ[0]);
				 				readyQ.back() ->state = '.';
				 				nextJob++;
				 		
				 	}

				 	//if first job has not arrived in readyqueue
				 	if (procQ[0].arrive > curTime)
				 	{
				 		printTable(procQ);
				 		curTime++;
				 		
				 	}




					//process expires on its final time slice
				 	if (cpuState !=0 && cpu[0]->burst == 0 )
				 	{
				 		cpu[0]->state = ' ';	//change state to finished
				 		cpu.pop_back();			//remove proc from CPU
				 		cpuState--;				//cpu state to idle
				 		jobsRemain--;
				 		
				 	}

					//process on CPU did not finish in time slice
				 	if (cpuState != 0 && cpu[0]->burst != 0 )
				 	{
				 		cpu[0]->state = '.';	//change state from running to ready
				 		readyQ.push_back(cpu[0]);//copy from cpu to RQ
				 		cpu.pop_back();			//remove proc from CPU
				 		cpuState--;				//cpu state to idle
				 		continue;
				 	}




				 	//if CPU is idle and RQ !empty then add proc to CPU
				 	if(cpuState == 0  && readyQ.size() != 0) //
				 	{
				 		cpu.push_back(readyQ[0]);		//move from RQ to CPU
				 		readyQ.erase(readyQ.begin());	//Remove from RQ
				 		cpu[0]->state = '#';			//state of moved proc to running
				 		cpuState++;						//set CPU to running
				
				 	}


				 }





				//Calculate and Print Waiting Times:
				 cerr 
				 << "------------------------------------------------------------"
				 << endl;
				 float averageWait=0;
				 for (int q=0; q < procQ.size(); q++)
				 {
				 	cerr << "P" << q << "  "<< "waited " << std::setprecision(4) << procQ[q].wait << " sec.\n";
				 	averageWait += procQ[q].wait;
				 }

				 averageWait = averageWait / procQ.size();
				 

				 cerr << "\n" << "Average waiting time = " << std::setprecision(4) << averageWait << " sec.\n"; 



			}




			procIn.close();
			return -1;
		}
	}


}