#include "common.h"
#include "difftest.h"

#define RAMSIZE (128 * 1024 * 1024)

static paddr_t ram[RAMSIZE / sizeof(paddr_t)];
static long img_size = 0;
void* get_img_start() { return &ram[0]; }
long get_img_size() { return RAMSIZE; }

void addpageSv39() {
//three layers
//addr range: 0x0000000080000000 - 0x0000000088000000 for 128MB from 2GB - 2GB128MB
//the first layer: one entry for 1GB. (512GB in total by 512 entries). need the 2th entries
//the second layer: one entry for 2MB. (1GB in total by 512 entries). need the 0th-63rd entries
//the third layer: one entry for 4KB (2MB in total by 512 entries). need 64 with each one all  

#define PAGESIZE (4 * 1024)  // 4KB = 2^12B
#define ENTRYNUM (PAGESIZE / 8) //512 2^9
#define PTEVOLUME (PAGESIZE * ENTRYNUM) // 2MB
#define PTENUM (RAMSIZE / PTEVOLUME) // 128MB / 2MB = 64
#define PDDENUM 1
#define PDENUM 1
#define PDDEADDR (0x88000000 - (PAGESIZE * (PTENUM + 2))) //0x88000000 - 0x1000*66
#define PDEADDR (0x88000000 - (PAGESIZE * (PTENUM + 1))) //0x88000000 - 0x1000*65
#define PTEADDR(i) (0x88000000 - (PAGESIZE * PTENUM) + (PAGESIZE * i)) //0x88000000 - 0x100*64
#define PTEMMIONUM 128
#define PDEMMIONUM 1

  uint64_t pdde[ENTRYNUM];
  uint64_t pde[ENTRYNUM];
  uint64_t pte[PTENUM][ENTRYNUM];
  
  //special addr for mmio 0x40000000 - 0x4fffffff
  uint64_t pdemmio[ENTRYNUM];
  uint64_t ptemmio[PTEMMIONUM][ENTRYNUM];
  
  pdde[1] = (((PDDEADDR-PAGESIZE*1) & 0xfffff000) >> 2) | 0x1;

  for(int i = 0; i < PTEMMIONUM; i++) {
    pdemmio[i] = (((PDDEADDR-PAGESIZE*(PTEMMIONUM+PDEMMIONUM-i)) & 0xfffff000) >> 2) | 0x1;
  }
  
  for(int outidx = 0; outidx < PTEMMIONUM; outidx++) {
    for(int inidx = 0; inidx < ENTRYNUM; inidx++) {
      ptemmio[outidx][inidx] = (((0x40000000 + outidx*PTEVOLUME + inidx*PAGESIZE) & 0xfffff000) >> 2) | 0xf;
    }
  }
  
  //0x800000000 - 0x87ffffff
  pdde[2] = ((PDEADDR & 0xfffff000) >> 2) | 0x1;
  //pdde[2] = ((0x80000000&0xc0000000) >> 2) | 0xf;

  for(int i = 0; i < PTENUM ;i++) {
    pde[i] = ((PTEADDR(i)&0xfffff000)>>2) | 0x1;
    //pde[i] = (((0x8000000+i*2*1024*1024)&0xffe00000)>>2) | 0xf;
  }

  for(int outidx = 0; outidx < PTENUM; outidx++ ) {
    for(int inidx = 0; inidx < ENTRYNUM; inidx++ ) {
      pte[outidx][inidx] = (((0x80000000 + outidx*PTEVOLUME + inidx*PAGESIZE) & 0xfffff000)>>2) | 0xf;
    }
  }

  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDENUM+PDENUM+PDEMMIONUM+PTEMMIONUM)),ptemmio, PAGESIZE*PTEMMIONUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDENUM+PDENUM+PDEMMIONUM)), pdemmio, PAGESIZE*PDEMMIONUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDENUM+PDENUM)), pdde, PAGESIZE*PDDENUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDENUM)), pde, PAGESIZE*PDENUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*PTENUM), pte, PAGESIZE*PTENUM);
}

