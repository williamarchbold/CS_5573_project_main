#include <iostream>
#include <stdio.h> //printf
#include <vector> //store the ints
#include <unistd.h> //get num cores
#include <sys/wait.h> //wait for child process
#include <chrono> //measure time
#include <cstring> //concatenate string

std::vector<int> SpawnSortProcesses(int);


int main() 
{
    int numCP = sysconf(_SC_NPROCESSORS_ONLN);

    printf("Number of cores: %d\n\n", numCP);

    std::vector<int> times = SpawnSortProcesses(24);
    
    return 0;
}

std::vector<int> SpawnSortProcesses(int numCores)
{
    std::vector<int> times;
    char timePath[] = "/usr/bin/time";
    char timeOption1[] = "-v";
    char timeOption2[] = "-o";
    char binaryPath[] = "/home/william/CS_5573/mergesort_project/merge_sort";
    //char redirection[] = " > output.txt";

    int i;

    for (i = 0; i < numCores; i++)
    {
        printf("\nCore: %d About to fork...\n", i);
        //std::chrono::_V2::system_clock::time_point start;
        pid_t pid;
        pid = fork();
        if (pid == 0) {

            int process_id = getpid();
            std::string i_as_string = std::to_string(i+1);
            std::string proc_as_string = std::to_string(process_id);
            std::string location_as_string = "results/";
            std::string file_location = location_as_string 
                                        + proc_as_string 
                                        + "_" 
                                        + i_as_string 
                                        + ".txt";
            char * timeOption3 = &file_location[0];
            char * binaryOption1 = &proc_as_string[0];
            //start = std::chrono::high_resolution_clock::now();
            printf("Core: %d  Child process ID: %d \n", i, process_id);
            if (execl(timePath, 
                    timePath, 
                    timeOption1, 
                    timeOption2, 
                    timeOption3, 
                    binaryPath, 
                    binaryOption1,
                    NULL) == -1) { 
                printf("\nError calling execl\n");
            }
        }
        // else {
             //wait(NULL);
        //     auto end = std::chrono::high_resolution_clock::now();
        //     auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        //     //auto duration = end - start;
        //     times.push_back((int) duration.count());

        //     //std::cout << times[0] << std::endl;

        //     printf("\nCore: %d Time: %ld \n", i, duration.count());
        //}
        else if (i == numCores - 1)
        {
            printf("last core. i = %d", i);
            wait(NULL);
            printf("\nExiting SpawnSortProcesses\n");
        }
        
    }
    return times;
}