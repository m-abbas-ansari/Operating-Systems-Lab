// OS Programming Lab 5: Round Robin Schedulling
// 20BCS041
// Mohammed Abbas Ansari
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures:

// Used to build priority queues based on arrival time.
struct node // node for process block
{
    int pid; // process id
    int at;  // arrival time
    int bt;  // burst time
    struct node *next;
};

// Used to build Running queue to print Gantt Chart and calculate process metrics.
struct exec // node for execution instance of process by the CPU
{
    int pid;   // process id
    int at;    // arrival time of process
    int start; // time of start of execution instance
    int end;   // time of end of execution instance
    struct exec *next;
};

// Functions:
void enqueueAT(struct node **start, struct node **new);     // enqueuing function based on arrival time
void enqueueReadyQ(struct node **start, struct node **new); // function to enqueueu processes into ready queue
void enqueuRunQ(struct exec **start, struct exec **new);    // function to append a new execution instance to running queue
void displayQ(struct node *start);                          // function to display queue
void displayL();
void roundRobin(int t); // executes round robin schedulling algorithm
void display(int n);    // function to calcualte process metrics and print them
void chart();           // function to print gantt chart

struct node *queueAT = NULL; // priority queue based on arrival time
struct node *readyQ = NULL;  // queue for processes that are ready to be executed
struct exec *runQ = NULL;    // queue for processes that are being executed (All of the nodes are stored)

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 5");
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);
    printf("Enter the time quantum: ");
    int t;
    scanf("%d%*c", &t);
    printf("\nEnter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        struct node *new = (struct node *)malloc(sizeof(struct node));
        printf("\nEnter process id: ");
        scanf("%d%*c", &new->pid);
        printf("Arrival time and Burst time: ");
        scanf("%d %d%*c", &new->at, &new->bt);
        new->next = NULL;
        enqueueAT(&queueAT, &new); // queing new process based on arrival time
    }
    //displayQ(queueAT);
    roundRobin(t); // executes round robin schedulling algorithm
    chart();       // printing gantt chart
    display(n);    // function which calculates process metrics and prints them

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

void enqueueReadyQ(struct node **start, struct node **new) // function to enqueueu processes into ready queue
{
    if (*start == NULL) // queue is empty
    {
        *start = *new; // enqueue at beginning
    }
    else
    {
        struct node *t = *start;
        while (t->next) // going till last node in the queue
            t = t->next;
        (*new)->next = t->next;
        t->next = *new; // enqueue at end
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

void roundRobin(int t) // t is the time quantum
{
    //printf("\nExecuting Round Robin function");
    int curTime = 0;
    struct node *activeP = NULL; // pointer to an active process

    while (queueAT != NULL || readyQ != NULL)
    {

        if (queueAT != NULL && curTime < queueAT->at)
            curTime = queueAT->at; // setting current time to latest arrived process

        // enqueing processes that have arrived uptil curTime into readyQ
        while (queueAT != NULL && queueAT->at <= curTime)
        {
            struct node *p = queueAT; // dequeing from AT queue
            queueAT = queueAT->next;
            p->next = NULL;
            enqueueReadyQ(&readyQ, &p); // enqueing into ready queue
        }

        if (activeP != NULL)
        {
            // an active process exits which must be enqueued back into ready queue (Context Switching)
            //printf("\nputting p%d back into ready queue", activeP->pid);
            enqueueReadyQ(&readyQ, &activeP);
            activeP = NULL; // no more active process
        }
        else
        {
            //printf("\nno process alive");
        }
        // printf("\ncurTime: %d\nreadyQ: ", curTime);
        // displayQ(readyQ);

        activeP = readyQ; // dequeing from ready queue to form an active process
        //printf("\nDequed p%d with bt %d", activeP->pid, activeP->bt);
        int bt = activeP->bt; // burst time of active process
        readyQ = readyQ->next;

        struct exec *i = (struct exec *)malloc(sizeof(struct exec)); // new execution instance node
        i->pid = activeP->pid;
        i->at = activeP->at;
        i->next = NULL;
        i->start = curTime; // setting start time of execution as curTime

        curTime = bt < t ? curTime + bt : curTime + t; // setting curTime depending on burst time and time quantum
        i->end = curTime;                              // setting end time of execution as curTime
        enqueuRunQ(&runQ, &i);                         // enqueing the execution instance
        //printf("\ncurTime: %d", curTime);

        if (bt - t > 0)
        {
            // process has not been completely executed, hence decrementing its burst time
            //printf("\np%d executed for %d s with remaining bt %d", activeP->pid, t, bt - t);
            activeP->bt = bt - t;
            // if (activeP != NULL)
            //     printf("\nprocess is still alive");
        }
        else
        {
            // process has been completely executed, hence freeing it
            //printf("\np%d completely executed", activeP->pid);
            free(activeP);
            activeP = NULL; // no more active process
        }
        //chart();
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
    printf("\nProcess BT AT CT WT  TAT RT\n");
    int total_ct = 0, total_wt = 0, total_tat = 0, total_rt = 0;
    while (runQ != NULL)
    {
        // displayL();
        int pid = runQ->pid;                  // process id
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
        printf("p%d      %2d %2d %2d %2d  %2d %2d\n", pid, bt, at, ct, wt, tat, rt);
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
        printf("|p%d|%d|%d|-> ", t->pid, t->at, t->bt);
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
Output:

20BCS041 Mohammed Abbas Ansari
OS Program 5
Enter the number of processes: 6
Enter the time quantum: 4

Enter the details of the processes.

Enter process id: 1
Arrival time and Burst time: 0 5

Enter process id: 2
Arrival time and Burst time: 1 6

Enter process id: 3
Arrival time and Burst time: 2 3

Enter process id: 4
Arrival time and Burst time: 3 1

Enter process id: 5
Arrival time and Burst time: 4 5

Enter process id: 6
Arrival time and Burst time: 6 4

Gantt Chart:
|   p1   |   p2   |  p3  |p4|   p5   |p1|   p6   | p2 |p5|
0        4        8      11 12       16 17       21   23 24

Displaying metrics:
Process BT AT CT WT  TAT RT
p1       5  0 17 12  17  0
p2       6  1 23 16  22  3
p3       3  2 11  6   9  6
p4       1  3 12  8   9  8
p5       5  4 24 15  20  8
p6       4  6 21 11  15 11


Avg CT = 18.00
Avg WT = 11.33
Avg TAT = 15.33
Avg RT = 6.00
*/