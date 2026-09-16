#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv) {
  
  printf("Used memory: %d bytes\n", getusedmem());

  return 0;
}
