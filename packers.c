#include <stdio.h>
#include<pthread.h>
#include "factory.h"
#include "packers.h"
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
//initialize buffer address,lock, and other varibles.
Product *buffer;
int buffersize;
int *runcount;
int out=0;
int productboxed=0;
int countc=0;
int tpacker;
pthread_mutex_t lock2 =PTHREAD_MUTEX_INITIALIZER;
//following two lines from https://eclass.srv.ualberta.ca/pluginfile.php/2678507/mod_resource/content/1/sample_colors
const char *COLORS[] = {"AliceBlue","AntiqueWhite","Aqua","Aquamarine","Azure","Beige","Bisque","Black","BlanchedAlmond","Blue","BlueViolet","Brown","BurlyWood","CadetBlue","Chartreuse","Chocolate","Coral","CornflowerBlue","Cornsilk","Crimson","Cyan","DarkBlue","DarkCyan","DarkGodenRod","DarkGray","DarkGrey","DarkGreen","DarkKhaki","DarkMagenta","DarkOliveGreen","Darkorange","DarkOrchid","DarkRed","DarkSalmon","DarkSeaGreen","DarkSlateBlue","DarkSlateGray","DarkSlateGrey","DarkTurquoise","DarkViolet","DeepPink","DeepSkyBlue","DimGray","DimGrey","DodgerBlue","FireBrick","FloralWhite","ForestGreen","Fuchsia","Gainsboro","GhostWhite","God","GodenRod","Gray","Grey","Green","GreenYellow","HoneyDew","HotPink","IndianRed","Indigo","Ivory","Khaki","Lavender","LavenderBlush","LawnGreen","LemonChiffon","LightBlue","LightCoral","LightCyan","LightGodenRodYellow","LightGray","LightGrey","LightGreen","LightPink","LightSalmon","LightSeaGreen","LightSkyBlue","LightSlateGray","LightSlateGrey","LightSteelBlue","LightYellow","Lime","LimeGreen","Linen","Magenta","Maroon","MediumAquaMarine","MediumBlue","MediumOrchid","MediumPurple","MediumSeaGreen","MediumSlateBlue","MediumSpringGreen","MediumTurquoise","MediumVioletRed","MidnightBlue","MintCream","MistyRose","Moccasin","NavajoWhite","Navy","OdLace","Olive","OliveDrab","Orange","OrangeRed","Orchid","PaleGodenRod","PaleGreen","PaleTurquoise","PaleVioletRed","PapayaWhip","PeachPuff","Peru","Pink","Plum","PowderBlue","Purple","Red","RosyBrown","RoyalBlue","SaddleBrown","Salmon","SandyBrown","SeaGreen","SeaShell","Sienna","Silver","SkyBlue","SlateBlue","SlateGray","SlateGrey","Snow","SpringGreen","SteelBlue","Tan","Teal","Thistle","Tomato","Turquoise","Violet","Wheat","White","WhiteSmoke","Yellow","YellowGreen"};
/** @brief init packer, receivous information from factory.
 *  @param the buffer address
 *  @param the buffer size
 *  @param the runtime count address
 *  @param the total number of thread
 *  @return void.
 */
void init_packers(Product *bufferaddress,int size, int *pcount,int p){
  buffer=bufferaddress;
  buffersize=size;
  runcount=pcount;
  tpacker=p;
}

/** @brief recrive information from the thread creater.
 *         Call PackToBox
 *  @param packer information struct
 */
void *packer(void( *arg)){
  //Get information
  PInfo *pinfo=(PInfo *) arg;
  int boxnum=pinfo->boxnumber;
  int iteminbox=pinfo->nitem;
  int remain=pinfo->remain;
  //Free memory
  free(pinfo);
  PackToBox(iteminbox,boxnum,remain);
  return((void *)0);
}

