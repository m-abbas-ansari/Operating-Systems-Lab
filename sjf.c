// OS Programming Lab 3: SJF Schedulling
// 20BCS041 Mohammed Abbas Ansari

#include <stdio.h>
#include <stdlib.h>

struct node
{
    char process[10];
    int bt;  // burst time
    int at;  // arrival time
    int ct;  // completion time
    int wt;  // waiting time = response time for fcfs
    int tat; // turn around time
    struct node *next;
};

struct node *pq_arr = NULL; // priority queue where priority as per arrival time
struct node *pq_sjf = NULL; // SJF Execution queue (priority as per burst time)

void freeq(struct node **start); // function for freeing up nodes of the priority queue

void displayQ(struct node *start) // function to display queue
{
    struct node *t = start;
    printf("\nQueue: ");
    while (t)
    {
        printf("|%s|%d|%d|-> ", t->process, t->bt, t->at);
        t = t->next;
    }
    printf("\n");
}
void enqueue(struct node **start) // enqueueing based on arrival time
{
    struct node *new = (struct node *)malloc(sizeof(struct node));
    printf("\nEnter process name: ");
    scanf("%s%*c", new->process);
    printf("Arrival time: ");
    scanf("%d%*c", &new->at);
    printf("Burst time: ");
    scanf("%d%*c", &new->bt);
    new->next = NULL;
    if (!*start)
    {
        // Queue is empty
        *start = new;
    }
    else if (new->at < (*start)->at)
    {
        // Insert at beginning
        new->next = *start;
        *start = new;
    }
    else
    {
        // Queue is not empty
        struct node *t = *start;
        while (t->next && new->at >= t->next->at)
        {
            t = t->next;
        }
        new->next = t->next;
        t->next = new;
    }
}

void enqueue_sjf(struct node **start, struct node **new) // enqueuing based on burst time
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

void sjf()
{
    struct node *rq = pq_arr; // pointer to priority queue based on arrival time (ready queue)
    struct node *eq = pq_sjf; // pointer to queue based on which execution will happen according to SJF Schedulling
    int curTime = rq->at;     // maintaing the time which begins from the process that arrived earliest
    struct node *q = NULL;    // pointer to priority queue based on burst time for processes with arrival time less than equal to curTime

    while (rq != NULL) // loop untill ready queue is empty
    {
        printf("\nCurrent time: %d", curTime);

        int t = 0; // mainting the count it would take to execute all the processes we have dequed from ready queue
        while (rq != NULL && rq->at <= curTime)
        {
            struct node *p = rq; // dequeing from ready queue
            rq = rq->next;
            p->next = NULL;
            t += p->bt;
            enqueue_sjf(&q, &p);
        }
        printf("\nTemp Q at T = %d: ", curTime);
        displayQ(q);

        int exTime = q->at; // execution time of sjf queue made uptil now
        while (q != NULL && q->at >= exTime)
        {
            if (rq == NULL) // base case when ready queue has been made empty.
            {
                if (pq_sjf == NULL)
                    pq_sjf = q;
                else
                    while (eq->next != NULL)
                        eq = eq->next;
                eq->next = q;
                break;
            }

            struct node *n = q; // dequeing from temp priority queue where bt is the priority
            q = q->next;
            n->next = NULL;
            if (pq_sjf == NULL) // sjf queue is empty
            {
                printf("\nempty pq_sjf\n");
                pq_sjf = n;
                eq = pq_sjf;
            }
            else // sjf queue is not empty
            {
                while (eq->next != NULL) // going till last node of sjf queue to append the node n from temp queue
                    eq = eq->next;
                eq->next = n;
            }
            exTime += n->bt;
        }
        printf("\nExceution queue uptil now: ");
        displayQ(pq_sjf);
        // q = NULL; // deleting the current pq so as to create a new one for the later arrival times
        if (rq != NULL)
            if (curTime + t < rq->at)
                curTime = rq->at; // resetting time to the latest process' arrival time
            else
                curTime += t;
    }
}

