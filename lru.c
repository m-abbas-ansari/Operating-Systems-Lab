/*
OS Programming Lab 14: Least Recently Used (LRU) page replacement algorithm
20BCS041
Mohammed Abbas Ansari
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("\n20BCS041 Mohammed Abbas Ansari\nOS Program 14");
    printf("\nEnter the number of pages in reference string: ");
    int n_pages;
    scanf("%d", &n_pages);
    printf("\nEnter the input reference string (seperated by commas): ");
    int *ref_string = (int *)malloc(n_pages * sizeof(int));
    for (int i = 0; i < n_pages; i++)
    {
        scanf("%d,", &ref_string[i]);
    }
    scanf("%*c");
    printf("\nEnter the number of frames: ");
    int n_frames;
    scanf("%d", &n_frames);
    int *frames = (int *)malloc(n_frames * sizeof(int));
    for (int i = 0; i < n_frames; i++)
    {
        frames[i] = -1;
    }
    // Least Recently Used Algorithm
    int num_faults = 0;
    for (int i = 0; i < n_pages; i++)
    {
        int flag = 0;
        for (int j = 0; j < n_frames; j++)
        {
            if (ref_string[i] == frames[j])
            {
                flag = 1; // page hit
                break;
            }
        }
        if (flag == 1)
        {
            printf("\nPage %d hit", ref_string[i]);
        }
        else
        {
            printf("\nPage %d fault.", ref_string[i]);
            num_faults++;
            int empty_flag = 0;
            for (int j = 0; j < n_frames; j++)
            {
                if (frames[j] == -1) // insert into empty frame
                {
                    frames[j] = ref_string[i];
                    empty_flag = 1;
                    break;
                }
            }
            if (empty_flag == 0) // no empty frames
            {
                // find the frame with recently used page
                int lru_index = -1;      // index of the frame with least recently used page
                int k_min = __INT_MAX__; // index in reference string of the least recently used page
                for (int j = 0; j < n_frames; j++)
                {
                    for (int k = i - 1; k >= 0; k--)
                    {
                        if (ref_string[k] == frames[j])
                        {
                            if (k < k_min)
                            {
                                k_min = k;
                                lru_index = j;
                            }
                            break;
                        }
                    }
                }
                int temp = frames[lru_index];
                frames[lru_index] = ref_string[i];
                printf("\nLRU Page %d replaced by page %d", temp, ref_string[i]);
            }
        }
        printf("\nFrames: |");
        for (int j = 0; j < n_frames; j++)
        {
            if (frames[j] == -1)
                printf(" - |");
            else
                printf(" %d |", frames[j]);
        }
        printf("\n");
    }
    printf("\nNumber of page faults: %d", num_faults);
    printf("\nAverage page faults: %f\n", (float)num_faults / n_pages);
    return 0;
}
/*
Output:
➜  OS ./lru

20BCS041 Mohammed Abbas Ansari
OS Program 14
Enter the number of pages in reference string: 20

Enter the input reference string (seperated by commas): 7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1,

Enter the number of frames: 4

Page 7 fault.
Frames: | 7 | - | - | - |

Page 0 fault.
Frames: | 7 | 0 | - | - |

Page 1 fault.
Frames: | 7 | 0 | 1 | - |

Page 2 fault.
Frames: | 7 | 0 | 1 | 2 |

Page 0 hit
Frames: | 7 | 0 | 1 | 2 |

Page 3 fault.
LRU Page 7 replaced by page 3
Frames: | 3 | 0 | 1 | 2 |

Page 0 hit
Frames: | 3 | 0 | 1 | 2 |

Page 4 fault.
LRU Page 1 replaced by page 4
Frames: | 3 | 0 | 4 | 2 |

Page 2 hit
Frames: | 3 | 0 | 4 | 2 |

Page 3 hit
Frames: | 3 | 0 | 4 | 2 |

Page 0 hit
Frames: | 3 | 0 | 4 | 2 |

Page 3 hit
Frames: | 3 | 0 | 4 | 2 |

Page 2 hit
Frames: | 3 | 0 | 4 | 2 |

Page 1 fault.
LRU Page 4 replaced by page 1
Frames: | 3 | 0 | 1 | 2 |

Page 2 hit
Frames: | 3 | 0 | 1 | 2 |

Page 0 hit
Frames: | 3 | 0 | 1 | 2 |

Page 1 hit
Frames: | 3 | 0 | 1 | 2 |

Page 7 fault.
LRU Page 3 replaced by page 7
Frames: | 7 | 0 | 1 | 2 |

Page 0 hit
Frames: | 7 | 0 | 1 | 2 |

Page 1 hit
Frames: | 7 | 0 | 1 | 2 |

Number of page faults: 8
Average page faults: 0.400000
*/
