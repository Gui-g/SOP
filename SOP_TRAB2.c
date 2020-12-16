#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>

pthread_barrier_t barrier;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int obtido = 0;
int *resultado;

typedef struct arguments {
    int** matrix;
    int num;
    int row;
    int col;
    int option;
    int round;
} args;

void swap(int *num1, int *num2);
void random_array(int array[], int n);
void *search_matrix(void *arguments);
int main(int argc, char* argv[]) {
    
    /*carregar dados*/
    char *p;
    long m = strtol(argv[1], &p, 10);
    long n = strtol(argv[2], &p, 10);
    long r = strtol(argv[3], &p, 10);
    int i, j;
    /*vetor resultado*/
    resultado = (int*)malloc(r*sizeof(int));
    for(i = 0; i < r; i++) {
        resultado[i] = 0;
    }
    /*inicializar e popular matriz (com auxilio de um vetor randomizado)*/
    int **matriz = (int**)malloc(m*sizeof(int*));
    for(i=0;i<m;i++) {
        matriz[i] = (int*)malloc(n*sizeof(int));
    }

    int *r_array = (int*)malloc(m*n*sizeof(int));
    for(i=0;i<m*n;i++)
        r_array[i] = i+1;
    random_array(r_array, m*n);

    int count = 0;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            matriz[i][j] = r_array[count];
            count++;
        }
    }

    /* barreira */
    int rc;
    rc = pthread_barrier_init(&barrier, NULL, 4);
    if (rc != 0) {
	    perror("erro em pthread_barrier_init()");
        exit(1);
    }

    /*mutex init*/
    pthread_mutex_init(&mutex, NULL);

    /*threads*/
    pthread_t threads[4];
    args new_arg;  
    new_arg.matrix = matriz;
    new_arg.row = m;
    new_arg.col = n;

    for(j=0;j<r;j++) {
        new_arg.round = j+1;
        new_arg.num = rand() % m*n+1;
        obtido = 0;
        for (i=0; i<4; i++) {
            new_arg.option = i+1;
            rc = pthread_create(&threads[i], NULL, search_matrix, &new_arg);
            usleep(500);
        }

        for (i = 0; i < 4; i++) {
	        rc = pthread_join(threads[i], NULL);
        }
    }

    int result_sum;
    int result_sum_array[4];
    int winner = 0;
    for(i=0;i<4;i++) {
        result_sum = 0;
        for(j=0;j<r;j++){
            if(resultado[j] == i+1)
                result_sum++;
        }
        if(result_sum > winner)
            winner = result_sum;
        result_sum_array[i] = result_sum;
        printf("thread %d => %d vitorias\n", i+1, result_sum);
    }
    printf("-------------------------\n");
    printf("Thread(s) vencedora(s): ");
    for(i=0;i<4;i++) {
        if(result_sum_array[i] == winner)
            printf("%d ", i+1);
    }
    printf("\n");
}

void swap(int *num1, int *num2) {
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

void random_array(int array[], int n) {
    int i;
    time_t t;

    srand((unsigned) time(&t));
    for (int i = n-1; i > 0; i--) 
    {  
        int j = rand() % (i+1); 
   
        swap(&array[i], &array[j]); 
    } 
}

void *search_matrix(void *arguments) {
    int i, j;
    args *func_arg = arguments;
    args new_func_arg = *func_arg;
    pthread_barrier_wait(&barrier);
    switch(new_func_arg.option) {
        case 1:
            for(i = 0; i < new_func_arg.row; i++) {
                for(j = 0; j < new_func_arg.col; j++) {
                    if (new_func_arg.matrix[i][j] == new_func_arg.num) {
                        pthread_mutex_lock(&mutex);
                        if(obtido == 0) {
                            *(resultado + new_func_arg.round-1) = new_func_arg.option;
                            obtido = 1;
                        }
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }
        break;
        case 2:
            for(i = 0; i < new_func_arg.row; i++) {
                for(j = new_func_arg.col-1; j >= 0; j--) {
                    if (new_func_arg.matrix[i][j] == new_func_arg.num) {
                        pthread_mutex_lock(&mutex);
                        if(obtido == 0) {
                            *(resultado + new_func_arg.round-1) = new_func_arg.option;
                            obtido = 1;
                        }
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }
        break;
        case 3:
            for(i = new_func_arg.row-1; i >= 0; i--) {
                for(j = 0; j < new_func_arg.col; j++) {
                    if (new_func_arg.matrix[i][j] == new_func_arg.num) {
                        pthread_mutex_lock(&mutex);
                        if(obtido == 0) {
                            *(resultado + new_func_arg.round-1) = new_func_arg.option;
                            obtido = 1;
                        }
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }
        break;
        case 4:
            for(i = new_func_arg.row-1; i >= 0; i--) {
                for(j = new_func_arg.col-1; j >= 0; j--) {
                    if (new_func_arg.matrix[i][j] == new_func_arg.num) {
                        pthread_mutex_lock(&mutex);
                        if(obtido == 0) {
                            *(resultado + new_func_arg.round-1) = new_func_arg.option;
                            obtido = 1;
                        }
                        pthread_mutex_unlock(&mutex);
                    }
                }
            }
        break;       
    }
    
    return 0;
}

