/*
 * Creating sets of semaphoers
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

void main(void)
{
  int sem1, sem2, sem3;
  key_t ipc_key;

  ipc_key = ftok(".", 'S');

  if ((sem1 = semget(ipc_key, 3, IPC_CREAT | 0660)) == -1)
  {
    perror("semget: IPC_CREAT | 0660");
  }

  printf("sem1 identifier %d\n", sem1);

  if ((sem2 = semget(ipc_key, 3, IPC_CREAT | IPC_EXCL | 0660)) == -1)
  {
    perror("semget: IPC_CREAT | IPC_EXCL | 0660");
  }

  printf("sem2 identifier %d\n", sem2);

  if ((sem3 = semget(IPC_PRIVATE, 3, 0660)) == -1)
  {
    perror("semget: IPC_PRIVATE");
  }

  printf("sem3 identifier %d\n", sem3);

  semctl(sem1, 0, IPC_RMID, 0);
  semctl(sem2, 0, IPC_RMID, 0);
  semctl(sem3, 0, IPC_RMID, 0);

  return;
}