void addpageSv57() {
//three layers
//addr range: 0x0000000080000000 - 0x0000000088000000 for 128MB from 2GB - 2GB128MB
//the first layer: one entry for 1GB. (512GB in total by 512 entries). need the 2th entries
//the second layer: one entry for 2MB. (1GB in total by 512 entries). need the 0th-63rd entries
//the third layer: one entry for 4KB (2MB in total by 512 entries). need 64 with each one all  

#define PAGESIZE (4 * 1024)  // 4KB = 2^12B
#define ENTRYNUM (PAGESIZE / 8) //512 2^9
#define PTEVOLUME (PAGESIZE * ENTRYNUM) // 2MB
#define PTENUM (RAMSIZE / PTEVOLUME) // 128MB / 2MB = 64
#define PDDDDENUM 1
#define PDDDENUM 1
#define PDDENUM 1
#define PDENUM 1
#define PDDDDEADDR (0x88000000 - (PAGESIZE * (PTENUM + 4))) //0x88000000 - 0x1000*68
#define PDDDEADDR (0x88000000 - (PAGESIZE * (PTENUM + 3))) //0x88000000 - 0x1000*67
#define PDDEADDR (0x88000000 - (PAGESIZE * (PTENUM + 2))) //0x88000000 - 0x1000*66
#define PDEADDR (0x88000000 - (PAGESIZE * (PTENUM + 1))) //0x88000000 - 0x1000*65
#define PTEADDR(i) (0x88000000 - (PAGESIZE * PTENUM) + (PAGESIZE * i)) //0x88000000 - 0x100*64
#define PTEMMIONUM 128
#define PDEMMIONUM 1
#define PTEDEVNUM 128
#define PDEDEVNUM 1

  uint64_t pdddde[ENTRYNUM];
  uint64_t pddde[ENTRYNUM];
  uint64_t pdde[ENTRYNUM];
  uint64_t pde[ENTRYNUM];
  uint64_t pte[PTENUM][ENTRYNUM];
  
  //special addr for mmio 0x40000000 - 0x4fffffff
  uint64_t pdemmio[ENTRYNUM];
  uint64_t ptemmio[PTEMMIONUM][ENTRYNUM];
  
  // special addr for internal devices 0x30000000-0x3fffffff
  uint64_t pdedev[ENTRYNUM];
  uint64_t ptedev[PTEDEVNUM][ENTRYNUM];

  // dev: 0x30000000-0x3fffffff
  pdde[0] = (((PDDDDEADDR-PAGESIZE*(PDEMMIONUM+PTEMMIONUM+PDEDEVNUM)) & 0xfffff000) >> 2) | 0x1;

  for (int i = 0; i < PTEDEVNUM; i++) {
    pdedev[ENTRYNUM-PTEDEVNUM+i] = (((PDDDDEADDR-PAGESIZE*(PDEMMIONUM+PTEMMIONUM+PDEDEVNUM+PTEDEVNUM-i)) & 0xfffff000) >> 2) | 0x1;
  }

  for(int outidx = 0; outidx < PTEDEVNUM; outidx++) {
    for(int inidx = 0; inidx < ENTRYNUM; inidx++) {
      ptedev[outidx][inidx] = (((0x30000000 + outidx*PTEVOLUME + inidx*PAGESIZE) & 0xfffff000) >> 2) | 0xf;
    }
  }

  pdde[1] = (((PDDDDEADDR-PAGESIZE*1) & 0xfffff000) >> 2) | 0x1;

  for(int i = 0; i < PTEMMIONUM; i++) {
    pdemmio[i] = (((PDDDDEADDR-PAGESIZE*(PTEMMIONUM+PDEMMIONUM-i)) & 0xfffff000) >> 2) | 0x1;
  }
  
  for(int outidx = 0; outidx < PTEMMIONUM; outidx++) {
    for(int inidx = 0; inidx < ENTRYNUM; inidx++) {
      ptemmio[outidx][inidx] = (((0x40000000 + outidx*PTEVOLUME + inidx*PAGESIZE) & 0xfffff000) >> 2) | 0xf;
    }
  }
  
  //0x800000000 - 0x87ffffff
  pdddde[0] = ((PDDDEADDR & 0xfffff000) >> 2) | 0x1;
  //pdddde[511] = ((PDDDEADDR & 0xfffff000) >> 2) | 0x1;
  pddde[0] = ((PDDEADDR & 0xfffff000) >> 2) | 0x1;
  //pddde[511] = ((PDDEADDR & 0xfffff000) >> 2) | 0x1;
  pdde[2] = ((PDEADDR & 0xfffff000) >> 2) | 0x1;
  //pdde[510] = ((PDEADDR & 0xfffff000) >> 2) | 0x1;
  //pdde[2] = ((0x80000000&0xc0000000) >> 2) | 0xf;

  for(int i = 0; i < PTENUM ;i++) {
    pde[i] = ((PTEADDR(i)&0xfffff000)>>2) | 0x1;
    //pde[i] = (((0x8000000+i*2*1024*1024)&0xffe00000)>>2) | 0xf;
  }

  for(int outidx = 0; outidx < PTENUM; outidx++ ) {
    for(int inidx = 0; inidx < ENTRYNUM; inidx++ ) {
      pte[outidx][inidx] = (((0x80000000 + outidx*PTEVOLUME + inidx*PAGESIZE) & 0xfffff000)>>2) | 0xf;
    }
  }

  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM+PDEMMIONUM+PTEMMIONUM+PDEDEVNUM+PTEDEVNUM)),ptedev,PAGESIZE*PTEDEVNUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM+PDEMMIONUM+PTEMMIONUM+PDEDEVNUM)),pdedev,PAGESIZE*PDEDEVNUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM+PDEMMIONUM+PTEMMIONUM)),ptemmio, PAGESIZE*PTEMMIONUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM+PDEMMIONUM)), pdemmio, PAGESIZE*PDEMMIONUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM)), pdddde, PAGESIZE*PDDENUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDDENUM+PDDENUM+PDENUM)), pddde, PAGESIZE*PDDENUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDDENUM+PDENUM)), pdde, PAGESIZE*PDDENUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*(PTENUM+PDENUM)), pde, PAGESIZE*PDENUM);
  memcpy((char *)ram+(RAMSIZE-PAGESIZE*PTENUM), pte, PAGESIZE*PTENUM);
  //printf("ram:%x\n",(ram[0x7fbc000/sizeof(paddr_t)])<<2);
}

