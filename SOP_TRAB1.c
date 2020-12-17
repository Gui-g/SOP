#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sched.h>

sem_t semA;

void* A(void *args);
void* B(void *args);
void* C(void *args);
int main() {

sem_init(&semA, 0, 0);

pthread_t threads[3];
int i;

pthread_create(&threads[0],NULL,A,NULL);
pthread_create(&threads[1],NULL,B,NULL);
pthread_create(&threads[2],NULL,C,NULL);

for(i=0;i<3;i++){
    pthread_join(threads[i], NULL);
}

return 0;
}

void* A(void *args) {
    while(1) {
        printf("O");
    }
}

void* B(void *args) {
    while(1) {
        printf("CA");
        printf("!\n");
    }
}

void* C(void *args) {
    while(1) {
        printf("S");
    }
}