/*Write a program to multiply two matrices
using shared memory and parallel Pthreads, one thread per column of the first matrix.
More precisely:
-the matrices are stored in shared memory segments;
-the multiplication computes A × B = C;
-thread k iterates through all valid (i, j) combinations and adds the value
a[i][k] * b[k][j] to c[i][j];
-each position in the result matrix is protected by a mutex to prevent concurrent writes;
-the initial (main) process displays the final result matrix.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Macro for accessing a matrix element
#define MATRIX_ELEM(matrix, rows, cols, i, j) matrix[(i) * (cols) + (j)]

// Structure for shared data
typedef struct {
    int *matrixA;
    int *matrixB;
    int *matrixC;
    pthread_mutex_t *mutexes;
    int rows_a;
    int cols_a;
    int rows_b;
    int cols_b;
} SharedData;

// Structure for thread arguments
typedef struct {
    SharedData *shared;
    int col_index;
} ThreadArgs;

// Function to clean up all allocated resources
void cleanup_resources(SharedData *shared, int shmid_A, int shmid_B, int shmid_C,
                       int shmid_mutex, pthread_t *threads, ThreadArgs *thread_args,
                       int num_mutexes_initialized) {
    if (shared->mutexes != NULL && shared->mutexes != (pthread_mutex_t*)-1) {
        for (int i = 0; i < num_mutexes_initialized; i++) {
            pthread_mutex_destroy(&shared->mutexes[i]);
        }
    }

    if (shared->matrixA != NULL && shared->matrixA != (int*)-1) {
        shmdt(shared->matrixA);
    }
    if (shared->matrixB != NULL && shared->matrixB != (int*)-1) {
        shmdt(shared->matrixB);
    }
    if (shared->matrixC != NULL && shared->matrixC != (int*)-1) {
        shmdt(shared->matrixC);
    }
    if (shared->mutexes != NULL && shared->mutexes != (pthread_mutex_t*)-1) {
        shmdt(shared->mutexes);
    }

    if (shmid_A >= 0) {
        shmctl(shmid_A, IPC_RMID, NULL);
    }
    if (shmid_B >= 0) {
        shmctl(shmid_B, IPC_RMID, NULL);
    }
    if (shmid_C >= 0) {
        shmctl(shmid_C, IPC_RMID, NULL);
    }
    if (shmid_mutex >= 0) {
        shmctl(shmid_mutex, IPC_RMID, NULL);
    }

    if (threads != NULL) {
        free(threads);
    }
    if (thread_args != NULL) {
        free(thread_args);
    }
}

// Thread function: processes one column of matrix A
void* multiply_column(void* arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    SharedData *shared = args->shared;
    int k = args->col_index;

    printf("Thread %d: Processing column %d of matrix A\n", k, k);

    for (int i = 0; i < shared->rows_a; i++) {
        int aik = MATRIX_ELEM(shared->matrixA, shared->rows_a, shared->cols_a, i, k);

        for (int j = 0; j < shared->cols_b; j++) {
            int bkj = MATRIX_ELEM(shared->matrixB, shared->rows_b, shared->cols_b, k, j);
            int product = aik * bkj;

            int pos = i * shared->cols_b + j;

            pthread_mutex_lock(&shared->mutexes[pos]);
            MATRIX_ELEM(shared->matrixC, shared->rows_a, shared->cols_b, i, j) += product;
            pthread_mutex_unlock(&shared->mutexes[pos]);

            printf("Thread %d: C[%d][%d] += A[%d][%d] * B[%d][%d] = %d * %d\n",
                   k, i, j, i, k, k, j, aik, bkj);
        }
    }

    pthread_exit(NULL);
}

// Print a matrix
void print_matrix(int *matrix, int rows, int cols, const char *name) {
    printf("\nMatrix %s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d ", MATRIX_ELEM(matrix, rows, cols, i, j));
        }
        printf("\n");
    }
}

// Read matrices from file
int read_matrices_from_file(const char *filename, int *matrixA, int *matrixB,
                            int rows_a, int cols_a, int rows_b, int cols_b) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening input file");
        return 0;
    }

    printf("Reading matrix A from file...\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            if (fscanf(file, "%d", &MATRIX_ELEM(matrixA, rows_a, cols_a, i, j)) != 1) {
                fprintf(stderr, "Error reading element A[%d][%d]\n", i, j);
                fclose(file);
                return 0;
            }
        }
    }

    printf("Reading matrix B from file...\n");
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            if (fscanf(file, "%d", &MATRIX_ELEM(matrixB, rows_b, cols_b, i, j)) != 1) {
                fprintf(stderr, "Error reading element B[%d][%d]\n", i, j);
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    printf("Matrices successfully read!\n");
    return 1;
}

int main(int argc, char *argv[]) {
    int shmid_A = -1, shmid_B = -1, shmid_C = -1, shmid_mutex = -1;
    pthread_t *threads = NULL;
    ThreadArgs *thread_args = NULL;
    int num_mutexes_initialized = 0;

    // Command-line argument check
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <input_file> <rows_A> <cols_A> <rows_B> <cols_B>\n", argv[0]);
        fprintf(stderr, "Example: %s matrix.txt 3 4 4 3\n", argv[0]);
        exit(1);
    }

    const char *filename = argv[1];
    int ROWS_A = atoi(argv[2]);
    int COLS_A = atoi(argv[3]);
    int ROWS_B = atoi(argv[4]);
    int COLS_B = atoi(argv[5]);

    // Dimension validation
    if (ROWS_A <= 0 || COLS_A <= 0 || ROWS_B <= 0 || COLS_B <= 0) {
        fprintf(stderr, "Error: Matrix dimensions must be positive!\n");
        exit(1);
    }

    if (COLS_A != ROWS_B) {
        fprintf(stderr, "Error: Matrices are not compatible for multiplication!\n");
        fprintf(stderr, "COLS_A (%d) must be equal to ROWS_B (%d)\n", COLS_A, ROWS_B);
        exit(1);
    }

    SharedData shared = {NULL, NULL, NULL, NULL, ROWS_A, COLS_A, ROWS_B, COLS_B};

    // Compute memory sizes
    int size_A = ROWS_A * COLS_A * sizeof(int);
    int size_B = ROWS_B * COLS_B * sizeof(int);
    int size_C = ROWS_A * COLS_B * sizeof(int);
    int size_mutex = ROWS_A * COLS_B * sizeof(pthread_mutex_t);

    // Shared memory allocation
    shmid_A = shmget(IPC_PRIVATE, size_A, IPC_CREAT | 0666);
    if (shmid_A < 0) {
        perror("shmget for matrix A");
        exit(1);
    }

    shmid_B = shmget(IPC_PRIVATE, size_B, IPC_CREAT | 0666);
    if (shmid_B < 0) {
        perror("shmget for matrix B");
        cleanup_resources(&shared, shmid_A, -1, -1, -1, NULL, NULL, 0);
        exit(1);
    }

    shmid_C = shmget(IPC_PRIVATE, size_C, IPC_CREAT | 0666);
    if (shmid_C < 0) {
        perror("shmget for matrix C");
        cleanup_resources(&shared, shmid_A, shmid_B, -1, -1, NULL, NULL, 0);
        exit(1);
    }

    shmid_mutex = shmget(IPC_PRIVATE, size_mutex, IPC_CREAT | 0666);
    if (shmid_mutex < 0) {
        perror("shmget for mutexes");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, -1, NULL, NULL, 0);
        exit(1);
    }

    // Attach shared memory
    shared.matrixA = (int*)shmat(shmid_A, NULL, 0);
    if (shared.matrixA == (int*)-1) {
        perror("shmat for matrix A");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, 0);
        exit(1);
    }

    shared.matrixB = (int*)shmat(shmid_B, NULL, 0);
    if (shared.matrixB == (int*)-1) {
        perror("shmat for matrix B");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, 0);
        exit(1);
    }

    shared.matrixC = (int*)shmat(shmid_C, NULL, 0);
    if (shared.matrixC == (int*)-1) {
        perror("shmat for matrix C");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, 0);
        exit(1);
    }

    shared.mutexes = (pthread_mutex_t*)shmat(shmid_mutex, NULL, 0);
    if (shared.mutexes == (pthread_mutex_t*)-1) {
        perror("shmat for mutexes");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, 0);
        exit(1);
    }

    // Read matrices from file
    if (!read_matrices_from_file(filename, shared.matrixA, shared.matrixB,
                                 ROWS_A, COLS_A, ROWS_B, COLS_B)) {
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, 0);
        exit(1);
    }

    // Initialize matrix C with zeros
    for (int i = 0; i < ROWS_A * COLS_B; i++) {
        shared.matrixC[i] = 0;
    }

    // Initialize mutexes
    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr) != 0) {
        perror("pthread_mutexattr_init");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, 0);
        exit(1);
    }

    for (int i = 0; i < ROWS_A * COLS_B; i++) {
        if (pthread_mutex_init(&shared.mutexes[i], &attr) != 0) {
            perror("pthread_mutex_init");
            pthread_mutexattr_destroy(&attr);
            cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, i);
            exit(1);
        }
        num_mutexes_initialized++;
    }

    pthread_mutexattr_destroy(&attr);

    // Display input matrices
    print_matrix(shared.matrixA, ROWS_A, COLS_A, "A");
    print_matrix(shared.matrixB, ROWS_B, COLS_B, "B");

    // Allocate threads
    threads = (pthread_t*)malloc(COLS_A * sizeof(pthread_t));
    if (threads == NULL) {
        perror("malloc for threads");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, NULL, NULL, num_mutexes_initialized);
        exit(1);
    }

    thread_args = (ThreadArgs*)malloc(COLS_A * sizeof(ThreadArgs));
    if (thread_args == NULL) {
        perror("malloc for thread_args");
        cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex, threads, NULL, num_mutexes_initialized);
        exit(1);
    }

    // Create threads
    printf("\n=== Parallel computation started ===\n");
    for (int k = 0; k < COLS_A; k++) {
        thread_args[k].shared = &shared;
        thread_args[k].col_index = k;

        if (pthread_create(&threads[k], NULL, multiply_column, &thread_args[k]) != 0) {
            perror("pthread_create");
            for (int j = 0; j < k; j++) {
                pthread_join(threads[j], NULL);
            }
            cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex,
                              threads, thread_args, num_mutexes_initialized);
            exit(1);
        }
    }

    // Wait for threads to finish
    for (int k = 0; k < COLS_A; k++) {
        pthread_join(threads[k], NULL);
    }

    printf("\n=== Computation finished ===\n");

    // Display result
    print_matrix(shared.matrixC, ROWS_A, COLS_B, "C = A * B");

    // Cleanup resources
    cleanup_resources(&shared, shmid_A, shmid_B, shmid_C, shmid_mutex,
                      threads, thread_args, num_mutexes_initialized);

    printf("\nProgram finished successfully!\n");
    
    return 0;
}
