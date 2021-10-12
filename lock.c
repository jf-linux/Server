/*************************************************************************
	> File Name: lock.c
	> Author: 
	> Mail: 
	> Created Time: Thu 07 Oct 2021 12:04:20 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/mman.h>
#include<unistd.h>
#define PTHREAD_SIZE 10

int count = 0;
pthread_mutex_t mutex;
pthread_spinlock_t spinlock;
pthread_rwlock_t rwlock;

void* func(void* arg){
    int* pnum = (int*)arg;

    int i = 0;
    while(i++ < 100000){
#if 0
        (*pnum)++;
#elif 0
        pthread_mutex_lock(&mutex);
        (*pnum)++;
        pthread_mutex_unlock(&mutex);
#elif 0
        if(0 != pthread_mutex_trylock(&mutex)){
            i--;
            continue;
        }
        (*pnum)++;
        pthread_mutex_unlock(&mutex);
#elif 0
        pthread_spin_lock(&spinlock);
        (*pnum)++;
        pthread_spin_unlock(&spinlock);
#else
        pthread_rwlock_rdlock(&rwlock);
        (*pnum)++;
        pthread_rwlock_unlock(&rwlock);
#endif

        usleep(1);
    }
}

int main(){
#if 0
    pthread_t threadid[PTHREAD_SIZE] = {0}; 
    pthread_mutex_init(&mutex , NULL);
    pthread_spin_init(&spinlock , PTHREAD_PROCESS_SHARED);
    int i = 0;
    int num = 0;
    for(i = 0 ; i < PTHREAD_SIZE ; i++){
        int ret = pthread_create(&threadid[i] , NULL , func , &count); 
        if(ret){
            break;
        }
    }

    for(i = 0 ; i < 100 ; i++){
        printf("count --> %d\n" , count);
        sleep(1);
    }
#else
    int *pcount = mmap(NULL , sizeof(int) , PROT_READ | PROT_WRITE , MAP_ANON | MAP_SHARED , -1 , 0);

    int i = 0 ;
    pid_t pid = 0;
    for(i = 0 ; i < PTHREAD_SIZE ; i++){
        pid = fork(); 
        if(pid <= 0){
            usleep(1);
            break;
        }
    }

    if(pid > 0){
        for(i = 0 ; i < 100 ; i++){
            printf("count ---> %d\n" , *pcount);
            sleep(1);
        }
    }else{
        int i = 0;
        while(i++ < 100000){
            (*pcount)++; 
            usleep(1);
        }
    }

#endif
    return 0;
}
