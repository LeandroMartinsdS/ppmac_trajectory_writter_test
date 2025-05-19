#ifndef TRAJ_WRITTER_H
#define TRAJ_WRITTER_H

#define TEST_ENV

#ifdef TEST_ENV
    #include "Libraries/ppmac_tcp/ppmac_tcp.h"
    #include "Libraries/ppmac_ushm_buffer/ppmac_ushm_buffer.h"

#else
    #define RUN_AS_RT_APP

    #include "../../Libraries/ppmac_tcp/ppmac_tcp.h"
    #include "../../Libraries/ppmac_ushm_buffer/ppmac_ushm_buffer.h"

    #endif // TEST_ENV
    int serverSock;

    typedef enum {
        IDLE,
        WRITING,
        READY,
        EXECUTING
    } bufferStatus;

    typedef enum {
        BUFFER_A,
        BUFFER_B
        // ...
    } bufferIndex;

    #define COUNT_ENUM_ITEMS(enumType)  sizeof(enumType)/sizeof(enumType[0])
#endif // TRAJ_WRITTER_H