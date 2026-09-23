#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    printf("Running test_nullptr...\n");
    
    int pid = fork();
    if (pid == 0) {
        int *null_ptr = 0;
        // Dereferencing the null pointer should cause a segmentation fault
        *null_ptr = 100; 
        
        // If this line prints, the kernel failed to kill the process
        printf("test_nullptr: FAILED (Process was not killed)\n");
        exit(1);
    }
    
    wait(0);
    printf("test_nullptr: OK (Ensure 'Segmentation Fault!' was printed above by the kernel)\n");
    exit(0);
}
