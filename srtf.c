// OS Programming Lab 4: SJF with premption (SRTF) Schedulling
// 20BCS041
// Mohammed Abbas Ansari
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structures:

// Used to build priority queues based on arrival time and burst time respectively.
struct node // node for process
{
    char process[10]; // process name
    int at;           // arrival time
    int bt;           // burst time
    struct node *next;
};

// Used to build linked list to print Gantt Chart and calculate process metrics. (Running Queue)
struct exec // node for execution instance of process by the CPU
{
    char process[10]; // name of the process being executed
    int at;           // arrival time of process
    int start;        // time of start of execution instance
    int end;          // time of end of execution instance
    struct exec *next;
};
// A single exection instance is the time given by the CPU for executing the process before preempting to a another process.

// Functions:
void enqueueAT(struct node **start, struct node **new);    // enqueuing function based on arrival time
void enqueueBT(struct node **start, struct node **new);    // enqueuing function based on burst time
void appendExInst(struct exec **start, struct exec **new); // function to append a new execution instance to gantt linked list (Running Queue)
void displayQ(struct node *start);                         // function to display queue
void srtf();                                               // function which executes SJF with pre-emption schedulling algorithm
void display(int n);                                       // function which calculates process metrics and prints them
void chart();                                              // printing gantt chart

struct node *queueAT = NULL; // priority queue based on arrival time
struct node *queueBT = NULL; // priority queue based on burst time (Ready Queue)
struct exec *gantt = NULL;   // a linked list representing Gantt chart (Running Queue)

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 4");
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);
    printf("Enter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        struct node *new = (struct node *)malloc(sizeof(struct node));
        printf("\nEnter process name: ");
        scanf("%s%*c", new->process);
        printf("Arrival time: ");
        scanf("%d%*c", &new->at);
        printf("Burst time: ");
        scanf("%d%*c", &new->bt);
        new->next = NULL;
        enqueueAT(&queueAT, &new); // queing new process based on arrival time
    }
    // displayQ(queueAT);
    srtf();     // function which executes SJF with pre-emption schedulling algorithm
    chart();    // printing gantt chart
    display(n); // function which calculates process metrics and prints them

    return 0;
}

// Function Definitons: 

void enqueueAT(struct node **start, struct node **new) // enqueuing based on arrival time
{
    if (!*start) // queue empty
    {
        *start = *new;
    }
    else if ((*new)->at < (*start)->at) // enqueing at head
    {
        (*new)->next = *start;
        *start = *new;
    }
    else
    {
        struct node *t = *start;
        while (t->next && (*new)->at >= t->next->at)
        {
            t = t->next;
        }
        (*new)->next = t->next;
        t->next = *new;
    }
}

void enqueueBT(struct node **start, struct node **new) // enqueuing based on burst time
{
    if (!*start) // queue empty
    {
        *start = *new;
    }
    else if ((*new)->bt < (*start)->bt) // enqueing at head
    {
        (*new)->next = *start;
        *start = *new;
    }
    else
    {
        struct node *t = *start;
        while (t->next && (*new)->bt >= t->next->bt)
        {
            t = t->next;
        }
        (*new)->next = t->next;
        t->next = *new;
    }
}

void appendExInst(struct exec **start, struct exec **new) // function to append a new execution instance to gantt linked list
{
    if (*start == NULL) // list is empty
    {
        *start = *new; // append at beginning
    }
    else
    {
        struct exec *t = *start;
        while (t->next) // going till last node in the linked list
            t = t->next;
        (*new)->next = t->next;
        t->next = *new; // append at end
    }
}

void displayQ(struct node *start) // function to display queue
{
    struct node *t = start;
    while (t)
    {
        printf("|%s|%d|%d|-> ", t->process, t->at, t->bt);
        t = t->next;
    }
    printf("\n");
}

void displayL()
{
    struct exec *t = gantt;
    printf("\nGantt list: ");
    int i = 0;
    while (t != NULL)
    {
        printf("%s ->", t->process);
        t = t->next;
        i++;
        if (i > 10)
            break;
    }
}

void srtf() // function which executes SJF with pre-emption schedulling algorithm
{
    // printf("\nExecuting SRTF Function");
    int curTime = 0;
    while (queueAT != NULL)
    {
        if (queueBT == NULL)
            curTime = queueAT->at; // setting current time to latest process

        // enqueing processes that have arrived uptil curTime into queueBT
        while (queueAT != NULL && queueAT->at <= curTime)
        {
            struct node *p = queueAT; // dequeing from AT queue
            queueAT = queueAT->next;
            p->next = NULL;
            enqueueBT(&queueBT, &p); // enqueing into BT queue
        }
        // printf("\nt: %d. queueBT: ", curTime);
        // displayQ(queueBT);
        while (queueBT != NULL)
        {
            struct node *d = queueBT; // dequeing from BT queue
            queueBT = queueBT->next;
            struct exec *i = (struct exec *)malloc(sizeof(struct exec)); // new execution instance node
            strcpy(i->process, d->process);                              // copying process name
            i->at = d->at;                                               // setting arrival time
            i->start = curTime;                                          // setting start time of execution instance to current time
            i->next = NULL;
            // printf("\nDequed %s", i->process);
            if (queueAT != NULL) // arrival time queue is not empty, therefore preemption can occur
            {
                while (d->bt != 0) // execute until bt = 0
                {
                    d->bt--; // executing process d for single unit of time
                    curTime++;

                    if (queueAT != NULL && curTime >= queueAT->at) // new processes have arrived, hence possibility of preemption
                    {
                        while (queueAT != NULL && queueAT->at <= curTime) // enqueing all the new processes into BT queue
                        {
                            struct node *p = queueAT; // dequeing from AT queue
                            queueAT = queueAT->next;
                            p->next = NULL;
                            enqueueBT(&queueBT, &p); // enqueing into BT queue
                        }
                        // printf("\nt: %d. queueBT: ", curTime);
                        //  displayQ(queueBT);
                        if (d->bt > queueBT->bt) // if a newer shorter process has arrived, then preempt and do context switching
                            break;
                    }
                }
                if (d->bt != 0)
                {
                    enqueueBT(&queueBT, &d); // process d has not been completely executed, therefore enqueing it back.
                }
                else
                {
                    free(d);
                    // burst time of process d = 0 which means it has been completely executed, hence freeing it.
                }
                i->end = curTime;         // setting end time of execution instance
                appendExInst(&gantt, &i); // appending the execution instance to gantt linked list
                // displayL();
            }
            else // arrival time queue is empty, therefore no need of preemption (Same as SJF)
            {
                curTime += d->bt; // executing the whole process
                i->end = curTime;
                free(d); // freeing the completed process
                appendExInst(&gantt, &i);
                // displayL();
            }

            if (queueAT != NULL) // arrival time queue is not yet empty, therefore queueBT can still grow
                break;
        }
    }
}

