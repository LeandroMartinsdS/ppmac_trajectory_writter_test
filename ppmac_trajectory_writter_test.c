#include "ppmac_trajectory_writter_test.h"

int main() {
    #ifdef TEST_ENV
    pushm = (void *)malloc(sizeof(pushm));  // HACK
    pshm = (void *)malloc(sizeof(pshm));    // HACK

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

    char *host = "127.0.0.1";
    int port = 8080;

    InitSocket(host, port);
    AcceptClient();
    CloseSocket(serverSock);

    #ifdef TEST_ENV
    free(pushm);
    #else
    CloseLibrary();
    #endif // TEST_ENV
    return 0;
}
