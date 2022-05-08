// OS Programming Lab 6: Non-Preemptive Priority Schedulling
// 20BCS041
// Mohammed Abbas Ansari
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures:

// Used to build priority queues based on arrival time.
struct node // node for process block
{
    int pid;      // process id
    int priority; // process priority
    int at;       // arrival time
    int bt;       // burst time
    struct node *next;
};

// Used to build Running queue to print Gantt Chart and calculate process metrics.
struct exec // node for execution instance of process by the CPU
{
    int pid;      // process id
    int priority; // process priority
    int at;       // arrival time of process
    int start;    // time of start of execution instance
    int end;      // time of end of execution instance
    struct exec *next;
};

// Functions:
void enqueueAT(struct node **start, struct node **new);     // enqueuing function based on arrival time
void enqueueReadyQ(struct node **start, struct node **new); // function to enqueue processes into ready queue based on priority
void enqueuRunQ(struct exec **start, struct exec **new);    // function to append a new execution instance to running queue
void displayQ(struct node *start);                          // function to display queue
void displayL();
void prSchedulling(); // executes non-preemptive priority schedulling algorithm
void display(int n);  // function to calculate process metrics and print them
void chart();

struct node *queueAT = NULL; // priority queue based on arrival time
struct node *readyQ = NULL;  // queue for processes that are ready to be executed (priority queue)
struct exec *runQ = NULL;    // queue for processes that are being executed (All of the nodes are stored)

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 6");
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);
    printf("\nEnter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        struct node *new = (struct node *)malloc(sizeof(struct node));
        printf("\nEnter process id: ");
        scanf("%d%*c", &new->pid);
        printf("Priority: ");
        scanf("%d%*c", &new->priority);
        printf("Arrival time and Burst time: ");
        scanf("%d %d%*c", &new->at, &new->bt);
        new->next = NULL;
        enqueueAT(&queueAT, &new); // queing new process based on arrival time
    }
    //displayQ(queueAT);
    prSchedulling(); // executes non-preemptive priority schedulling algorithm
    chart();         // printing gantt chart
    display(n);      // function which calculates process metrics and prints them

    return 0;
}

// Function Definitons:

void enqueueAT(struct node **start, struct node **new) // enqueuing based on arrival time
{
    if (!*start) // queue empty
    {
        *start = *new;
    }
    else if ((*new)->at < (*start)->at || ((*new)->at == (*start)->at && (*new)->pid < (*start)->pid)) // enqueing at head
    {
        (*new)->next = *start;
        *start = *new;
    }
    else
    {
        struct node *t = *start;
        while (t->next && ((*new)->at >= t->next->at || ((*new)->at == t->next->at && (*new)->pid > t->next->pid)))
        {
            t = t->next;
        }
        (*new)->next = t->next;
        t->next = *new;
    }
}

void enqueueReadyQ(struct node **start, struct node **new) // enqueuing based on priority
{
    if (!*start) // queue empty
    {
        *start = *new;
    }
    else if ((*new)->priority < (*start)->priority || ((*new)->priority == (*start)->priority && (*new)->pid < (*start)->pid)) // enqueing at head
    {
        (*new)->next = *start;
        *start = *new;
    }
    else
    {
        struct node *t = *start;
        while (t->next && ((*new)->priority >= t->next->priority || ((*new)->priority == t->next->priority && (*new)->pid > t->next->pid)))
        {
            t = t->next;
        }
        (*new)->next = t->next;
        t->next = *new;
    }
}

void enqueuRunQ(struct exec **start, struct exec **new) // function to append a new execution instance to running queue
{
    if (*start == NULL) // queue is empty
    {
        *start = *new; // enqueue at beginning
    }
    else
    {
        struct exec *t = *start;
        while (t->next) // going till last node in the queue
            t = t->next;
        (*new)->next = t->next;
        t->next = *new; // enqueue at end
    }
}

