==============================================================
                 OPERATING SYSTEM PRACTICALS
                 (Practical 1 to Practical 8)
==============================================================

GENERAL NOTES
-------------
- Run all programs in an Ubuntu / Linux terminal.
- To create a file:      nano <filename>
- To save in nano:       Ctrl + O  ->  Enter  ->  Ctrl + X
- To compile a C file:   gcc <file>.c -o <name>
- Programs that use threads/semaphores need the -pthread flag.


##############################################################
PRACTICAL 1 : ADDRESS BOOK (SHELL SCRIPT)
##############################################################

FILE NAME
---------
address_book.sh

INSTRUCTIONS
------------
Step 1: Create the file
    nano address_book.sh

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Give execution permission
    chmod +x address_book.sh

Step 4: Run the program
    ./address_book.sh

CODE
----
#!/bin/bash

FILE="addressbook.txt"

while true
do
    echo ""
    echo "========== ADDRESS BOOK =========="
    echo "1. Create Address Book"
    echo "2. View Address Book"
    echo "3. Insert a Record"
    echo "4. Delete a Record"
    echo "5. Modify a Record"
    echo "6. Exit"
    echo "=================================="
    read -p "Enter your choice: " choice

    case $choice in

        1)
            > "$FILE"
            echo "Address book created successfully."
            ;;

        2)
            if [ -f "$FILE" ]; then
                echo "----- Address Book Records -----"
                cat "$FILE"
            else
                echo "Address book does not exist."
            fi
            ;;

        3)
            if [ ! -f "$FILE" ]; then
                touch "$FILE"
            fi
            read -p "Enter Name: " name
            read -p "Enter Phone Number: " phone
            read -p "Enter Address: " address
            echo "$name | $phone | $address" >> "$FILE"
            echo "Record inserted successfully."
            ;;

        4)
            if [ ! -f "$FILE" ]; then
                echo "Address book does not exist."
                continue
            fi
            read -p "Enter the name to delete: " name
            if grep -Fq "$name |" "$FILE"; then
                grep -Fv "$name |" "$FILE" > temp.txt
                mv temp.txt "$FILE"
                echo "Record deleted successfully."
            else
                echo "Record not found."
            fi
            ;;

        5)
            if [ ! -f "$FILE" ]; then
                echo "Address book does not exist."
                continue
            fi
            read -p "Enter the name to modify: " name
            if grep -Fq "$name |" "$FILE"; then
                grep -Fv "$name |" "$FILE" > temp.txt
                read -p "Enter New Name: " newname
                read -p "Enter New Phone Number: " newphone
                read -p "Enter New Address: " newaddress
                echo "$newname | $newphone | $newaddress" >> temp.txt
                mv temp.txt "$FILE"
                echo "Record modified successfully."
            else
                echo "Record not found."
            fi
            ;;

        6)
            echo "Exiting Address Book."
            exit 0
            ;;

        *)
            echo "Invalid choice. Please try again."
            ;;

    esac
done


##############################################################
PRACTICAL 2 : PROCESS MANAGEMENT (fork, wait, zombie, orphan)
##############################################################

--------------------------------------------------------------
PART A : PROCESS MANAGEMENT (fork + bubble sort)
--------------------------------------------------------------

FILE NAME
---------
process_management.c

INSTRUCTIONS
------------
Step 1: Create the C file
    nano process_management.c

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Compile the program
    gcc process_management.c -o process_management

Step 4: Run the program
    ./process_management

Step 5: To observe process states (in another terminal)
    ps -el | grep process_management

CODE
----
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void bubbleSort(int arr[], int n)
{
    int i, j, temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void display(int arr[], int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int arr[100], n, i;
    pid_t pid;

    printf("Enter number of integers: ");
    scanf("%d", &n);

    if (n <= 0 || n > 100)
    {
        printf("Invalid number of integers.\n");
        return 1;
    }

    printf("Enter %d integers:\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        printf("\nChild Process Created.\n");
        printf("Child PID: %d\n", getpid());
        bubbleSort(arr, n);
        printf("Child Process Sorted Array: ");
        display(arr, n);
        exit(0);
    }
    else
    {
        printf("\nParent Process.\n");
        printf("Parent PID: %d\n", getpid());
        bubbleSort(arr, n);
        printf("Parent Process Sorted Array: ");
        display(arr, n);
        wait(NULL);
        printf("\nParent process: Child process completed.\n");
    }

    return 0;
}

--------------------------------------------------------------
PART B : ZOMBIE PROCESS DEMONSTRATION
--------------------------------------------------------------

FILE NAME
---------
zombie.c

INSTRUCTIONS
------------
Step 1: Create the file
    nano zombie.c

Step 2: Paste the code and save it.

Step 3: Compile and run
    gcc zombie.c -o zombie
    ./zombie

Step 4: During the 30-second sleep, open another terminal and run:
    ps -el | grep zombie

Step 5: Look for state "Z" (Zombie).

CODE
----
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child process exiting. PID: %d\n", getpid());
        exit(0);
    }
    else if (pid > 0)
    {
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        printf("Parent sleeping for 30 seconds...\n");
        sleep(30);
    }

    return 0;
}

