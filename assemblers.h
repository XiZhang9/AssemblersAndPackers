#pragma once
#include "factory.h"
void init_assembler(Product *bufferaddress,int buffersize);
 void *PutOnBuffer(Product *p);
void *assembler(void *arg);
