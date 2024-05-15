#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

static void do_shmctl(int shmid, int cmd, struct shmid_ds *buf);

int main()
{
    int cmd;                  /* command code for shmctl() */
    int shmid;                /* segment ID */
    struct shmid_ds shmid_ds; /* shared memory data structure to hold results */

    printf("All numeric input is expected to follow C conventions:\n");
    printf("\t0x... is interpreted as hexadecimal,\n");
    printf("\t0... is interpreted as octal,\n");
    printf("\totherwise, decimal.\n");

    /* Get shmid and cmd. */
    printf("Enter the shmid for the desired segment: ");
    scanf("%i", &shmid);
    printf("Valid shmctl cmd values are:\n");
    printf("\tIPC_RMID =\t%d\n", IPC_RMID);
    printf("\tIPC_SET =\t%d\n", IPC_SET);
    printf("\tIPC_STAT =\t%d\n", IPC_STAT);
    printf("\tSHM_LOCK =\t%d\n", SHM_LOCK);
    printf("\tSHM_UNLOCK =\t%d\n", SHM_UNLOCK);
    printf("Enter the desired cmd value: ");
    scanf("%i", &cmd);

    switch (cmd)
    {
    case IPC_STAT:
        /* Get shared memory segment status. */
        break;
    case IPC_SET:
        /* Set owner UID and GID and permissions. */
        /* Get and print current values. */
        do_shmctl(shmid, IPC_STAT, &shmid_ds);
        /* Set UID, GID, and permissions to be loaded. */
        printf("\nEnter shm_perm.uid: ");
        scanf("%u", &shmid_ds.shm_perm.uid);
        printf("Enter shm_perm.gid: ");
        scanf("%u", &shmid_ds.shm_perm.gid);
        printf("Note: Keep read permission for yourself.\n");
        printf("Enter shm_perm.mode: ");
        scanf("%u", &shmid_ds.shm_perm.mode);
        break;
    case IPC_RMID:
        /* Remove the segment when the last attach point is detached. */
        break;
    case SHM_LOCK:
        /* Lock the shared memory segment. */
        break;
    case SHM_UNLOCK:
        /* Unlock the shared memory segment. */
        break;
    default:
        /* Unknown command will be passed to shmctl. */
        break;
    }
    do_shmctl(shmid, cmd, &shmid_ds);
    return 0;
}

/*
 * Display the arguments being passed to shmctl(), call shmctl(),
 * and report the results. If shmctl() fails, do not return; this
 * example doesn't deal with errors, it just reports them.
 */
static void do_shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
    int rtrn; /* hold area */

    printf("shmctl: Calling shmctl(%d, %d, buf)\n", shmid, cmd);
    if (cmd == IPC_SET)
    {
        printf("\tbuf->shm_perm.uid == %d\n", buf->shm_perm.uid);
        printf("\tbuf->shm_perm.gid == %d\n", buf->shm_perm.gid);
        printf("\tbuf->shm_perm.mode == %#o\n", buf->shm_perm.mode);
    }
    if ((rtrn = shmctl(shmid, cmd, buf)) == -1)
    {
        perror("shmctl: shmctl failed");
        exit(1);
    }
    else
    {
        printf("shmctl: shmctl returned %d\n", rtrn);
    }
    if (cmd != IPC_STAT && cmd != IPC_SET)
        return;

    /* Print the current status. */
    printf("\nCurrent status:\n");
    printf("\tshm_perm.uid = %d\n", buf->shm_perm.uid);
    printf("\tshm_perm.gid = %d\n", buf->shm_perm.gid);
    printf("\tshm_perm.cuid = %d\n", buf->shm_perm.cuid);
    printf("\tshm_perm.cgid = %d\n", buf->shm_perm.cgid);
    printf("\tshm_perm.mode = %#o\n", buf->shm_perm.mode);
    printf("\tshm_perm.key = %#x\n", buf->shm_perm.__key);
    printf("\tshm_segsz = %lu\n", buf->shm_segsz);
    printf("\tshm_nattch = %lu\n", buf->shm_nattch);
    printf("\tshm_lpid = %d\n", buf->shm_lpid);
    printf("\tshm_cpid = %d\n", buf->shm_cpid);
    printf("\tshm_atime = %s", buf->shm_atime ? ctime(&buf->shm_atime) : "Not Set\n");
    printf("\tshm_dtime = %s", buf->shm_dtime ? ctime(&buf->shm_dtime) : "Not Set\n");
    printf("\tshm_ctime = %s", ctime(&buf->shm_ctime));
}