void init_ram(const char *img) {
  assert(img != NULL);
  FILE *fp = fopen(img, "rb");
  if (fp == NULL) {
    printf("Can not open '%s'\n", img);
    assert(0);
  }

  printf("The image is %s\n", img);

  fseek(fp, 0, SEEK_END);
  img_size = ftell(fp);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(ram, img_size, 1, fp);
  assert(ret == 1);
  fclose(fp);

  //new add
  //addpageSv39();
  addpageSv57();
  //new end
}

extern "C" void ram_helper(
    paddr_t rIdx, paddr_t *rdata, paddr_t wIdx, paddr_t wdata, paddr_t wmask, uint8_t wen) {
  *rdata = ram[rIdx];

 // printf("ridx:%x\n",rIdx);
  if(rIdx >= (RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM))/sizeof(paddr_t)){
    //printf("%x\n",RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM));
    //printf("%x\n",RAMSIZE-PAGESIZE*(PTENUM+PDDDENUM+PDDENUM+PDENUM));
    //printf("%x\n",rIdx);
  }
  //printf("%x\n",RAMSIZE-PAGESIZE*(PTENUM+PDDDDENUM+PDDDENUM+PDDENUM+PDENUM));
  //printf("%x\n",ram[2+(0x7fbe000)/sizeof(paddr_t)]<<2);
  if (wen) { ram[wIdx] = (ram[wIdx] & ~wmask) | (wdata & wmask); }
}
