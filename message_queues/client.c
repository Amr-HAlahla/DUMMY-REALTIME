/*
  CLIENT ... sends messages to the server
  */
#include "local.h"

int main()
{
  key_t key;
  pid_t cli_pid;
  int mid, n;
  MESSAGE msg;
  static char m_key[10];

  cli_pid = getpid();

  if ((key = ftok(".", SEED)) == -1)
  {
    perror("Client: key generation");
    printf("Error number: %d\n", errno);
    return 1;
  }

  if ((mid = msgget(key, 0)) == -1)
  {                                      /* message queue exists */
    mid = msgget(key, IPC_CREAT | 0660); /* create message queue if not exist */
    switch (fork())
    {
    case -1:
      perror("Client: fork");
      return 2;

    case 0:
      sprintf(m_key, "%d", mid);
      execlp("./server", "server", m_key, "&", 0); /* "&" to run the server in the background */
      perror("Client: exec");
      return 3;
    }
  }

  while (1)
  {
    msg.msg_to = SERVER;
    msg.msg_fm = cli_pid;
    // write(fileno(stdout), "cmd> ", 6);
    write(fileno(stdout), "cmd> ", strlen("cmd> "));
    memset(msg.buffer, 0x0, BUFSIZ);

    if ((n = read(fileno(stdin), msg.buffer, BUFSIZ)) == 0)
      break;

    n += sizeof(msg.msg_fm);
    n += sizeof(msg.msg_to);

    if (msgsnd(mid, &msg, n, 0) == -1)
    {
      perror("Client: msgsend");
      return 4;
    }

    if ((n = msgrcv(mid, &msg, BUFSIZ, cli_pid, 0)) != -1)
      write(fileno(stdout), msg.buffer, n);
  }
  // msgsnd(mid, &msg, 0, 0);
  return 0;
}
