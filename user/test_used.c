#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    printf("Running test_used...\n");
    
    // Test base system call execution
    int initial_mem = getusedmem();
    printf("Used memory: %d bytes\n", initial_mem);
    
    // Allocate 4096 bytes (one page) to verify the walk_used function updates the count
    sbrk(4096);
    int new_mem = getusedmem();
    printf("Used memory after allocation: %d bytes\n", new_mem);
    
    if (new_mem > initial_mem) {
        printf("test_used: OK\n");
    } else {
        printf("test_used: FAILED (memory did not increase after allocation)\n");
    }
    
    exit(0);
}
