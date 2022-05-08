/*
OS Programming Lab 8: Highest Response Ratio Next (Non-Preemptive) Schedulling
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>
// Structures:

// Used to build array of processes
struct proc // node for process block
{
    int pid;   // process id
    int at;    // arrival time
    int bt;    // burst time
    int wt;    // waiting time
    int ct;    // completion time
    int start; // start time of execution
};

void insertProc(struct proc **arr, struct proc new, int *front, int *rear); // function that inserts into array to ensure it is sorted as per arrival time.
void printArray(struct proc *arr, int n);                                   // function to display array of processes
void printTable(struct proc *arr, int n);                                   // function to compute and display process metrics
void ganttChart(struct proc *arr, int n);                                   // function to display gantt chart

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 8");
    printf("\nEnter the number of processes: ");
    int n;
    scanf("%d%*c", &n);

    struct proc *arrayOfProcs = (struct proc *)malloc(n * sizeof(struct proc)); // Array of struct procesess
    int front = -1;                                                             // front pointer pointing to first process
    int rear = -1;                                                              // rear pointer pointing to empty position in array
    int cur_time = 0;                                                           // variable keeping track of the current time. It should begin from earliest arrived process.

    printf("Enter the details of the processes.\n");
    for (int i = 0; i < n; i++)
    {
        struct proc temp;
        printf("\nProcess id: ");
        scanf("%d%*c", &temp.pid);
        printf("Arrival Time: ");
        scanf("%d%*c", &temp.at);
        printf("Burst Time: ");
        scanf("%d%*c", &temp.bt);
        insertProc(&arrayOfProcs, temp, &front, &rear);
    }
    printArray(arrayOfProcs, n);
    int f = front;
    int r = rear;
    cur_time = arrayOfProcs[f].at;
    for (int i = 0; i < n; i++)
    {
        if(cur_time < arrayOfProcs[f].at)
            cur_time = arrayOfProcs[f].at;
        int j = f;
        int hrr_index = f; // index of process with highest response ratio
        arrayOfProcs[f].wt = cur_time - arrayOfProcs[f].at;
        float max_rr = (float)(arrayOfProcs[f].wt + arrayOfProcs[f].bt) / (float)arrayOfProcs[f].bt; // highest response ratio of first process
        while (j < n && arrayOfProcs[j].at <= cur_time) // run loop for arrived processes
        {
            arrayOfProcs[j].wt = cur_time - arrayOfProcs[j].at;
            float rr = (float)(arrayOfProcs[j].wt + arrayOfProcs[j].bt) / (float)arrayOfProcs[j].bt;
            if (rr > max_rr)
            {
                max_rr = rr;
                hrr_index = j;
            }
            j++;
        }
        struct proc temp = arrayOfProcs[f];
        arrayOfProcs[f] = arrayOfProcs[hrr_index];
        arrayOfProcs[hrr_index] = temp;
        arrayOfProcs[f].start = cur_time;
        arrayOfProcs[f].wt = cur_time - arrayOfProcs[f].at;
        cur_time += arrayOfProcs[f].bt;
        arrayOfProcs[f].ct = cur_time;
        f++;
        printArray(arrayOfProcs, n);
    }
    printArray(arrayOfProcs, n);
    printTable(arrayOfProcs, n);
    ganttChart(arrayOfProcs, n);
    // for (int i = 0; i < n; i++)
    // {
    //     free(&arrayOfProcs[i]);
    // }
    return 0;
}

void printArray(struct proc *arr, int n)
{
    printf("\n");
    for (int i = 0; i < n; i++)
    {
        printf("|id: %d,", arr[i].pid);
        printf("at: %d,", arr[i].at);
        printf("bt: %d|", arr[i].bt);
    }
    printf("\n");
}
void insertProc(struct proc **arr, struct proc new, int *front, int *rear) // function that inserts new process into array to ensure it is sorted as per arrival time.
{
    if (*front == -1)
    { // base case when array is empty
        (*arr)[0] = new;
        (*front)++;
        (*rear) = 1;
        return;
    }
    int f = 0;
    int r = *rear;

    for (int i = 0; i < r; i++)
    {
        struct proc temp = new; // temp variable to store the new process
        if (temp.at < (*arr)[i].at)
        {
            new = (*arr)[i];  // storing the current process in 'new' which will be inserted elsewhere according to arrival time
            (*arr)[i] = temp; // inserting temp at ith index
        }
        else if (temp.at == (*arr)[i].at && temp.pid < (*arr)[i].pid)
        {
            // sort by pid if arrival time is same
            new = (*arr)[i];
            (*arr)[i] = temp;
        }
    }
    (*arr)[r] = new; // inserting new process at rear
    (*rear)++;
}

void printTable(struct proc *arr, int n)
{
    int total_wt = 0;
    int total_ct = 0;
    int total_tat = 0;
    int total_rt = 0;
    printf("\n\n+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+\n");
    printf("| Process\t| Arrival Time\t| Burst Time\t| Completion Time | Waiting Time | Turnaround Time | Response Time |\n");
    printf("+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+\n");
    for (int i = 0; i < n; i++)
    {
        int tat = arr[i].ct - arr[i].at;
        int rt = tat - arr[i].bt;
        printf("| %d\t\t| %d\t\t| %d\t\t| %d\t\t  | %d\t\t | %d\t\t   | %d\t\t   |\n", arr[i].pid, arr[i].at, arr[i].bt, arr[i].ct, arr[i].wt, tat, rt);
        total_wt += arr[i].wt;
        total_ct += arr[i].ct;
        total_tat += tat;
        total_rt += rt;
    }
    printf("+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+\n");
    printf("| Total\t\t|\t\t|\t\t| %d\t\t  | %d\t\t | %d\t\t   | %d\t\t   |\n", total_ct, total_wt, total_tat, total_rt);
    printf("+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+\n");
    printf("\n Average Waiting Time: %f\n", (float)total_wt / (float)n);
    printf(" Average Turnaround Time: %f\n", (float)total_tat / (float)n);
    printf(" Average Response Time: %f\n", (float)total_rt / (float)n);
    printf(" Average Completion Time: %f\n", (float)total_ct / (float)n);
    printf("\n");
}

void ganttChart(struct proc *arr, int n)
{
    printf("\n\nGantt Chart\n");
    int cur_time = arr[0].at;
    int f = 0;
    int r = n;
    while (f < r)
    {
        if (cur_time < arr[f].start)
        {
            for (int i = 0; i < (arr[f].start - cur_time); i++)
                printf(" ");
            printf("|");
            cur_time = arr[f].start;
        }
        int bt = arr[f].bt;

        for (int i = 0; i < bt - 1; i++)
            printf(" ");
        printf("p%d", arr[f].pid);
        for (int i = 0; i < bt - 1; i++)
            printf(" ");
        printf("|");
        cur_time += bt;
        f++;
    }
    cur_time = arr[0].at;
    f = 0;
    r = n;
    printf("\n%d ", cur_time);
    while (f < r)
    {
        if (cur_time < arr[f].start)
        {
            for (int i = 0; i < (arr[f].start - cur_time); i++)
                printf(" ");
            printf("|");
            if (arr[f].start < 9)
                printf("%d ", arr[f].start);
            else
                printf("%d", arr[f].start);
            cur_time = arr[f].start;
        }
        int bt = arr[f].bt;
        for (int i = 0; i < 2 * (bt)-1; i++)
            printf(" ");
        if (arr[f].ct < 9)
            printf("%d ", arr[f].ct);
        else
            printf("%d", arr[f].ct);
        cur_time += bt;
        f++;
    }
    printf("\n");
}

/*
Output:
➜  OS ./hrrn          

20BCS041 Mohammed Abbas Ansari
OS Program 8
Enter the number of processes: 5

Enter the details of the processes.

Process id: 1 
Arrival Time: 1 
Burst Time: 3 

Process id: 2
Arrival Time: 3
Burst Time: 6 

Process id: 3
Arrival Time: 5
Burst Time: 8

Process id: 4
Arrival Time: 7
Burst Time: 4

Process id: 5
Arrival Time: 8
Burst Time: 5

+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+
| Process       | Arrival Time  | Burst Time    | Completion Time | Waiting Time | Turnaround Time | Response Time |
+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+
| 1             | 1             | 3             | 4               | 0            | 3               | 0             |
| 2             | 3             | 6             | 10              | 1            | 7               | 1             |
| 4             | 7             | 4             | 14              | 3            | 7               | 3             |
| 5             | 8             | 5             | 19              | 6            | 11              | 6             |
| 3             | 5             | 8             | 27              | 14           | 22              | 14            |
+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+
| Total         |               |               | 74              | 24           | 50              | 24            |
+---------------+---------------+---------------+-----------------+--------------+-----------------+---------------+

 Average Waiting Time: 4.800000
 Average Turnaround Time: 10.000000
 Average Response Time: 4.800000
 Average Completion Time: 14.800000



Gantt Chart
  p1  |     p2     |   p4   |    p5    |       p3       |
1      4            10       14         19               27

*/