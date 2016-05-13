void init_packers(Product *bufferaddress,int size,int *pcount,int p);
void *packer(void( *arg));
void PackToBox(int maxitem,int boxnum,int remain);
void PackProduct(Product *blist,int i,int maxitem);
void NotFullBox(Product *blist,int i,int maxitem);
