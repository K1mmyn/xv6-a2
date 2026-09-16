#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

static int failed = 0;

static void check(int cond, const char *msg) {
  if (cond) {
    printf("[PASS] %s\n", msg);
  } else {
    printf("[FAIL] %s\n", msg);
    failed++;
  }
}

int main(int argc, char *argv[]) {
  int before = getusedmem();
  printf("getusedmem() = %d bytes (%d pages)\n", before, before / PGSIZE);

  // 1. Sanity: must be positive and a multiple of the page size.
  check(before > 0, "used memory is positive");
  check(before % PGSIZE == 0, "used memory is a multiple of PGSIZE");

  // 2. Upper bound: cannot exceed physical memory (128 MB in xv6).
  check(before <= 128 * 1024 * 1024, "used memory <= PHYSTOP - KERNBASE");

  // 3. Stability: two back-to-back calls should agree.
  int again = getusedmem();
  check(again == before, "repeated calls return the same value");

  // 4. Allocate some pages and see how the value reacts.
  //    Kernel page table: expect NO change (kernel mappings are static).
  //    Process page table: expect growth of exactly 10 pages.
  int npages = 10;
  char *p = sbrk(npages * PGSIZE);
  check(p != (char *)-1, "sbrk succeeded");

  // Touch pages so they are definitely mapped.
  for (int i = 0; i < npages; i++)
    p[i * PGSIZE] = 1;

  int after = getusedmem();
  printf("after sbrk(+%d pages): %d bytes (delta %d pages)\n", npages, after,
         (after - before) / PGSIZE);

  if (after == before)
    printf("  -> unchanged: consistent with counting the KERNEL page table\n");
  else if (after - before == npages * PGSIZE)
    printf("  -> grew by exactly %d pages: consistent with counting the "
           "PROCESS page table\n",
           npages);
  else
    check(0, "sbrk changed used memory by an unexpected amount");

  // 5. Free the memory and check it returns to the original value.
  sbrk(-npages * PGSIZE);
  int freed = getusedmem();
  check(freed == before, "used memory returns to original after freeing");

  // 6. Fork: a child should observe the same kernel value.
  int pid = fork();
  if (pid == 0) {
    int child = getusedmem();
    exit(child == before ? 0 : 1);
  }
  int status;
  wait(&status);
  check(status == 0, "child sees same used memory as parent");

  printf("\n%s (%d failure%s)\n", failed ? "SOME TESTS FAILED" : "ALL TESTS PASSED",
         failed, failed == 1 ? "" : "s");
  exit(failed ? 1 : 0);
}
