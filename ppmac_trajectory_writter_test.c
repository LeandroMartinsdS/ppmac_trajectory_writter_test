#include "ppmac_trajectory_writter_test.h"

int main() {
    #ifdef TEST_ENV
    pushm = (void *)malloc(1024*1024);          // HACK
    // pshm = (void *)malloc(sizeof(struct SHM));  // HACK

    #else
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
    #endif // RUN_AS_RT_APP

    InitLibrary();  // Required for accessing Power PMAC library
    #endif // TEST_ENV

    Point *array[7] = {};
    char *frame_types = "ddddddd";
    size_t frame_bytesize = 0x00000000;

    char *host = "127.0.0.1";
    int port = 8080;
    size_t data_count = 7;
    int clientSock, socketStatus;
    char message[BUFFSIZE];

    size_t frames_per_buffer = 10000;
    unsigned int write_idx = 0;
    unsigned int traj_idx = 0;

    unsigned int buffer_num = COUNT_ENUM_ITEMS(bufferIndex);

    bufferIndex buffer_idx = BUFFER_A;
    bufferStatus buffer_status[2] = 0;

    size_t address_offset;
    void* base_memory;
    InitSocket(host, port);
    // TODO: Update AcceptClient to receive clientSock and return status
    clientSock = AcceptClient();
    do {
        while (write_idx < frames_per_buffer) {
            // if (frames_per_buffer == 0) {
            //     break;
            // }

            address_offset = buffer_idx * frame_bytesize * frames_per_buffer;
            base_memory = (void *)USHM_BASE_ADDR + address_offset;
            init_buffer(frame_types, array, &frame_bytesize, base_memory);
            // while (buffer_status != IDLE) { /*sleep*/};
            // buffer_status = WRITING;
            socketStatus = HandleClient(clientSock, message, frame_bytesize);
            // write_frame(frame_types, array, frame_bytesize, message)
            memcpy(&array[0]->d, message, frame_bytesize);
            update_buffer(frame_types, array,frame_bytesize);
            write_idx++;
            traj_idx++;
        }

        buffer_status = READY;
        buffer_idx = (buffer_idx+1) % buffer_num;
        write_idx = 0;
        // TODO Add response to client
    } while (socketStatus == 0);

    CloseSocket(clientSock);
    CloseSocket(serverSock);
    #ifndef TEST_ENV
    CloseLibrary();
    #endif // TEST_ENV
    return 0;
}