--------------------------------------------------------------
PART C : ORPHAN PROCESS DEMONSTRATION
--------------------------------------------------------------

FILE NAME
---------
orphan.c

INSTRUCTIONS
------------
Step 1: Create the file
    nano orphan.c

Step 2: Paste the code and save it.

Step 3: Compile and run
    gcc orphan.c -o orphan
    ./orphan

Result: The child continues executing after its parent terminates
and gets adopted by PID 1 (init/systemd) or another system reaper.

CODE
----
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        printf("Child process created.\n");
        printf("Child PID: %d\n", getpid());
        printf("Initial Parent PID: %d\n", getppid());

        sleep(5);

        printf("\nAfter parent termination:\n");
        printf("Child PID: %d\n", getpid());
        printf("New Parent PID: %d\n", getppid());
        printf("Child process completed.\n");
    }
    else if (pid > 0)
    {
        printf("Parent PID: %d\n", getpid());
        printf("Parent process terminating...\n");
        exit(0);
    }

    return 0;
}


##############################################################
PRACTICAL 3 : PRODUCER-CONSUMER PROBLEM
##############################################################

FILE NAME
---------
producer_consumer.c

INSTRUCTIONS
------------
Step 1: Create the C file
    nano producer_consumer.c

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Compile the program
    gcc producer_consumer.c -o producer_consumer -pthread

Step 4: Run the program
    ./producer_consumer

