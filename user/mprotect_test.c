#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char* address = sbrk(100);
    // write to make sure memory has been allocated and is writable
    *address = 0x12;
    mprotect(address);
    // Try uncommenting this line, and see if the program crashes
    // *address = 0x34;
    printf("%d", (int)(*address));
    munprotect(address);
    *address = 0x56;
    printf("%d", (int)(*address));
    exit(0);
}
