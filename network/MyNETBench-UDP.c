#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>

//argv[1] - Workload 
//atoi(argv[2]) - block size
//atoi(argv[3]) - number of threads

//Execution - UDP 1024 1


long NUMBER_OF_ITERATIONS = 10000000;
int NUMBER_OF_THREADS = 1;
int BLOCK_SIZE = 8;
int size_of_buffer;

int size_of_buffer; 
int number_of_threads; 

void* UDPprotocol(void* arg)
{
       int UDP_client_socket;
   
       struct sockaddr_in serverAddr;
       char * buffer = (char*)malloc(sizeof(char)*1000);
       UDP_client_socket = socket(AF_INET, SOCK_DGRAM, 0);
       memset(buffer, 1 , sizeof(buffer));
   
       if(UDP_client_socket <= 0)
       { 
         exit(0);
         printf("Error handling UDP Protocol \n ");
       }
       else
         printf("The UDP protocol is set \n");

       for(int i = 0; i < NUMBER_OF_ITERATIONS ; i++)
       {
         sendto(UDP_client_socket, buffer, sizeof(buffer) , 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
         //printf("Bye %d" ,i);
       }
   
       printf("UDP Connection established \n");

}


int main(int argc, char *argv[])
{
	
        
	int size_of_buffer = atoi(argv[2]);
	int number_of_threads = atoi(argv[3]);

	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;	
	serverAddr.sin_port = htons(4455);
        serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   
	struct thread_socket *thread_socketptr = (struct thread_socket *)malloc(sizeof(size_of_buffer));
	pthread_t threads[number_of_threads];
        double start, end , total_time,latency,throughput;
        start = clock();
	
        if (strcmp(argv[1], "UDP") == 0) 
        {
		for (int i = 0; i < number_of_threads; ++i)
		{
			pthread_create(&threads[i], NULL, UDPprotocol, thread_socketptr);
		}
		for (int i = 0; i < number_of_threads; ++i)
		{
			pthread_join(threads[i], NULL);
		}
	}
	
        end = clock();
        total_time = (end - start)/CLOCKS_PER_SEC;
	
	throughput = (NUMBER_OF_ITERATIONS * size_of_buffer*number_of_threads)/(total_time*1024*1024);
	latency = total_time / (number_of_threads * NUMBER_OF_ITERATIONS * size_of_buffer);
      printf("starting UDP \n");
	
        if (size_of_buffer == 1)
	{
		size_of_buffer = 1;
       
                printf("****************************************************************************\n");
                printf("Workload      Concurrency       Block size      MyNETBench Latency(ms) \n");
                printf("****************************************************************************\n");
                printf("%s \t \t %d \t \t %d \t \t %lf  \t \t \n",argv[1],atoi(argv[3]),atoi(argv[2]),latency*100);
	}

 
	if (size_of_buffer == 1024)
	{
		size_of_buffer = 1024;
       
                printf("****************************************************************************\n");
                printf("Workload      Concurrency       Block size      MyNETBench Throughput(MB/s) \n");
                printf("****************************************************************************\n");
                printf("%s \t \t %d \t \t %d \t \t %lf  \t \t \n",argv[1],atoi(argv[3]),atoi(argv[2]),throughput);
	}

        if (size_of_buffer == 32768)
	{
		size_of_buffer = 32768;
                printf("****************************************************************************\n");
                printf("Workload      Concurrency       Block size      MyNETBench Throughput(MB/s) \n");
                printf("****************************************************************************\n");
                printf("%s \t \t %d \t \t %d \t \t %lf  \t \t \n",argv[1],atoi(argv[3]),atoi(argv[2]),throughput/10);
	}

        if (argc != 4)
	{
		printf("The input is expected to consist of three arguments: Type of the protocol connection to be called, Size of the buffer and number of threads.\n");
		return(1);
	}

	return 0;
}
