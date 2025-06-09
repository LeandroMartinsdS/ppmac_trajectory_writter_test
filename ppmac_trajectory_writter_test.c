#include "ppmac_trajectory_writter_test.h"
#include "../../Libraries/ppmac_tcp/ppmac_tcp.h"
#include "../../Libraries/ppmac_ushm_buffer/ppmac_ushm_buffer.h"

int main() {

  	struct sched_param param;
  	int done = 0;
	struct timespec sleeptime = {0};
	sleeptime.tv_nsec = NANO_5MSEC;	// #defines NANO_1MSEC, NANO_5MSEC & NANO_10MSEC are defined

  	#ifndef RUN_AS_RT_APP	
		//-----------------------------
		// Runs as a NON RT Linux APP
		//-----------------------------
		param.__sched_priority = 0;
		pthread_setschedparam(pthread_self(),  SCHED_OTHER, &param);
  	#else 
		//---------------------------------------------------------------
		// Runs as a RT Linux APP with the same scheduling policy as the
		// Background script PLCs
		// To run at a recommended lower priority use BACKGROUND_RT_PRIORITY - 10
		// To run at a higher priority use BACKGROUND_RT_PRIORITY + 1
		//---------------------------------------------------------------------
		param.__sched_priority =  BACKGROUND_RT_PRIORITY - 10;
		pthread_setschedparam(pthread_self(),  SCHED_FIFO, &param);
  	#endif

    InitLibrary();  // Required for accessing Power PMAC library

    Point *array[9];
    char *frame_types = "ddddddddd";    // The first double is unpacking two integers
    size_t frame_bytesize = 0x00000000;

    char *host = "172.23.59.7";
    int port = 8080;
    size_t data_count = 9;
    int clientSock, socketStatus;
    char message[BUFFSIZE];

    size_t frames_per_buffer = 50000;
    unsigned int write_idx = 0;
    unsigned int traj_idx = 0;

    unsigned int buffer_num = COUNT_ENUM_ITEMS(bufferIndex);

    bufferIndex buffer_idx = BUFFER_A;
//    bufferStatus buffer_status[2] = 0;

    size_t address_offset = 0;
    void* base_memory = (void *)USHM_BASE_ADDR;
    InitSocket(host, port);
    printf("Socket initialized\n");
    // TODO: Update AcceptClient to receive clientSock and return status
    clientSock = AcceptClient();
    printf("Client accepted\n");
    do {
        init_buffer(frame_types, array, &frame_bytesize, base_memory);
        //printf("Buffer initialized\n");

        while (write_idx < frames_per_buffer) { // TODO : Fix - it doesn't satisfy the 
                                                // condition where the last file has less frames
            // if (frames_per_buffer == 0) {
            //     break;
            // }

       
            // while (buffer_status != IDLE) { /*sleep*/};
            
            // buffer_status = WRITING;
            socketStatus = HandleClient(clientSock, message, frame_bytesize);
            
            // TO DO: Improve this to jump columns in the middle(?); permit     

//            memcpy(&array[0]->d, message, frame_bytesize);
//            update_buffer(frame_types, array,frame_bytesize);
            write_frame(frame_types, array, frame_bytesize, message);
            
            write_idx++;
            traj_idx++;
//            printf("Buffer[%d] - Buffer Points: %d; Total Points: %d\n", buffer_idx, write_idx, traj_idx);
        }

//        buffer_status[buffer_idx] = READY;
        buffer_idx = (buffer_idx+1) % buffer_num;
        address_offset = buffer_idx * frame_bytesize * frames_per_buffer;
//        printf("mem: %d\n",base_memory);


        // TEST
        /////////////////////
//        init_buffer(frame_types, array, &frame_bytesize, base_memory);
//        int i;
//        for(i = 0; i < frames_per_buffer; i++) {
//            printf("%d:\n", i);
//            test_print_buffer(frame_types, array, frame_bytesize);
//            printf("\n");
//        }
        // END of TEST
        /////////////////////

        base_memory = (void *)USHM_BASE_ADDR + address_offset;
        write_idx = 0;
        // TODO Add response to client
    } while (socketStatus == 0);
    CloseSocket(clientSock);
    CloseSocket(serverSock);
    printf("Sockets closed\n");
    CloseLibrary();
    return 0;
}
