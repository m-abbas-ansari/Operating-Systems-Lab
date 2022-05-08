// OS Programming Lab 7: Preemptive Priority Schedulling
// 20BCS041
// Mohammed Abbas Ansari
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structures:

// Used to build priority queues based on arrival time and burst time respectively.
struct node // node for process
{
    int pid; // process name
    int at;  // arrival time
    int bt;  // burst time
    int pr;  // priority
    struct node *next;
};

// Used to build linked list to print Gantt Chart and calculate process metrics. (Running Queue)
struct exec // node for execution instance of process by the CPU
{
    int pid;   // name of the process being executed
    int at;    // arrival time of process
    int start; // time of start of execution instance
    int end;   // time of end of execution instance
    struct exec *next;
};
// A single exection instance is the time given by the CPU for executing the process before preempting to a another process.

// Functions:
void enqueueAT(struct node **start, struct node **new);    // enqueuing function based on arrival time
void enqueuePr(struct node **start, struct node **new);    // enqueuing function based on priority
void appendExInst(struct exec **start, struct exec **new); // function to append a new execution instance to gantt linked list (Running Queue)
void displayQ(struct node *start);                         // function to display queue
void prioritySchedulling();                                // function which executes Priority with pre-emption schedulling algorithm
void display(int n);                                       // function which calculates process metrics and prints them
void chart();                                              // printing gantt chart

struct node *queueAT = NULL; // priority queue based on arrival time
struct node *queuePr = NULL; // priority queue based on priority (Ready Queue)
struct exec *gantt = NULL;   // a linked list representing Gantt chart (Running Queue)

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 7");
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);
    printf("Enter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        struct node *new = (struct node *)malloc(sizeof(struct node));
        printf("\nEnter process id: ");
        scanf("%d%*c", &new->pid);
        printf("Arrival time: ");
        scanf("%d%*c", &new->at);
        printf("Burst time: ");
        scanf("%d%*c", &new->bt);
        printf("Priority: ");
        scanf("%d%*c", &new->pr);
        new->next = NULL;
        enqueueAT(&queueAT, &new); // queing new process based on arrival time
    }
    // displayQ(queueAT);
    prioritySchedulling(); // function which executes Priority with pre-emption schedulling algorithm
    chart();               // printing gantt chart
    display(n);            // function which calculates process metrics and prints them

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

void enqueuePr(struct node **start, struct node **new) // enqueuing based on priority
{
    if (!*start) // queue empty
    {
        *start = *new;
    }
    else if ((*new)->pr < (*start)->pr) // enqueing at head
    {
        (*new)->next = *start;
        *start = *new;
    }
    else
    {
        struct node *t = *start;
        while (t->next && (*new)->pr >= t->next->pr)
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
        printf("|p%d|%d|%d|-> ", t->pid, t->at, t->bt);
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
        printf("p%d ->", t->pid);
        t = t->next;
        i++;
        if (i > 10)
            break;
    }
}

void prioritySchedulling() // function which executes Priority with pre-emption schedulling algorithm
{
    // printf("\nExecuting prioritySchedulling Function");
    int curTime = 0;
    while (queueAT != NULL)
    {
        if (queuePr == NULL)
            curTime = queueAT->at; // setting current time to latest process

        // enqueing processes that have arrived uptil curTime into queuePr
        while (queueAT != NULL && queueAT->at <= curTime)
        {
            struct node *p = queueAT; // dequeing from AT queue
            queueAT = queueAT->next;
            p->next = NULL;
            enqueuePr(&queuePr, &p); // enqueing into Priorirty queue
        }
        // printf("\nt: %d. queuePr: ", curTime);
        // displayQ(queuePr);
        while (queuePr != NULL)
        {
            struct node *d = queuePr; // dequeing from BT queue
            queuePr = queuePr->next;
            struct exec *i = (struct exec *)malloc(sizeof(struct exec)); // new execution instance node
            i->pid = d->pid;                                             // copying process name
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
                        while (queueAT != NULL && queueAT->at <= curTime) // enqueing all the new processes into Prioriry queue
                        {
                            struct node *p = queueAT; // dequeing from AT queue
                            queueAT = queueAT->next;
                            p->next = NULL;
                            enqueuePr(&queuePr, &p); // enqueing into Priority queue
                        }
                        // printf("\nt: %d. queuePr: ", curTime);
                        //  displayQ(queuePr);
                        if (d->pr > queuePr->pr) // if a process with higher priority has arrived, then preempt and do context switching
                            break;
                    }
                }
                if (d->bt != 0)
                {
                    enqueuePr(&queuePr, &d); // process d has not been completely executed, therefore enqueing it back.
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
            else // arrival time queue is empty, therefore no need of preemption
            {
                curTime += d->bt; // executing the whole process
                i->end = curTime;
                free(d); // freeing the completed process
                appendExInst(&gantt, &i);
                // displayL();
            }

            if (queueAT != NULL) // arrival time queue is not yet empty, therefore queuePr can still grow
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
        int pid = gantt->pid;                   // process id
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
                if (pid == t->next->pid)
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
        printf("p%2d      %2d %2d %2d %2d  %2d %2d\n", pid, bt, at, ct, wt, tat, rt);
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
        printf("p%d", t->pid);
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
➜  OS ./pre_priority

20BCS041 Mohammed Abbas Ansari
OS Program 7
Enter the number of processes: 7
Enter the details of the processes.

Enter process id: 1
Arrival time: 0
Burst time: 1
Priority: 2

Enter process id: 2
Arrival time: 1
Burst time: 7
Priority: 6

Enter process id: 3
Arrival time: 2
Burst time: 3
Priority: 3

Enter process id: 4
Arrival time: 3
Burst time: 6
Priority: 5

Enter process id: 5
Arrival time: 4
Burst time: 5
Priority: 4

Enter process id: 6
Arrival time: 5
Burst time: 15
Priority: 10

Enter process id: 7
Arrival time: 15
Burst time: 8
Priority: 9

Gantt Chart:
|p1|p2|  p3  |    p5    |     p4     |     p2     |       p7       |              p6              |
0  1  2      5          10           16           22               30                             45

Displaying metrics:
Process BT AT CT WT  TAT RT
p 1       1  0  1  0   1  0
p 2       7  1 22 14  21  1
p 3       3  2  5  0   3  2
p 5       5  4 10  1   6  5
p 4       6  3 16  7  13 10
p 7       8 15 30  7  15 22
p 6      15  5 45 25  40 30


Avg CT = 18.43
Avg WT = 7.71
Avg TAT = 14.14
Avg RT = 10.00
*/