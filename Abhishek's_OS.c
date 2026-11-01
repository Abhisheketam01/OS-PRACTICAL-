#include <stdio.h>

int n, m;
int alloc[10][10], max[10][10], need[10][10], avail[10];

/* Safety algorithm: returns 1 if safe, 0 if not. Fills seq[] with safe sequence */
int isSafe(int seq[]) {
    int work[10], finish[10] = {0};
    int i, j, count = 0;

    for (j = 0; j < m; j++)
        work[j] = avail[j];

    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int ok = 1;
                for (j = 0; j < m; j++)
                    if (need[i][j] > work[j]) { ok = 0; break; }
                if (ok) {
                    for (j = 0; j < m; j++)
                        work[j] += alloc[i][j];
                    finish[i] = 1;
                    seq[count++] = i;
                    found = 1;
                }
            }
        }
        if (!found) return 0;   /* no process could run -> unsafe */
    }
    return 1;
}

void printSeq(int seq[]) {
    int i;
    printf("Safe sequence: ");
    for (i = 0; i < n; i++) {
        printf("P%d", seq[i]);
        if (i < n - 1) printf(" -> ");
    }
    printf("\n");
}

int main() {
    int i, j, seq[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);

    printf("Enter Allocation matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Max matrix (%d x %d):\n", n, m);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);

    printf("Enter Available vector (%d values):\n", m);
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);

    /* Need = Max - Allocation */
    printf("\nNeed matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    if (isSafe(seq)) {
        printf("\nSystem is in a SAFE state.\n");
        printSeq(seq);
    } else {
        printf("\nSystem is NOT in a safe state (deadlock possible).\n");
        return 0;
    }

    /* Resource request */
    int p, req[10], choice;
    printf("\nMake a resource request? (1 = yes, 0 = no): ");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Enter process number (0 to %d): ", n - 1);
        scanf("%d", &p);
        printf("Enter request vector (%d values): ", m);
        for (j = 0; j < m; j++)
            scanf("%d", &req[j]);

        for (j = 0; j < m; j++) {
            if (req[j] > need[p][j]) {
                printf("Error: process has exceeded its maximum claim.\n");
                return 0;
            }
        }
        for (j = 0; j < m; j++) {
            if (req[j] > avail[j]) {
                printf("Resources not available. P%d must wait.\n", p);
                return 0;
            }
        }
        /* Pretend to allocate */
        for (j = 0; j < m; j++) {
            avail[j]    -= req[j];
            alloc[p][j] += req[j];
            need[p][j]  -= req[j];
        }
        if (isSafe(seq)) {
            printf("Request can be GRANTED.\n");
            printSeq(seq);
        } else {
            printf("Request cannot be granted (unsafe state). P%d must wait.\n", p);
        }
    }
    return 0;
}




ouput - 
  compile - gcc banker.c -o banker
  run - ./banker
https://res.cloudinary.com/dtwrjtz8k/image/upload/v1793565143/Screenshot_2026-11-02_015305_nirkfn.png

  

  