/** @brief Controll access to the buffer and call PackProduct
 *  @param the maximum item per box.
 *  @param the total box number
 *  @param the remainning number to be boxed.
 *  @return void.
 */
void PackToBox(int maxitem,int boxnum,int remain){
  int iteminbox=maxitem;
  Product *box=malloc(sizeof(Product)*(long unsigned int)(iteminbox));
  int i;
  //Stop if all remainning running times are 0.
  while(runcount[boxnum]>0){
    //Decrease count
    runcount[boxnum]--;
    //Check if it is the last box to pack remainnings.
    if(remain>0 && runcount[tpacker-1]==0 && boxnum==(tpacker-1)){
      //Check how many products is boxed in a box.
      for (i=0;i<remain;i++){
        countc++;
        //Gos to lock and get product from buffer.
        pthread_mutex_lock(&lock2);
        if (countc<=totalproducts){
          while (countbuffer==0){
            pthread_cond_wait(&qready2,&lock2);
          }
          PackProduct(box,i,remain);
          pthread_cond_signal(&qready);
        }
        else{
          NotFullBox(box,i,iteminbox);
        }
          pthread_mutex_unlock(&lock2);
      }
       int seconds;
        seconds=rand()%2;
        seconds=seconds*100;
        usleep((unsigned int)(seconds));
    }
    else{
      //Check how many products is boxed in a box.
      for (i=0;i<iteminbox;i++){
        //Gos to lock and get products from buffer.
        pthread_mutex_lock(&lock2);
        countc++;
        //printf("%d\n",countc);
        if (countc<=totalproducts){
          while (countbuffer==0){
            pthread_cond_wait(&qready2,&lock2);
          }
          PackProduct(box,i,iteminbox);
          pthread_cond_signal(&qready);
        }
        else{
          NotFullBox(box,i,iteminbox);
        }
        pthread_mutex_unlock(&lock2);
      
        int seconds;
        seconds=rand()%2;
        seconds=seconds*100;
        usleep((unsigned int)(seconds));
      }
    }
  }
  
  free(box);
  
}

/** @brief Products from buffer
 *         Print
 *  @param list of product in box
 *  @param index of product
 *  @param maxmum product
 *  @return void.
 */
void PackProduct(Product *blist,int i,int maxitem){
//Get product from buffer
  Product *p=malloc(1*sizeof(Product));
  p->number=buffer[out].number;
  p->color=buffer[out].color;
  blist[i]=*p;
  countbuffer--;
  productboxed++;
  out = (out + 1) %buffersize;
  free(p);
  //If it is the last product in box, print
  if(i==(maxitem-1)){
    int boxi;
    pthread_t tid;
    tid=pthread_self();
    int ptid=(int)(tid%10000);
    printf("[Packer %04d]: I have a box of products containing: ",ptid);
    for (boxi=0;boxi<maxitem;boxi++){
      if (boxi>0){
        printf(",");
      }
      printf(" %s%d ",COLORS[blist[boxi].color],blist[boxi].number);
    }
    printf("\n");
    printf("\n");
  }
}

/** @brief If all products are packer, add null
 *  @param list of product in box
 *  @param index of product
 *  @param maxmum product
 *  @return void.
 */
void NotFullBox(Product *blist,int i,int maxitem){
  Product *p=malloc(1*sizeof(Product));
  p->number=-1;
  p->color=-1;
  blist[i]=*p;
  free(p);
  if(i==(maxitem-1)){
    int boxi;
    pthread_t tid;
    tid=pthread_self();
    int ptid=(int)(tid%10000);
    if( blist[0].color>(-1)){
      printf("[Packer %04d]: I have a box of products containing: ",ptid);
      for (boxi=0;boxi<maxitem;boxi++){
        if (blist[boxi].color>(-1)){
          printf(" %s%d, ",COLORS[blist[boxi].color],blist[boxi].number);
        }
        else{
          printf(" ");
        } 
      }
      printf("\n");
    }
  }
}
