// user/nulltest.c
#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("about to dereference a null pointer...\n");

  // Read from address 0
  int *p = 0;
  int x = *p;   // should trigger "Segmentation fault!"

  // If we get here, the trap handler failed to kill us
  printf("ERROR: read from null succeeded, got %d\n", x);
  exit(1);
}
