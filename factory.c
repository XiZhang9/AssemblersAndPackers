#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include <string.h>
#include <stdio.h>
#include "assemblers.h"
#include "packers.h"
#include "factory.h"
#include <assert.h>
pthread_cond_t  qready;
pthread_cond_t  qready2;
/** @brief Create threads and join threads.
 *         Pass parameters to thread.
 *         Create buffer
 *  @param number of argument the user inputed
 *         
 *  @param argument list.
 *
 *  @return void.
 */
int main(int arg, char *argv[]){
  //Make sure we have enough arguments
  if ( arg==6){
    //Get input
    int a=atoi(argv[1]);
    int p=atoi(argv[2]);
    int l=atoi(argv[3]);
    int n=atoi(argv[4]);
    int c=atoi(argv[5]);
    //Calculate total products
    totalproducts=a*c;
    //Calculate packer's total run time.
    int runtime=totalproducts/n;
    //Create a list to store run time for each packer thread.
    int *pcount=malloc(sizeof(int)*(long unsigned int)(p));
    int z;
    for (z=0;z<p;z++){
      pcount[z]=0;
    }
    int bcount;
    //Put run times in a list
    while (runtime>0){
     for (bcount=0;bcount<p;bcount++){
       if(runtime>0){
         pcount[bcount]++;
         runtime--;
       }
     }
    }
    //Calculate remainnings.
    int remain=totalproducts%n;
    if (remain>0){
      pcount[p-1]++;
    }
    //Store assembler IDs and packer IDs
    pthread_t assemblersIDs[a];
    pthread_t packersIDs[p];
    int i;
    int err;
    //Create buffer
    Product *buffer=malloc (sizeof(Product)*(long unsigned int)(l));
    init_assembler(buffer,l);
    init_packers(buffer,l,pcount,p);
    //Creat assembler threads
    for (i=0;i<a;i++){
      AInfo *info=malloc(1*sizeof(AInfo));
      info->tnumber=i;
      info->nproduce=c;
      err=pthread_create( &assemblersIDs[i], NULL, assembler, info );
      
      if (err != 0){
        perror("can’t create thread");
        sleep(1);
        exit(0);
      }
    }
    //Create packer threads
    int count;
    for (count=0;count<p;count++){
      PInfo *pinfo=malloc(1*sizeof(PInfo));
      pinfo->boxnumber=count;
      pinfo->nitem=n;
      pinfo->remain=remain;
      err=pthread_create( &packersIDs[count], NULL, packer, pinfo );
      
      if (err != 0){
        perror("can’t create thread");
        sleep(1);
        exit(0);
      }
    }
    //Join assembler threads
    int wait;
    for (wait=0; wait< a; ++wait) {
      pthread_join(assemblersIDs[wait], 0);
    }
    //Join packer threads
    int wait2;
    for (wait2=0;wait2<p;++wait2){
      pthread_join(packersIDs[wait2],0);
    }
    free(buffer);
    free(pcount);
    // printf("--------------%d/n",*nbuffer);
    //free(assemblersIDs);
    return 0;
  }
  else{
    printf("Too few or too many argument\n Please enter 5 numbers after program name\n ");
    return 0;
  }
}

