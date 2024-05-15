#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_NAP 4 /* Maximum number of concurrent attaches. */

static int ask();
static void catcher(int sig);
static int good_addr(char *address);

struct state
{
    int shmid;     /* shmid of attached segment */
    char *shmaddr; /* attach point */
    int shmflg;    /* flags used on attach */
};

static struct state ap[MAX_NAP]; /* Internal record of currently attached segments. */
static int nap = 0;              /* Number of currently attached segments. */
static jmp_buf segvbuf;          /* Process state save area for SIGSEGV catching. */

int main()
{
    int action;         /* action to be performed */
    char *addr;         /* address work area */
    struct state *p;    /* ptr to current state entry */
    void (*savefunc)(); /* SIGSEGV state hold area */

    fprintf(stderr, "All numeric input is expected to follow C conventions:\n");
    fprintf(stderr, "\t0x... is interpreted as hexadecimal,\n");
    fprintf(stderr, "\t0... is interpreted as octal,\n");
    fprintf(stderr, "\totherwise, decimal.\n");

    while ((action = ask()))
    {
        if (nap)
        {
            fprintf(stderr, "\nCurrently attached segment(s):\n");
            fprintf(stderr, " shmid address\n");
            fprintf(stderr, "------ ----------\n");
            p = &ap[nap - 1];
            while (p >= ap) /* print in reverse order of attachment */
            {
                fprintf(stderr, "%6d", p->shmid);
                fprintf(stderr, "%#11x", (unsigned int)p->shmaddr);
                fprintf(stderr, " Read%s\n", (p->shmflg & SHM_RDONLY) ? "-Only" : "/Write");
                p--;
            }
        }
        else
        {
            fprintf(stderr, "\nNo segments are currently attached.\n");
        }

        switch (action)
        {
        case 1: /* Shmat requested. */
            /* Verify that there is space for another attach. */
            if (nap == MAX_NAP)
            {
                fprintf(stderr, "This simple example will only allow %d attached segments.\n", MAX_NAP);
                break;
            }

            p = &ap[nap++];
            /* Get the arguments, make the call, report the results, and update the current state array. */
            fprintf(stderr, "Enter shmid of segment to attach: ");
            scanf("%d", &p->shmid);

            fprintf(stderr, "Enter shmaddr: ");
            scanf("%i", &p->shmaddr);

            fprintf(stderr, "Meaningful shmflg values are:\n");
            fprintf(stderr, "\tSHM_RDONLY = \t%#8.8o\n", SHM_RDONLY);
            fprintf(stderr, "\tSHM_RND = \t%#8.8o\n", SHM_RND);
            fprintf(stderr, "Enter shmflg value: ");
            scanf("%i", &p->shmflg);

            fprintf(stderr, "shmop: Calling shmat(%d, %#x, %#o)\n", p->shmid, (unsigned int)p->shmaddr, p->shmflg);
            p->shmaddr = shmat(p->shmid, p->shmaddr, p->shmflg);
            if (p->shmaddr == (char *)-1)
            {
                perror("shmop: shmat failed");
                nap--;
            }
            else
            {
                fprintf(stderr, "shmop: shmat returned %#8.8x\n", (unsigned int)p->shmaddr);
            }
            break;

        case 2: /* Shmdt requested. */
            /* Get the address, make the call, report the results, and make the internal state match. */
            fprintf(stderr, "Enter detach shmaddr: ");
            scanf("%i", &addr);

            int i = shmdt(addr);
            if (i == -1)
            {
                perror("shmop: shmdt failed");
            }
            else
            {
                fprintf(stderr, "shmop: shmdt returned %d\n", i);
                p = ap;
                for (int j = 0; j < nap; j++)
                {
                    if (p->shmaddr == addr)
                    {
                        *p = ap[--nap];
                    }
                    p++;
                }
            }
            break;

        case 3: /* Read from segment requested. */
            if (nap == 0)
                break;

            fprintf(stderr, "Enter address of an attached segment: ");
            scanf("%i", &addr);

            if (good_addr(addr))
            {
                fprintf(stderr, "String @ %#x is `%s'\n", (unsigned int)addr, addr);
            }
            break;

        case 4: /* Write to segment requested. */
            if (nap == 0)
                break;

            fprintf(stderr, "Enter address of an attached segment: ");
            scanf("%i", &addr);

            /* Set up SIGSEGV catch routine to trap attempts to write into a read-only attached segment. */
            savefunc = signal(SIGSEGV, catcher);

            if (setjmp(segvbuf))
            {
                fprintf(stderr, "shmop: %s: %s\n", "SIGSEGV signal caught", "Write aborted.");
            }
            else
            {
                if (good_addr(addr))
                {
                    fflush(stdin);
                    fprintf(stderr, "%s %s %#x:\n", "Enter one line to be copied", "to shared segment attached @", (unsigned int)addr);
                    gets(addr); /* write into shared segment */
                }
            }
            fflush(stdin);

            /* Restore SIGSEGV to previous condition. */
            signal(SIGSEGV, savefunc);
            break;
        }
    }

    return 0;
}

/*
** Ask for next action.
*/
static int ask()
{
    int response; /* user response */
    do
    {
        fprintf(stderr, "Your options are:\n");
        fprintf(stderr, "\t^D = exit\n");
        fprintf(stderr, "\t 0 = exit\n");
        fprintf(stderr, "\t 1 = shmat\n");
        fprintf(stderr, "\t 2 = shmdt\n");
        fprintf(stderr, "\t 3 = read from segment\n");
        fprintf(stderr, "\t 4 = write to segment\n");
        fprintf(stderr, "Enter the number corresponding to your choice: ");

        /* Preset response so "^D" will be interpreted as exit. */
        response = 0;
        scanf("%i", &response);
    } while (response < 0 || response > 4);
    return response;
}

/*
** Catch signal caused by attempt to write into shared memory segment
** attached with SHM_RDONLY flag set.
*/
static void catcher(int sig)
{
    longjmp(segvbuf, 1); /* return to write-to-segment prompt */
}

/*
** Verify that given address is the address of an attached segment.
** Return 1 if address is valid; 0 if not.
*/
static int good_addr(char *address)
{
    struct state *p; /* ptr to state of attached segment */

    for (p = ap; p < &ap[nap]; p++)
    {
        if (p->shmaddr == address)
        {
            return 1;
        }
    }
    return 0;
}
