#define TEST_ENV

#ifdef TEST_ENV
    # define MAX_P 65536
    struct SHM {
        double P[MAX_P];            // Global P variable Array
    };
    struct SHM  *pshm;              // Pointer to shared memory
    void        *pushm;             // Pointer to user shared memory

    #include "Libraries/ppmac_tcp/ppmac_tcp.h"
    #include "Libraries/ppmac_ushm_buffer/ppmac_ushm_buffer.h"
#else
    #define RUN_AS_RT_APP

    #include "../../Libraries/ppmac_tcp/ppmac_tcp.h"
    #include "../../Libraries/ppmac_ushm_buffer/ppmac_ushm_buffer.h"
#endif // TEST_ENV
