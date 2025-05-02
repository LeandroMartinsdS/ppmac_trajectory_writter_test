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
    double tmp[7] = {};

    init_buffer(frame_types, array, &frame_bytesize);
    InitSocket(host, port);
    clientSock = AcceptClient();
    do {
        // TODO: Add busy-wait here until buffer is clear to be overwritten?
        socketStatus = HandleClient(clientSock, message, frame_bytesize);

        // write_frame(frame_types, array, frame_bytesize, message)
        memcpy(tmp, message, frame_bytesize);
        for (int i=0; i<data_count; i++) {
            printf("tmp[%d] = %f", i,  tmp[i]);
        }

        memcpy(&array[0]->d, message, frame_bytesize);
        update_buffer(frame_types, array,frame_bytesize);
        // test_print_data(array, data_count);
    } while (socketStatus == 0);

    CloseSocket(clientSock);
    CloseSocket(serverSock);
    #ifndef TEST_ENV
    CloseLibrary();
    #endif // TEST_ENV
    return 0;
}
