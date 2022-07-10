#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    dyn_array_t * r_queue;
    r_queue = load_process_control_blocks(argv[1]);
    if(r_queue == NULL)
    {
        printf("ERROR: could not read PCB file\n");
        return EXIT_FAILURE;
    }
    else
    {
        ScheduleResult_t *result = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
        if(result == NULL)
        {
            printf("ERROR: failed to allocate result memory\n");
            return EXIT_FAILURE;
        }
        result->average_waiting_time = 0;
        result->average_turnaround_time = 0;
        result->total_run_time = 0;

        //scheduling algo is First Come First Serve
        if(strcmp(argv[2], FCFS) == 0)
        {
            first_come_first_serve(r_queue, result);
        }
        else if(strcmp(argv[2], P) == 0) //Shortest remaining time first
        {
            shortest_remaining_time_first(r_queue, result);
        }
        else if(strcmp(argv[2], RR) == 0) //round robin, check for valid quantum here
        {
            if(strcmp(argv[3], "") == 0)
            {
                printf("ERROR: No quantum provided, cannot run Round Robin\n");
                return EXIT_FAILURE;
            }
            else
            {
                int quantum = atoi(argv[3]);
                size_t quantum_convert = (size_t)quantum;
                round_robin(r_queue, result, quantum_convert);
            }
        }
        else if(strcmp(argv[2], SJF) == 0)
        {
            shortest_job_first(r_queue, result);
        }
        else
        {
            printf("ERROR: Invalid input\n");
            return EXIT_FAILURE;
        }

        printf("%s Scheduling Algorithm Results:\n", argv[2]);
        printf("AVERAGE WAIT TIME: %.4f\nAVERGAE TURN AROUND TIME: %.4f\nTOTAL RUN TIME: %.4f\n", (double)result->average_waiting_time, 
            (double)result->average_turnaround_time, (double)result->total_run_time);
        printf("Successful Analysis!\n");

        dyn_array_destroy(r_queue);

        return EXIT_SUCCESS;
    }
}
