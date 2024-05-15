#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#define MAX_BUF 64
typedef struct
{
    long int msg_type;
    char msg_text[MAX_BUF];
} msg;
