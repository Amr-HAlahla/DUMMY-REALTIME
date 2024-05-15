#include "local2.h"

void report_and_exit(const char *msg)
{
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

int main()
{
  int i, qid, pid;
  char strk[100], stri[100], strt[100];
  message msg;

  key_t key = ftok(".", 'S');

  printf("Parent => key = %d\n", key);

  if (key < 0)
    report_and_exit("couldn't get key...");

  if ((qid = msgget(key, 0666 | IPC_CREAT)) == -1)
  {
    report_and_exit("couldn't get queue id...");
  }

  char *payloads[] = {"msg1", "msg2", "msg3", "msg4", "msg5", "msg6"};
  long types[] = {1, 1, 2, 2, 3, 3}; /* each must be > 0 */

  sprintf(strk, "%d", key);

  for (int i = 0; i < 6; i++)
  {

    pid = fork();

    if (pid == 0)
    {
      sprintf(stri, "%d", i + 1);
      sprintf(strt, "%ld", types[i]);
      execl("./child", "./child", strk, stri, strt, (char *)0);
      perror("execl -- child");
    }
    msg.mesg_type = types[i];
    strcpy(msg.mesg_text, payloads[i]);

    msgsnd(qid, &msg, sizeof(msg), IPC_NOWAIT); /* don't block */
    printf("%s sent as type %i\n", msg.mesg_text, (int)msg.mesg_type);
  }

  sleep(2);
  msgctl(qid, IPC_RMID, NULL);
  return 0;
}
