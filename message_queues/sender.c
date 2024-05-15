#include "header.h"

int main(int argc, char *argv[])
{
    int running = 1;
    int msgid;
    msg message;
    char buffer[MAX_BUF];
    /* Create the message queue */
    msgid = msgget((key_t)12345, 0666 | IPC_CREAT);

    if (msgid == -1)
    {
        fprintf(stderr, "msgget failed with error\n");
        exit(EXIT_FAILURE);
    }
    while (running)
    {
        printf("Enter some text, or type 'end' to stop: ");
        fgets(buffer, MAX_BUF, stdin);
        message.msg_type = 1; // Note: msg_type is not used in this example
        strcpy(message.msg_text, buffer);
        if (msgsnd(msgid, (void *)&message, MAX_BUF, 0) == -1)
        {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
        if (strncmp(buffer, "end", 3) == 0)
        {
            running = 0;
        }
    }
}