void display(struct node *start, int n)
{
    struct node *t = start;
    float total_ct = 0, total_wt = 0, total_tat = 0, total_rt = 0;
    int time = 0;
    printf("\nOutput:\n");
    printf("\nProcess BT AT CT WT  TAT RT\n");
    while (t)
    {
        if (time < t->at)
            time = t->at;
        time += t->bt;
        t->ct = time;
        t->tat = t->ct - t->at;
        t->wt = t->tat - t->bt;
        printf("%s      %2d %2d %2d %2d  %2d %2d\n", t->process, t->bt, t->at, t->ct, t->wt, t->tat, t->wt);
        total_ct += t->ct;
        total_wt += t->wt;
        total_tat += t->tat;
        total_rt += t->wt;
        t = t->next;
    }
    printf("\nAvg CT = %.2f", total_ct / (float)n);
    printf("\nAvg WT = %.2f", total_wt / (float)n);
    printf("\nAvg TAT = %.2f", total_tat / (float)n);
    printf("\nAvg RT = %.2f", total_rt / (float)n);
}

void ganttChart(struct node *start)
{
    struct node *t = start;
    int time = 0;
    printf("\nGantt Chart:\n");
    while (t)
    {
        if (time == 0 | time < t->at)
        {
            for (int i = 0; i < (t->at - time); i++)
            {
                printf(" ");
            }
            printf("|");
            time = t->at;
        }
        for (int i = 0; i < t->bt - 1; i++)
            printf(" ");
        printf("%s", t->process);
        for (int i = 0; i < t->bt - 1; i++)
            printf(" ");
        printf("|");
        time += t->bt;
        t = t->next;
    }
    t = start;
    time = 0;
    printf("\n");
    while (t)
    {
        if (time == 0 | time < t->at)
        {
            for (int i = 0; i < (t->at - time) - 1; i++)
            {
                printf(" ");
            }
            if (t->ct < 9)
                printf("%d ", t->at);
            else
                printf("%d", t->at);
            time = t->at;
        }
        for (int i = 0; i < 2 * (t->bt) - 1; i++)
            printf(" ");
        if (t->ct < 9)
            printf("%d ", t->ct);
        else
            printf("%d", t->ct);
        time += t->bt;
        t = t->next;
    }
    printf("\n");
}

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari");
    printf("\nOS Program 3");
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);
    printf("Enter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        enqueue(&pq_arr);
        // queuing the processes in the ready queue depending on the arrival time
        // the process that arrives earlier will be at front of the queue to be processes
    }
    printf("\nReady Queue: ");
    displayQ(pq_arr);
    sjf(); // queueing the processes from the ready queue depending on the burst time (pq_arr => pq_sjf)
    printf("\nFinal Execution Queue: ");
    displayQ(pq_sjf);
    display(pq_sjf, n); // calculating and displaying the table and average metrics
    ganttChart(pq_sjf); // Gantt Chart
    freeq(&pq_sjf);     // freeing up memory
    return 0;
}

void freeq(struct node **start)
{
    struct node *t = *start;
    while (t)
    {
        struct node *del = t;
        t = t->next;
        free(del);
    }
}
/*
20BCS041 Mohammed Abbas Ansari
OS Program 3
Enter the number of processes: 5
Enter the details of the processes.

Enter process name: p1
Arrival time: 0
Burst time: 3

Enter process name: p2
Arrival time: 6
Burst time: 7

Enter process name: p3
Arrival time: 6
Burst time: 4

Enter process name: p4
Arrival time: 6
Burst time: 2

Enter process name: p5
Arrival time: 5
Burst time: 2

Output:

Process BT AT CT WT  TAT RT
p1       3  0  3  0   3  0
p5       2  5  7  0   2  0
p4       2  6  9  1   3  1
p3       4  6 13  3   7  3
p2       7  6 20  7  14  7

Avg CT = 10.40
Avg WT = 2.20
Avg TAT = 5.80
Avg RT = 2.20
Gantt Chart:
|  p1  |  | p5 | p4 |   p3   |      p2      |
0      3  5    7    9       13             20
*/