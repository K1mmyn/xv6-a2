// user/nullwrite.c
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("about to write to a null pointer...\n");

  int *p = 0;
  *p = 42;      // should trigger "Segmentation fault!"

  printf("ERROR: write to null succeeded\n");
  exit(1);
}
