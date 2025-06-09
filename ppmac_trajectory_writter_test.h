#ifndef TRAJ_WRITTER_H
#define TRAJ_WRITTER_H

//#define RUN_AS_RT_APP

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

#define COUNT_ENUM_ITEMS(enumType)  2 //sizeof(enumType)/sizeof(enumType[0])


#endif // TRAJ_WRITTER_H