void prSchedulling()
{
    int curTime = 0;

    struct node *proc = NULL; // pointer to process to be executed

    while (queueAT != NULL || readyQ != NULL)
    {

        if (queueAT != NULL && curTime < queueAT->at)
            curTime = queueAT->at; // setting current time to latest arrived process

        // enqueing processes that have arrived uptil curTime into readyQ based on priority
        while (queueAT != NULL && queueAT->at <= curTime)
        {
            struct node *p = queueAT; // dequeing from AT queue
            queueAT = queueAT->next;
            p->next = NULL;
            enqueueReadyQ(&readyQ, &p); // enqueing into ready queue
        }

        proc = readyQ; // dequeing process from ready queue
        readyQ = readyQ->next;

        struct exec *i = (struct exec *)malloc(sizeof(struct exec)); // new execution instance node
        i->pid = proc->pid;
        i->priority = proc->priority;
        i->at = proc->at;
        i->start = curTime;
        curTime += proc->bt; // executing the whole process without preemption
        i->end = curTime;
        i->next = NULL;
        enqueuRunQ(&runQ, &i); // enquing the execution instance into ready queue
        free(proc);            // freeing the process that has been executed
    }
}

void chart() // printing gantt chart
{
    printf("\nGantt Chart:\n");
    struct exec *t = runQ;
    int time = runQ->at; // setting time to the arrival time of first process to be executed
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
    time = runQ->at;
    t = runQ;
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

void display(int n) // function which calculates process metrics and prints them
{
    printf("\nDisplaying metrics:");
    printf("\nProcess Priority BT AT CT WT  TAT RT\n");
    int total_ct = 0, total_wt = 0, total_tat = 0, total_rt = 0;
    while (runQ != NULL)
    {
        // displayL();
        int pid = runQ->pid;                  // process id
        int priority = runQ->priority;        // process priority
        int at = runQ->at;                    // arrival time
        int rt = runQ->start - at;            // response time
        int wt = runQ->start - at;            // waiting time
        int ct = runQ->end;                   // completion time
        int bt = (runQ->end) - (runQ->start); // burst time
        struct exec *p = runQ;
        runQ = runQ->next;
        free(p); // freeing execution instance
        // displayL();
        struct exec *t = runQ;
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
        printf("p%d        %2d    %2d %2d %2d %2d  %2d %2d\n", pid, priority, bt, at, ct, wt, tat, rt);
    }
    printf("\n\nAvg CT = %.2f", total_ct / (float)n);
    printf("\nAvg WT = %.2f", total_wt / (float)n);
    printf("\nAvg TAT = %.2f", total_tat / (float)n);
    printf("\nAvg RT = %.2f\n", total_rt / (float)n);
}

void displayQ(struct node *start) // function to display queue
{
    struct node *t = start;
    while (t)
    {
        printf("|p%d|%d|%d|%d|-> ", t->pid, t->priority, t->at, t->bt);
        t = t->next;
    }
    printf("\n");
}

void displayL()
{
    struct exec *t = runQ;
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

/*
20BCS041 Mohammed Abbas Ansari
OS Program 6
Enter the number of processes: 7

Enter the details of the processes.

Enter process id: 1
Priority: 2
Arrival time and Burst time: 0 3

Enter process id: 3
Priority: 3 
Arrival time and Burst time: 1 4

Enter process id: 2
Priority: 6
Arrival time and Burst time: 2 5

Enter process id: 4
Priority: 5
Arrival time and Burst time: 4 2

Enter process id: 5
Priority: 7
Arrival time and Burst time: 6 9

Enter process id: 6
Priority: 4
Arrival time and Burst time: 5 4

Enter process id: 7
Priority: 10
Arrival time and Burst time: 7 10

Gantt Chart:
|  p1  |   p3   |   p6   | p4 |    p2    |        p5        |         p7         |
0      3        7        11   13         18                 27                   37

Displaying metrics:
Process Priority BT AT CT WT  TAT RT
p1         2     3  0  3  0   3  0
p3         3     4  1  7  2   6  2
p6         4     4  5 11  2   6  2
p4         5     2  4 13  7   9  7
p2         6     5  2 18 11  16 11
p5         7     9  6 27 12  21 12
p7        10    10  7 37 20  30 20


Avg CT = 16.57
Avg WT = 7.71
Avg TAT = 13.00
Avg RT = 7.71
*/