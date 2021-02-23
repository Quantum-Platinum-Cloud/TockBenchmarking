#include <perf.h>
#include <stdio.h>

#define DEBUG 0

struct node
{
	struct node *next;
};

int main(void){
  //how many bytes to jump by
  int dists[] = {16, 32, 64};
  struct node ar[64*500/sizeof(struct node)];
#if DEBUG
  printf("Array bounds: %p-%p\n", ar, &ar[64*500/sizeof(struct node)-1]);
#endif

  //call perfcycles syscall
  for(int i = 0; i < 3; i++){
    int jumpDist = dists[i]/sizeof(struct node);

    //set up pointers
    struct node* startp = &ar[0];
    for(int j = 0; j < 499; j++){
      startp->next = startp + jumpDist;
#if DEBUG
      printf("dist: %ld\n", (long)(startp->next) - (long)(startp));
#endif
      startp += jumpDist;
    }
    startp = NULL;
#if DEBUG
    printf("end ptr: %p\n", startp);
    int count = 0;
#endif

    //do test
    unsigned start, end;
    startp = &ar[0];
    start = perf_cycles();
    while(startp){
      startp = startp->next;
#if DEBUG
      count++;
#endif
    }
    end = perf_cycles();
    printf("Cycles: %d\n", end-start);
#if DEBUG
    printf("%d:\t%p\n", count, startp);
#endif
  }
}
