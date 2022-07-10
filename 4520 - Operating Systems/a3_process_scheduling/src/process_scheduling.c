// Kent Studer (krspcp - 12544417)
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

/* Shortest time first comparison function */
int ARRIVALcompare(void const *a, void const *b)
{
    ProcessControlBlock_t first = *(ProcessControlBlock_t*)a;
    ProcessControlBlock_t second = *(ProcessControlBlock_t*)b;

    //sorting based on arrivals first
    if(first.arrival > second.arrival)
    {
        return -1;
    }
    else if(first.arrival < second.arrival)
    {
        return 1;
    }
    else //equal arrivals goes to shortest burst
    {
        if(first.remaining_burst_time < second.remaining_burst_time)
        {
            return 1;
        }
        else if(first.remaining_burst_time > second.remaining_burst_time)
        {
            return -1;
        }
        else
        {
            return 0;
        }  
    }
}

int BURSTcompare(void const *a, void const *b)
{
    ProcessControlBlock_t first = *(ProcessControlBlock_t*)a;
    ProcessControlBlock_t second = *(ProcessControlBlock_t*)b;

    //sorting based on burst
    if(first.remaining_burst_time > second.remaining_burst_time)
    {
        return -1;
    }
    else if(first.remaining_burst_time < second.remaining_burst_time)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int PRIORITYCompare(void const *a, void const *b)
{
    ProcessControlBlock_t first = *(ProcessControlBlock_t*)a;
    ProcessControlBlock_t second = *(ProcessControlBlock_t*)b;

    if(first.priority < second.priority)
    {
        return -1;
    }
    else if(first.priority > second.priority)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//custome floor function since math.h is not being linked in tc.rnet.
double floor(double num) {
    if (num >= 9223372036854775807 || num <= -9223372036854775807 || num != num) {
        /* handle large values, infinities and nan */
        return num;
    }
    long long n = (long long)num;
    double d = (double)n;
    if (d == num || num >= 0)
        return d;
    else
        return d - 1;
}

/* This function will implement the first come first serve scheduling algorithm, returns true for success and false for failure */
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }
    else
    {
        //to implement this we can just pop the front of the ready queue for each iteration 
        //keeping track of the wait times, turn around times, and storing the averages in the result pointer
        //since we are focused on all nodes arriving at the same time, we need to first sort the array based on the priority
        //Wait times = time started - time arrived
        //Turn around = time finished - time arrived
        ProcessControlBlock_t *temp = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        if(temp == NULL)
        {
            return false;
        }

        clock_t start = clock(); //start overarching clock
        int count = 0;
        double WT_total = 0;
        double TT_total = 0;

        while(dyn_array_extract_back(ready_queue, temp))
        {
            //mark that we are starting the each process
            clock_t temp_start = clock();
            temp->started = true;

            double cpu_time = 0;
            while(cpu_time < temp->remaining_burst_time) //this while loop should execute until the cpu clock has taken burst time seconds to complete
            {
                clock_t check = clock();
                cpu_time = ((double)(check - temp_start) / CLOCKS_PER_SEC);
            }

            clock_t temp_finish = clock(); //process has finished
            
            WT_total += ((double)(temp_start - start) / CLOCKS_PER_SEC);
            TT_total += ((double)(temp_finish - start) / CLOCKS_PER_SEC);

            count++;
        }
        clock_t finish = clock();
        double AVG_WT = WT_total / (double)count;
        double AVG_TT = TT_total / (double)count;
        double TOTAL = ((double)(finish - start) / CLOCKS_PER_SEC);

        result->average_turnaround_time = AVG_TT;
        result->average_waiting_time = AVG_WT;
        result->total_run_time = TOTAL;

        return true;
    }
    return false;
}
/* This function will implement the shortest job first algorithm on a virtual CPU. To do this I will implement a comparison function
    that can sort the array of PCBs by its burst time. Once that is done the program will remove from the back similar to FCFS */
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }
    else
    {

        //First things first we should sort the ready queue in terms of arrival times
        dyn_array_sort(ready_queue, ARRIVALcompare);

        //need temp and next PCB for extraction
        ProcessControlBlock_t *temp = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        ProcessControlBlock_t *next = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        dyn_array_t *temp_q = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);

        if(temp == NULL || next == NULL || temp_q == NULL)
        {
            return false;
        }

        double WT_total = 0;
        double TT_total = 0;
        bool empty_flag = false;
        int count = dyn_array_size(ready_queue);

        //pull first one off back and add it to new queue
        //we need another start to signal start of different scheduling alg
        clock_t MAIN_Start = clock();
        dyn_array_extract_back(ready_queue, temp);
        dyn_array_push_back(temp_q, temp);


        //pop the first PCB off and store in temp
        while(dyn_array_extract_back(temp_q, temp))
        {

            size_t PCB_left;

            //extract the next arrived PCB
            if(empty_flag == false){

                dyn_array_extract_back(ready_queue, next);
            }

            //start clock for the PCB
            clock_t PCB_Start = clock();
            temp->started = true;
            
            double cpu_time = 0;
            while(cpu_time <= temp->remaining_burst_time) //this while loop should execute until the cpu clock has taken burst time seconds to complete
            {
                //if our next PCB is ready while the current is still running, add it to temp_q and remove the next one in line
                clock_t check = clock();
                cpu_time = ((double)(check - PCB_Start) / CLOCKS_PER_SEC);

                //if the next PCBs arrival is during the current burst time, add it to the queue and get the next one
                //once we add it to the queue we can sort the current waiting nodes based on their burst times
                if((cpu_time == (double)next->arrival) && empty_flag == false)
                {
                    dyn_array_push_back(temp_q, next);
                    dyn_array_sort(temp_q, BURSTcompare);
                    PCB_left = dyn_array_size(ready_queue);
                    dyn_array_extract_back(ready_queue, next);
                    if(PCB_left == 0)
                    {
                        empty_flag = true;
                    }
                }
            }
            clock_t PCB_End = clock();
            double PCB_arrival = (double)temp->arrival;

            //because the clock starts when the testing does we have to floor each addition to account for the offset of ticks
            //in between functions
            WT_total += floor(((double)((PCB_Start - MAIN_Start) / CLOCKS_PER_SEC) - PCB_arrival));
            TT_total += floor(((double)((PCB_End - MAIN_Start) / CLOCKS_PER_SEC) - PCB_arrival));

        }
        clock_t MAIN_End = clock();

        double AVG_WT = WT_total / (double)count;
        double AVG_TT = TT_total / (double)count;
        double TOTAL = ((double)(MAIN_End - MAIN_Start) / CLOCKS_PER_SEC);

        result->average_turnaround_time = AVG_TT;
        result->average_waiting_time = AVG_WT;
        result->total_run_time = TOTAL;

        free(temp);
        free(next);
        free(temp_q);

        return true;
    }
    return false;
}  

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}
/* This funciton will execute a "virtual" round robin scheduling technique for the given ready queue full of Process Control Blocks */
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    if(ready_queue == NULL || result == NULL || quantum == 0)
    {
        return false;
    }
    else
    {
        size_t PCB_left;
        bool isEmpty = false;
        double TT_Total = 0;
        int TT_Burst = 0;

        //like in other scheduling algorithms lets sort the array first by arrival times
        dyn_array_sort(ready_queue, PRIORITYCompare);

        //in order to do this we must create a separate queue to store all the PCBs that are waiting time be completed
        dyn_array_t* wait_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
        ProcessControlBlock_t* temp = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        ProcessControlBlock_t* next = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        size_t total_PCBs = dyn_array_size(ready_queue);

        if(wait_queue == NULL || temp == NULL)
        {
            return false;
        }

        //remove the first PCB from the ready queue and add it to the wait queue
        dyn_array_extract_back(ready_queue, temp);
        if(temp->arrival > 0)
        {
            dyn_array_push_front(ready_queue, temp);
            dyn_array_extract_back(ready_queue, temp);
        }
        dyn_array_push_back(wait_queue, temp);

        clock_t MAIN_Start = clock();

        while(dyn_array_extract_back(wait_queue, temp))
        {
            //if this is the PCBs first time being processed
            //bool first = false;
            if(temp->started == false)
            {
                temp->started = true;
                TT_Burst += temp->remaining_burst_time;
            }
        
            bool next_PCB = dyn_array_extract_back(ready_queue, next);
            clock_t check_1 = clock();
            double check_time = floor(((double)(check_1 - MAIN_Start) / CLOCKS_PER_SEC));
            if(next_PCB == true && next->arrival <= check_time)
            {
                dyn_array_push_back(wait_queue, next);
            }

            double cpu_time = 0;
            double temp_TT = 0;
            clock_t PCB_Start = clock();

            //loop should run and remove quantum time
            while(cpu_time <= temp->remaining_burst_time)
            {
                clock_t check = clock();
                cpu_time = ((double)(check - PCB_Start) / CLOCKS_PER_SEC);

                //while the clock is running, if it is equal to the quantum but the PCB still has time remaining
                //subtract the cpu_time from the burst, set the time equal so it exists the loop
                //then push the PCB back onto the wait queue
                if(cpu_time == quantum && cpu_time < temp->remaining_burst_time)
                {

                    //subtract what clock we have removed from the burst if quantum is smaller then initial burst
                    temp->remaining_burst_time -= cpu_time;

                    cpu_time = quantum;
                    dyn_array_push_front(wait_queue, temp);
                    break;
                }
                //if PCB has finished we clock the cpu time, perform calculations and check whether there are more PCBs
                //in the ready queue, if so push them onto the wait_queue
                if(cpu_time == temp->remaining_burst_time)
                {
                    clock_t PCB_End = clock();
                    temp_TT = ((double)((PCB_End - MAIN_Start) / CLOCKS_PER_SEC) - temp->arrival);                

                    TT_Total += temp_TT;

                    PCB_left = dyn_array_size(ready_queue);
                    if(PCB_left == 0)
                    {
                        isEmpty = true;
                    }
                    
                    if(isEmpty == false)
                    {
                        dyn_array_extract_back(ready_queue, temp);
                        dyn_array_push_front(wait_queue, temp);
                    }
                    //PCB is finished!
                    temp->started = false;
                    break;
                }
            } 
        }
        clock_t MAIN_End = clock();

        double AVG_WT = (TT_Total - TT_Burst) / total_PCBs;
        double AVG_TT = TT_Total / total_PCBs;
        double TOTAL = ((double)(MAIN_End - MAIN_Start) / CLOCKS_PER_SEC);

        result->average_turnaround_time = AVG_TT;
        result->average_waiting_time = AVG_WT;
        result->total_run_time = TOTAL;

        return true;
    }
    return false;
}
/* This function will take a string (char*) and attempt to read a file and import the data as an array of 
    ProcessControlBlocks, stored in a dyn_array. If error occurs, NULL will be returned */
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    if(input_file == NULL) //input validation
    {
        return NULL;
    }
    else
    {
        int file = open(input_file, O_RDONLY);
        if(file == -1) //could not find file
        {
            return NULL;
        }

        int *PCBs = malloc(sizeof(int)); //read first value of file as number of bursts
        read(file, (void*)PCBs, 4); 
        if(*PCBs < 0)  //read will put large negative value in bursts if file is empty
        {
            return NULL;
        }
        
        uint32_t dst[(*PCBs * 3)]; //each PCB has 3 uint32_t objects 
        lseek(file, 4, SEEK_SET); //offset past the first uint32_t which is our PCBs
        ssize_t bytes_read = read(file, (void*)dst, (*PCBs * 12));
        if(bytes_read != (*PCBs * 12))
        {
            free(PCBs);
            return NULL;
        }

        dyn_array_t *pcb_array = dyn_array_create((size_t)PCBs, 13, NULL); //object is 13 bytes long, 3 * uint32_t + bool = 13 bytes
        int count = 0;
        for(int i = 0; i < (*PCBs * 3); i++) //loop through and insert each value appropriately
        {
            ProcessControlBlock_t *temp = malloc(sizeof(ProcessControlBlock_t));
            temp->remaining_burst_time = dst[i];
            temp->priority = dst[i+1];
            temp->arrival = dst[i+2];

            dyn_array_insert(pcb_array, count, temp);

            count++;
            i += 2;
            free(temp);
        }

        free(PCBs);
        if(pcb_array == NULL) //final error check and free loose ends
        {
            return NULL;
        } 
        return pcb_array;
    }
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }
    else
    {
        //to do this we will keep another queue that stores incomplete PCBs waiting to be processed
        //before we draw from this wait queue every time we will sort based on the remaining burst time
        //the PCB with the shortest burst time that is NOT 0 will be processed. During this processing
        //every time a unit of time is subtract we will check the wait queue to determine if there is a shorter burst time
        //if so we stop processing load the current PCB back onto the wait queue and process the shorter block
        //bool isEmpty = false;

        dyn_array_t *wait_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
        ProcessControlBlock_t *current = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        ProcessControlBlock_t *next = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
        ProcessControlBlock_t *PCB_check = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));

        if(wait_queue == NULL || current == NULL || PCB_check == NULL || next == NULL)
        {
            return false;
        }

        //lets sort based on arrival time so we know which one comes first to process
        dyn_array_sort(ready_queue, ARRIVALcompare);

        //remove first arrived PCB and add it to the wait queue so we can extract in the while loop
        dyn_array_extract_back(ready_queue, current);
        dyn_array_push_back(wait_queue, current);

        clock_t MAIN_Start = clock();

        while(dyn_array_extract_back(wait_queue, current))
        {
            double cpu_time = 0;
            uint32_t smallest_burst;
            clock_t PCB_Start = clock();
            printf("PCB %lu is being processed now\n", (unsigned long)current->priority);
            printf("REMAINING TIME: %lu\n", (unsigned long)current->remaining_burst_time);

            if(current->started == false)
            {
                current->started = true;
            }

            //extract the next PCB in time, if there are none left, set empty flag to true
            bool next_PCB = dyn_array_extract_back(ready_queue, next);
            /*if(next_PCB == false)
            {
                isEmpty == true;
            }*/

            //if in this current moment in time the next PCB has arrived we add it to the ready queue & sort the 
            //waiting PCBs by there burst time, then we continue processing the PCB we have started
            clock_t check_1 = clock();
            double check_time = floor(((double)(check_1 - MAIN_Start) / CLOCKS_PER_SEC));
            if(next_PCB == true && next->arrival <= check_time)
            {
                printf("PCB %lu has arrived and is added to the wait queue\n", (unsigned long)next->priority);
                dyn_array_push_back(wait_queue, next);
                dyn_array_sort(wait_queue, BURSTcompare);
                PCB_check = dyn_array_at(wait_queue, 0);
                smallest_burst = PCB_check->remaining_burst_time;
            }

            while(cpu_time <= current->remaining_burst_time)
            {
                clock_t check = clock();
                cpu_time = ((double)(check - PCB_Start) / CLOCKS_PER_SEC);
                current->remaining_burst_time -= cpu_time;
                if(smallest_burst < current->remaining_burst_time)
                {
                    printf("We have found a smaller burst time loading PCB %lu back onto queue\n", (unsigned long)current->priority);
                    dyn_array_push_front(wait_queue, current);
                    dyn_array_sort(wait_queue, BURSTcompare);
                    break;
                } 
            }
            printf("PCB %lu has finished\n", (unsigned long)current->priority);
            return true;
        }
        return false;
    }
}