void display(int n) // function which calculates process metrics and prints them
{
    printf("\nDisplaying metrics:");
    printf("\nProcess BT AT CT WT  TAT RT\n");
    int total_ct = 0, total_wt = 0, total_tat = 0, total_rt = 0;
    while (gantt != NULL)
    {
        // displayL();
        char pname[10];
        strcpy(pname, gantt->process);
        int at = gantt->at;                     // arrival time
        int rt = gantt->start;                  // response time
        int wt = gantt->start - at;             // waiting time
        int ct = gantt->end;                    // completion time
        int bt = (gantt->end) - (gantt->start); // burst time
        struct exec *p = gantt;
        gantt = gantt->next;
        free(p); // freeing execution instance
        // displayL();
        struct exec *t = gantt;
        if (t != NULL)
        {
            while (t->next != NULL)
            {
                if (strcmp(pname, t->next->process) == 0)
                {
                    // found another execution instance of the same process
                    wt += t->next->start - ct;               // incrementing the waiting time when other programs were being executed
                    bt += (t->next->end) - (t->next->start); // incrementing burst time
                    ct = t->next->end;
                    p = t->next;
                    t->next = t->next->next;
                    free(p);
                    // displayL();
                    continue;
                }
                t = t->next;
            }
        }
        int tat = ct - at; // turn around time
        total_ct += ct;
        total_tat += tat;
        total_wt += wt;
        total_rt += rt;
        printf("%s      %2d %2d %2d %2d  %2d %2d\n", pname, bt, at, ct, wt, tat, rt);
    }
    printf("\n\nAvg CT = %.2f", total_ct / (float)n);
    printf("\nAvg WT = %.2f", total_wt / (float)n);
    printf("\nAvg TAT = %.2f", total_tat / (float)n);
    printf("\nAvg RT = %.2f\n", total_rt / (float)n);
}

void chart() // printing gantt chart
{
    printf("\nGantt Chart:\n");
    struct exec *t = gantt;
    int time = gantt->at; // setting time to the arrival time of first process to be executed
    printf("|");
    while (t != NULL)
    {
        if (time < t->start)
        {
            for (int i = 0; i < (t->start - time); i++)
                printf(" ");
            printf("|");
            time = t->start;
        }
        int bt = t->end - t->start;

        for (int i = 0; i < bt - 1; i++)
            printf(" ");
        printf("%s", t->process);
        for (int i = 0; i < bt - 1; i++)
            printf(" ");
        printf("|");
        time += bt;
        t = t->next;
    }
    time = gantt->at;
    t = gantt;
    printf("\n%d ", t->start);
    while (t)
    {
        if (time < t->at)
        {
            for (int i = 0; i < (t->at - time) - 1; i++)
            {
                printf(" ");
            }
            if (t->start < 9)
                printf("%d ", t->start);
            else
                printf("%d", t->start);
            time = t->start;
        }
        int bt = (t->end) - (t->start);
        for (int i = 0; i < 2 * (bt)-1; i++)
            printf(" ");
        if (t->end < 9)
            printf("%d ", t->end);
        else
            printf("%d", t->end);
        time += bt;
        t = t->next;
    }
    printf("\n");
}

/*
20BCS041 Mohammed Abbas Ansari
OS Program 4
Enter the number of processes: 6
Enter the details of the processes.

Enter process name: p1
Arrival time: 0
Burst time: 8

Enter process name: p2
Arrival time: 1
Burst time: 4

Enter process name: p3
Arrival time: 2
Burst time: 2

Enter process name: p4
Arrival time: 3
Burst time: 1

Enter process name: p5
Arrival time: 4
Burst time: 3

Enter process name: p6
Arrival time: 5
Burst time: 2

Gantt Chart:
|p1|p2| p3 |p4| p6 |  p2  |  p5  |      p1      |
0  1  2    4  5    7      10     13             20

Displaying metrics:
Process BT AT CT WT  TAT RT
p1       8  0 20 12  20  0
p2       4  1 10  5   9  1
p3       2  2  4  0   2  2
p4       1  3  5  1   2  4
p6       2  5  7  0   2  5
p5       3  4 13  6   9 10


Avg CT = 9.83
Avg WT = 4.00
Avg TAT = 7.33
Avg RT = 3.67
*/