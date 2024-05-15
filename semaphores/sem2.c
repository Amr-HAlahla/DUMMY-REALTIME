/*
 * The semctl system call
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NS 3

union semun
{
  int val;
  struct semid_ds *buf;
  ushort *array;
};

void main(void)
{
  int sem_id, sem_value, i;
  key_t ipc_key;
  struct semid_ds sem_buf;
  static ushort sem_array[NS] = {3, 1, 4};
  union semun arg;

  ipc_key = ftok(".", 'S');

  /*
   * Create the semaphore
   */

  if ((sem_id = semget(ipc_key, NS, IPC_CREAT | 0660)) == -1)
  {
    perror("semget: IPC_CREAT | 0660");
    exit(1);
  }

  printf("Semaphore identifier %d\n", sem_id);

  /*
   * Set arg (the union) to the addr of the storage location
   * for returned semid_ds values.
   */
  arg.buf = &sem_buf;

  if (semctl(sem_id, 0, IPC_STAT, arg) == -1)
  {
    perror("semctl: IPC_STAT");
    exit(2);
  }

  printf("Created %s", ctime(&sem_buf.sem_ctime));

  /*
   * Set arg (the union) to the addr of the initializing
   * vector.
   */
  arg.array = sem_array;

  /* Initialize the semaphore using the setall command */
  if (semctl(sem_id, 0, SETALL, arg) == -1)
  {
    perror("semctl: SETALL");
    exit(3);
  }

  for (i = 0; i < NS; i++)
  { /* display contents */
    if ((sem_value = semctl(sem_id, i, GETVAL, 0)) == -1)
    {
      perror("semctl: GETVAL");
      exit(4);
    }

    printf("Semaphore %d has value of %d\n", i, sem_value);
  }

  if (semctl(sem_id, 0, IPC_RMID, 0) == -1)
  {
    perror("semctl: IPC_RMID"); /* remove semaphore */
    exit(5);
  }

  return;
}
