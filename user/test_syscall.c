#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
    printf("Running test_syscalls...\n");
    char *address = sbrk(100);
    *address = 0x12;

    // Test error handling for invalid arguments
    if (mprotect((void*)0xFFFFFFFFFFFFFFFF) != -1 || munprotect((void*)0xFFFFFFFFFFFFFFFF) != -1) {
        printf("test_syscalls: FAILED (Did not return appropriate error code for invalid address)\n");
        exit(1);
    }

    mprotect(address);

    int pid = fork();
    if (pid == 0) {
        // Child process: Attempt to write to a protected page
        // This should cause the kernel to kill the child process
        *address = 0x34;
        printf("test_syscalls: FAILED (Child process was allowed to write to read-only page)\n");
        exit(1);
    }
    
    // Parent waits for child to be killed by the OS
    wait(0); 

    // Test restoring permissions
    munprotect(address);
    *address = 0x56;

    if (*address == 0x56) {
        printf("test_syscalls: OK\n");
    } else {
        printf("test_syscalls: FAILED (Could not write after munprotect)\n");
    }

    exit(0);
}
