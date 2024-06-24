#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <assert.h>
#include "process_ancestors.h"

// Sys-call number:
#define _PROCESS_ANCESTORS_ 438

/**
 * Prototypes
 */
void test_normal_conditions(void);
void test_edge_cases(void);
void test_invalid_arguments(void);
static void do_syscall_and_print(struct process_info *info_array, long size);
static void do_syscall_failing(struct process_info *info_array, long size, long *num_filled, long ret_code);

/***********************************************************
 * main()
 ***********************************************************/
int main(int argc, char *argv[])
{
    test_normal_conditions();
    test_edge_cases();
    test_invalid_arguments();

    return 0;
}

/***********************************************************
 * Testing routines for specific test
 ***********************************************************/
void test_normal_conditions()
{
    const int size = 5;
    struct process_info info_array[size];
    long num_filled;
    do_syscall_and_print(info_array, size);
}

void test_edge_cases()
{
    const int max_ancestors = 20; // Adjust based on expected depth of process tree
    struct process_info info_array[max_ancestors];
    long num_filled;
    do_syscall_and_print(info_array, max_ancestors);
}

void test_invalid_arguments()
{
    struct process_info info_array[5];
    long num_filled;

    // Zero or negative sizes
    do_syscall_failing(info_array, 0, &num_filled, EINVAL);
    do_syscall_failing(info_array, -1, &num_filled, EINVAL);

    // Bad info_array pointers
    do_syscall_failing(NULL, 5, &num_filled, EFAULT);
    do_syscall_failing((void*)1, 5, &num_filled, EFAULT);

    // Bad num_filled pointers
    do_syscall_failing(info_array, 5, NULL, EFAULT);
    do_syscall_failing(info_array, 5, (long*)1, EFAULT);
}

/***********************************************************
 * Routines to actually make the sys-call and check results
 ***********************************************************/
static void do_syscall_and_print(struct process_info *info_array, long size)
{
    long num_filled;
    printf("\nAttempting syscall with size %ld\n", size);
    long result = syscall(_PROCESS_ANCESTORS_, info_array, size, &num_filled);
    if (result == 0) {
        printf("Syscall successful. Number filled: %ld\n", num_filled);
        for (int i = 0; i < num_filled; i++) {
            printf("PID: %ld, Name: %s, State: %ld, UID: %ld, Voluntary CSW: %ld, Involuntary CSW: %ld, Children: %ld, Siblings: %ld\n",
                   info_array[i].pid, info_array[i].name, info_array[i].state, info_array[i].uid,
                   info_array[i].nvcsw, info_array[i].nivcsw, info_array[i].num_children, info_array[i].num_siblings);
        }
    } else {
        perror("Syscall failed");
        printf("Error code: %ld\n", result);
    }
}

static void do_syscall_failing(struct process_info *info_array, long size, long *num_filled, long ret_code)
{
    long result = syscall(_PROCESS_ANCESTORS_, info_array, size, num_filled);
    assert(result == ret_code);
    if (result != ret_code) {
        printf("Test failed: expected %ld, got %ld\n", ret_code, result);
    } else {
        printf("Test passed for error code %ld\n", ret_code);
    }
}