CODE
----
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE];
int in = 0, out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void *producer(void *arg)
{
    int item;
    for (int i = 1; i <= 10; i++)
    {
        item = i;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer produced: %d\n", item);
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg)
{
    int item;
    for (int i = 1; i <= 10; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        item = buffer[out];
        printf("Consumer consumed: %d\n", item);
        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        sleep(2);
    }
    return NULL;
}

int main()
{
    pthread_t producer_thread, consumer_thread;

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\nProducer-Consumer problem completed.\n");
    return 0;
}


##############################################################
PRACTICAL 4 : DINING PHILOSOPHERS PROBLEM
##############################################################

FILE NAME
---------
dining_philosophers.c

INSTRUCTIONS
------------
Step 1: Create the C file
    nano dining_philosophers.c

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Compile the program
    gcc dining_philosophers.c -o dining_philosophers -pthread

Step 4: Run the program
    ./dining_philosophers

CODE
----
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t room;
sem_t chopstick[N];

void *philosopher(void *num)
{
    int id = *(int *)num;

    printf("Philosopher %d is thinking.\n", id);
    sleep(1);

    sem_wait(&room);
    sem_wait(&chopstick[id]);
    sem_wait(&chopstick[(id + 1) % N]);

    printf("Philosopher %d is eating.\n", id);
    sleep(2);

    printf("Philosopher %d finished eating.\n", id);

    sem_post(&chopstick[(id + 1) % N]);
    sem_post(&chopstick[id]);
    sem_post(&room);

    printf("Philosopher %d is thinking again.\n", id);
    return NULL;
}

int main()
{
    pthread_t threads[N];
    int ids[N];

    sem_init(&room, 0, N - 1);

    for (int i = 0; i < N; i++)
    {
        sem_init(&chopstick[i], 0, 1);
    }

    for (int i = 0; i < N; i++)
    {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < N; i++)
    {
        sem_destroy(&chopstick[i]);
    }

    sem_destroy(&room);

    printf("\nAll philosophers have finished eating.\n");
    return 0;
}


##############################################################
PRACTICAL 5 : BANKER'S ALGORITHM
##############################################################

FILE NAME
---------
bankers_algorithm.c

INSTRUCTIONS
------------
Step 1: Create the C file
    nano bankers_algorithm.c

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Compile the program
    gcc bankers_algorithm.c -o bankers_algorithm

Step 4: Run the program
    ./bankers_algorithm

CODE
----
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


##############################################################
PRACTICAL 6 : INTER-PROCESS COMMUNICATION USING FIFO (NAMED PIPES)
##############################################################

FILE NAMES
----------
process1.c
process2.c

INSTRUCTIONS
------------
Step 1: Create the first process file
    nano process1.c
  Paste the first code and save (Ctrl + O -> Enter -> Ctrl + X).

Step 2: Create the second process file
    nano process2.c
  Paste the second code and save.

Step 3: Compile both programs
    gcc process1.c -o process1
    gcc process2.c -o process2

Step 4: Run the programs
  Open two Ubuntu terminals.

  Terminal 1:
    ./process1

  Terminal 2:
    ./process2

Step 5: Enter sentences in Terminal 1.
  Press Ctrl + D when finished to send the input for processing.
  The character, word and line counts will be shown in Terminal 1.

CODE
----

FILE 1: process1.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PIPE1 "/tmp/pipe1"
#define PIPE2 "/tmp/pipe2"

int main()
{
    int fd1, fd2;
    char buffer[1024];

    mkfifo(PIPE1, 0666);
    mkfifo(PIPE2, 0666);

    printf("Enter sentences (Press Ctrl+D to finish):\n");

    fd1 = open(PIPE1, O_WRONLY);

    while (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        write(fd1, buffer, strlen(buffer));
    }

    close(fd1);

    fd2 = open(PIPE2, O_RDONLY);

    printf("\nResult received from Process 2:\n");

    while (1)
    {
        int n = read(fd2, buffer, sizeof(buffer) - 1);
        if (n <= 0)
            break;

        buffer[n] = '\0';
        printf("%s", buffer);
    }

    close(fd2);

    unlink(PIPE1);
    unlink(PIPE2);

    return 0;
}

FILE 2: process2.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define PIPE1 "/tmp/pipe1"
#define PIPE2 "/tmp/pipe2"

int main()
{
    int fd1, fd2;
    char buffer[1024];
    int characters = 0, words = 0, lines = 0;
    int in_word = 0;
    int n;

    fd1 = open(PIPE1, O_RDONLY);

    while ((n = read(fd1, buffer, sizeof(buffer))) > 0)
    {
        for (int i = 0; i < n; i++)
        {
            characters++;

            if (buffer[i] == '\n')
                lines++;

            if (isspace((unsigned char)buffer[i]))
            {
                in_word = 0;
            }
            else if (in_word == 0)
            {
                words++;
                in_word = 1;
            }
        }
    }

    close(fd1);

    FILE *fp = fopen("result.txt", "w");
    if (fp == NULL)
    {
        perror("File error");
        return 1;
    }

    fprintf(fp, "Number of Characters: %d\n", characters);
    fprintf(fp, "Number of Words: %d\n", words);
    fprintf(fp, "Number of Lines: %d\n", lines);

    fclose(fp);

    fp = fopen("result.txt", "r");
    if (fp == NULL)
    {
        perror("File error");
        return 1;
    }

    fd2 = open(PIPE2, O_WRONLY);

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        write(fd2, buffer, n);
    }

    close(fd2);
    fclose(fp);

    return 0;
}


##############################################################
PRACTICAL 7 : FULL DUPLEX COMMUNICATION USING PIPES
##############################################################

FILE NAME
---------
full_duplex_pipe.c

INSTRUCTIONS
------------
Step 1: Create the C file
    nano full_duplex_pipe.c

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Compile the program
    gcc full_duplex_pipe.c -o full_duplex_pipe

Step 4: Run the program
    ./full_duplex_pipe

Step 5: Enter the pathname of an existing file when prompted.
  Example:
    Enter file pathname: input.txt

CODE
----
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;
    char filename[256];
    char buffer[1024];
    int n;

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("Pipe creation failed");
        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0)
    {
        // Parent Process
        close(pipe1[0]);
        close(pipe2[1]);

        printf("Enter file pathname: ");
        scanf("%255s", filename);

        // Send filename to child
        write(pipe1[1], filename, strlen(filename) + 1);
        close(pipe1[1]);

        printf("\nFile contents received from child:\n");

        // Read file contents from second pipe
        while ((n = read(pipe2[0], buffer, sizeof(buffer))) > 0)
        {
            write(STDOUT_FILENO, buffer, n);
        }

        close(pipe2[0]);
        wait(NULL);
    }
    else
    {
        // Child Process
        close(pipe1[1]);
        close(pipe2[0]);

        // Read filename from first pipe
        read(pipe1[0], filename, sizeof(filename));
        close(pipe1[0]);

        // Open requested file
        FILE *fp = fopen(filename, "r");

        if (fp == NULL)
        {
            char error[] = "Error: Unable to open file.\n";
            write(pipe2[1], error, strlen(error));
        }
        else
        {
            // Send file contents to parent
            while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0)
            {
                write(pipe2[1], buffer, n);
            }
            fclose(fp);
        }

        close(pipe2[1]);
        exit(0);
    }

    return 0;
}


