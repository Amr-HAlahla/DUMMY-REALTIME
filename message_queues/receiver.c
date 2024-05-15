#include "header.h"

int main(int argc, char *argv[])
{
    int running = 1;
    int msgid;
    msg message;
    long int msg_to_receive = 0;
    /* Create the message queue */
    msgid = msgget((key_t)12345, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error\n");
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        if (msgrcv(msgid, (void *)&message, MAX_BUF, msg_to_receive, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error\n");
            exit(EXIT_FAILURE);
        }
        printf("You wrote: %s", message.msg_text);
        if (strncmp(message.msg_text, "end", 3) == 0)
        {
            running = 0;
        }
    }
    /* Remove the message queue */
    if (msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}