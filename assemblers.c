//#include "factory.h"
#include <stdio.h>
#include<pthread.h>
#include "assemblers.h"
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#define MAXCOLORS 147
// initialize lock and other varibous.
pthread_mutex_t lock =PTHREAD_MUTEX_INITIALIZER;
Product *buffer;
int maxsize;
int in=0;
/** @brief init assembler, receive paramaters from factory.
 *  @param the buffer address
 *  @param the buffer size
 *  @return void.
 */
void init_assembler(Product *bufferaddress,int buffersize){
  buffer=bufferaddress;
  maxsize=buffersize;
}
/** @brief get colors and numbers from main function and call PutOnBuffer
 *  @param Struct address from factory
 *  @return void.
 */

void *assembler(void *arg){
  //Get the number of production and the color for each thread.
  AInfo *info=(AInfo *) arg;
  Product *p=malloc(1*sizeof(Product));
  int produce=info->nproduce;
  p->color=info->tnumber;
  //It thread numbers greater than 147, recycle colors.
  if ((p->color)>=MAXCOLORS){
    p->color=(p->color)-MAXCOLORS; 
  }
  //Free the memory in parameter.
  free(info);
  int i;
//Create products and call PutOnBuffer
  for (i=1;i<=produce;i++){
    p->number=i;
    pthread_mutex_lock(&lock);
    while (countbuffer==maxsize){
      pthread_cond_wait(&qready,&lock);
    }
    PutOnBuffer(p);
    pthread_cond_signal(&qready2);
    pthread_mutex_unlock(&lock);
    int seconds;
    seconds=rand()%2;
    seconds=seconds*100;
    usleep((unsigned int)(seconds));
    }
//free memory
  free(p);
  return((void *)0);
}

/** @brief Put product structs on buffer
 *  @param product struct contains color and number
 *  @return void.
 */
void * PutOnBuffer(Product *p){
//Put struct in buffer
  buffer[in]=*p;
  in=(in+1)%maxsize;
  countbuffer++;
  return((void *)0);
}
