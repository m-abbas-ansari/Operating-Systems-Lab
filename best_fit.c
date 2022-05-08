/*
OS Programming Lab 11: Best fit memory management algorithm.
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 11");
    printf("\nEnter Number of blocks: ");
    int numBlocks;
    scanf("%d%*c", &numBlocks);
    int *blocks = (int *)malloc(numBlocks * sizeof(int));
    int *flags = (int *)malloc(numBlocks * sizeof(int));
    int *allocation = (int *)malloc(numBlocks * sizeof(int));

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
    int *procs = (int *)malloc(numProcs * sizeof(int));
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
        int bestFitIdx = -1;
        for (int j = 0; j < numBlocks; j++)
        {
            if (flags[j] == 0 && blocks[j] >= procs[i])
            {
                // found a memory block that can fit the process
                if (bestFitIdx != -1)
                {
                    // checking whether it is the best fit
                    if (blocks[j] < blocks[bestFitIdx])
                        bestFitIdx = j;
                }
                else
                    bestFitIdx = j;
            }
        }
        if (bestFitIdx != -1)
        {
            allocation[bestFitIdx] = i;
            flags[bestFitIdx] = 1;
            numAllocated++;
            flProcs[i] = 1;
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
    for (int i = 0; i < numProcs; i++)
    {
        if (flProcs[i] == 0)
            remProcMem += procs[i];
    }
    if (remProcMem > totExternalFrag)
        totExternalFrag = 0;
        
    printf("\nTotal internal fragmentation: %d", totInternalFrag);
    printf("\nTotal External Fragmentation: %d\n", totExternalFrag);
    return 0;
}

/*
Output
20BCS041 Mohammed Abbas Ansari
OS Program 11
Enter Number of blocks: 5
Enter memory blocks (seperated by a space):200 100 300 400 500

Enter number of processes: 4
Enter memory of processes (seperated by a space):90 390 110 450

Block No         Size of Block   Process Allocated       IF
0                  200             110(P2)               90
1                  100             90(P0)                10
2                  300             ---                   0
3                  400             390(P1)               10
4                  500             450(P3)               50
Total internal fragmentation: 160
Total External Fragmentation: 0

*/