/*
OS Programming Lab 12: Worst fit memory management algorithm.
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 12");
    printf("\nEnter Number of blocks: ");
    int numBlocks;
    scanf("%d%*c", &numBlocks);
    int *blocks = (int *)malloc(numBlocks * sizeof(int));     // memory block array
    int *flags = (int *)malloc(numBlocks * sizeof(int));      // flags for whether the block is filled with a process
    int *allocation = (int *)malloc(numBlocks * sizeof(int)); // index of process allocated to the block

    printf("Enter memory blocks (seperated by a space):");
    for (int i = 0; i < numBlocks; i++)
    {
        scanf("%d%*c", &blocks[i]);
        flags[i] = 0;
        allocation[i] = -1;
    }
    scanf("%*c");
    printf("\nEnter number of processes: ");
    int numProcs;
    scanf("%d%*c", &numProcs);
    int *procs = (int *)malloc(numProcs * sizeof(int));   // process array
    int *flProcs = (int *)malloc(numProcs * sizeof(int)); // flag for whether the process was allocated a memory block

    printf("Enter memory of processes (seperated by a space):");
    for (int i = 0; i < numProcs; i++)
    {
        scanf("%d%*c", &procs[i]);
        flProcs[i] = 0;
    }
    int numAllocated = 0;
    for (int i = 0; i < numProcs; i++)
    {
        int worstFitIdx = -1;
        for (int j = 0; j < numBlocks; j++)
        {
            if (flags[j] == 0 && blocks[j] >= procs[i])
            {
                // found a memory block that can fit the process
                if (worstFitIdx != -1)
                {
                    // checking whether it is the worst fit
                    if (blocks[j] > blocks[worstFitIdx])
                        worstFitIdx = j;
                }
                else
                    worstFitIdx = j;
            }
        }
        if (worstFitIdx != -1)
        {
            allocation[worstFitIdx] = i;
            flags[worstFitIdx] = 1;
            flProcs[i] = 1;
            numAllocated++;
        }
    }
    printf("\nBlock No \t Size of Block \t Process Allocated \t IF");
    int totInternalFrag = 0;
    int totExternalFrag = 0;
    for (int i = 0; i < numBlocks; i++)
    {
        if (allocation[i] != -1)
        {
            int internalFrag = blocks[i] - procs[allocation[i]];
            printf("\n%d \t\t   %d \t\t   %d(P%d) \t\t %d", i, blocks[i], procs[allocation[i]], allocation[i], internalFrag);
            totInternalFrag += internalFrag;
        }
        else
        {
            printf("\n%d \t\t   %d \t\t   --- \t\t\t 0", i, blocks[i]);
            if (numAllocated < numProcs)
                totExternalFrag += blocks[i];
        }
    }
    int remProcMem = 0;
    for(int i = 0; i < numProcs; i++)
    {
        if(flProcs[i] == 0)
            remProcMem += procs[i];
    }
    if(remProcMem > totExternalFrag)
        totExternalFrag = 0;

    printf("\nTotal internal fragmentation: %d", totInternalFrag);
    printf("\nTotal External Fragmentation: %d\n", totExternalFrag);
    return 0;
}

/*
Output

➜  OS ./worstFit

20BCS041 Mohammed Abbas Ansari
OS Program 12
Enter Number of blocks: 5
Enter memory blocks (seperated by a space):500 400 300 200 100

Enter number of processes: 4
Enter memory of processes (seperated by a space): 90 200 200 300

Block No         Size of Block   Process Allocated       IF
0                  500             90(P0)                410
1                  400             200(P1)               200
2                  300             200(P2)               100
3                  200             ---                   0
4                  100             ---                   0
Total internal fragmentation: 710
Total External Fragmentation: 300
➜  OS ./worstFit

20BCS041 Mohammed Abbas Ansari
OS Program 12
Enter Number of blocks: 5
Enter memory blocks (seperated by a space): 200 100 500 600 480

Enter number of processes: 4
Enter memory of processes (seperated by a space):210 210 250 450

Block No         Size of Block   Process Allocated       IF
0                  200             ---                   0
1                  100             ---                   0
2                  500             210(P1)               290
3                  600             210(P0)               390
4                  480             250(P2)               230
Total internal fragmentation: 910
Total External Fragmentation: 0
*/