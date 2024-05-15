#ifndef __LOCAL_H_
#define __LOCAL_H_

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct mesg_buffer {
  long mesg_type;
  char mesg_text[100];
} message;

#endif
