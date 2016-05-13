#include<pthread.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>
#pragma once
sem_t empty;
sem_t full;
int totalproducts;
int countbuffer;
extern pthread_cond_t qready;
extern pthread_cond_t qready2;
typedef struct Passinfo {
int  tnumber;
int  nproduce;
} AInfo;
typedef struct Packerinfo {
int  boxnumber;
int  nitem;
int remain;
} PInfo;
typedef struct Production {
int  color;
int  number;
} Product;
