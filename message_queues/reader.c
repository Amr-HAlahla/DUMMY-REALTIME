#include "header.h"

int main(int argc, char *argv[])
{

    key_t key;
    int msgid;
    msg message;

    key = ftok(".", 65); /* Create unique key */
    msgid = msgget(key, 0666 | IPC_CREAT);

    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error\n");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        if (msgrcv(msgid, (void *)&message, MAX_BUF, 1, 0) == -1)
        {
            fprintf(stderr, "msgrcv failed with error\n");
            exit(EXIT_FAILURE);
        }
        printf("You wrote: %s", message.msg_text);
        if (strncmp(message.msg_text, "end", 3) == 0)
        {
            break;
        }
    }
    /* Remove the message queue */
    if (msgctl(msgid, IPC_RMID, 0) == -1)
    {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}