##############################################################
PRACTICAL 8 : DISK SCHEDULING (SSTF, SCAN, C-LOOK)
##############################################################

FILE NAME
---------
disk_scheduling.c

INSTRUCTIONS
------------
Step 1: Create the C file
    nano disk_scheduling.c

Step 2: Paste the code and save it.
    Save: Ctrl + O -> Enter -> Ctrl + X

Step 3: Compile the program
    gcc disk_scheduling.c -o disk_scheduling

Step 4: Run the program
    ./disk_scheduling

Step 5: Enter the inputs
  Example:
    Enter number of disk requests: 8
    Enter disk requests:
    98 183 37 122 14 124 65 67
    Enter initial head position: 53
    Enter total number of cylinders: 200

CODE
----
#include <stdio.h>
#include <stdlib.h>

void SSTF(int requests[], int n, int head)
{
    int visited[100] = {0};
    int total = 0;

    printf("\nSSTF Disk Scheduling:\n");
    printf("Head Movement: %d", head);

    for (int i = 0; i < n; i++)
    {
        int min = 9999, index = -1;

        for (int j = 0; j < n; j++)
        {
            if (!visited[j] && abs(head - requests[j]) < min)
            {
                min = abs(head - requests[j]);
                index = j;
            }
        }

        visited[index] = 1;
        total += abs(head - requests[index]);
        head = requests[index];

        printf(" -> %d", head);
    }

    printf("\nTotal Head Movement = %d\n", total);
}

void SCAN(int requests[], int n, int head, int disk_size)
{
    int arr[102], total = 0, index = 0;

    for (int i = 0; i < n; i++)
        arr[i] = requests[i];

    arr[n] = 0;
    arr[n + 1] = disk_size - 1;

    // Sort requests and disk boundaries
    for (int i = 0; i < n + 2; i++)
    {
        for (int j = i + 1; j < n + 2; j++)
        {
            if (arr[i] > arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    while (index < n + 2 && arr[index] < head)
        index++;

    printf("\nSCAN Disk Scheduling:\n");
    printf("Head Movement: %d", head);

    // Move towards higher cylinder numbers first
    for (int i = index; i < n + 2; i++)
    {
        total += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }

    // Reverse direction
    for (int i = index - 1; i >= 0; i--)
    {
        total += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }

    printf("\nTotal Head Movement = %d\n", total);
}

void CLOOK(int requests[], int n, int head)
{
    int arr[100], total = 0, index = 0;

    for (int i = 0; i < n; i++)
        arr[i] = requests[i];

    // Sort requests
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (arr[i] > arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

    while (index < n && arr[index] < head)
        index++;

    printf("\nC-LOOK Disk Scheduling:\n");
    printf("Head Movement: %d", head);

    // Move towards higher cylinder numbers
    for (int i = index; i < n; i++)
    {
        total += abs(head - arr[i]);
        head = arr[i];
        printf(" -> %d", head);
    }

    // Jump to lowest request
    if (index > 0)
    {
        total += abs(head - arr[0]);
        head = arr[0];
        printf(" -> %d", head);

        for (int i = 1; i < index; i++)
        {
            total += abs(head - arr[i]);
            head = arr[i];
            printf(" -> %d", head);
        }
    }

    printf("\nTotal Head Movement = %d\n", total);
}

int main()
{
    int n, head, disk_size;
    int requests[100];

    printf("Enter number of disk requests: ");
    scanf("%d", &n);

    if (n <= 0 || n > 100)
    {
        printf("Invalid number of requests.\n");
        return 1;
    }

    printf("Enter disk requests:\n");
    for (int i = 0; i < n; i++)
        scanf("%d", &requests[i]);

    printf("Enter initial head position: ");
    scanf("%d", &head);

    printf("Enter total number of cylinders: ");
    scanf("%d", &disk_size);

    if (disk_size <= 0 || head < 0 || head >= disk_size)
    {
        printf("Invalid disk size or head position.\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        if (requests[i] < 0 || requests[i] >= disk_size)
        {
            printf("Invalid request position.\n");
            return 1;
        }
    }

    SSTF(requests, n, head);
    SCAN(requests, n, head, disk_size);
    CLOOK(requests, n, head);

    return 0;
}

NOTE: SCAN and C-LOOK initially move towards higher cylinder numbers
(away from the spindle).


==============================================================
                        END OF FILE
==============================================================
