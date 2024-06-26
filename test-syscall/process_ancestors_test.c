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
static void test_internal(_Bool success, int lineNum, char* argStr);
static void test_print_summary(void);

/***********************************************************
 * main()
 ***********************************************************/
int main(int argc, char *argv[])
{
    test_normal_conditions();
    test_edge_cases();
    test_invalid_arguments();

    test_print_summary();

    return 0;
}


/***********************************************************
 * Custom testing framework
 ***********************************************************/
// Track results:
static int numTests = 0;
static int numTestPassed = 0;

static int current_syscall_test_num = 0;
static int last_syscall_test_num_failed = -1;
static int num_syscall_tests_failed = 0;

// Macro to allow us to get the line number, and argument's text:
#define TEST(arg) test_internal((arg), __LINE__, #arg)

// Actual function used to check success/failure:
static void test_internal(_Bool success, int lineNum, char* argStr)
{
	numTests++;
	if (!success) {
		if (current_syscall_test_num != last_syscall_test_num_failed) {
			last_syscall_test_num_failed = current_syscall_test_num;
			num_syscall_tests_failed++;
		}
		printf("-------> ERROR %4d: test on line %d failed: %s\n",
				numTestPassed, lineNum, argStr);
	} else {
		numTestPassed++;
	}
}

static void test_print_summary(void)
{
	printf("\nExecution finished.\n");
	printf("%4d/%d tests passed.\n", numTestPassed, numTests);
	printf("%4d/%d tests FAILED.\n", numTests - numTestPassed, numTests);
	printf("%4d/%d unique sys-call testing configurations FAILED.\n", num_syscall_tests_failed, current_syscall_test_num);
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
    current_syscall_test_num++;
    long num_filled;
    printf("\nAttempting syscall with size %ld\n", size);
    int result = syscall(_PROCESS_ANCESTORS_, info_array, size, &num_filled);
    TEST(result == 0);
    // if (result == 0) {+
    //     printf("Syscall successful. Number filled: %ld\n", num_filled);
    //     for (int i = 0; i < num_filled; i++) {
    //         printf("PID: %ld, Name: %s, State: %ld, UID: %ld, Voluntary CSW: %ld, Involuntary CSW: %ld, Children: %ld, Siblings: %ld\n",
    //                info_array[i].pid, info_array[i].name, info_array[i].state, info_array[i].uid,
    //                info_array[i].nvcsw, info_array[i].nivcsw, info_array[i].num_children, info_array[i].num_siblings);
    //     }
    // } else {
    //     perror("Syscall failed");
    //     printf("Error code: %ld\n", result);
    // }
}

static void do_syscall_failing(struct process_info *info_array, long size, long *num_filled, long ret_code)
{
    current_syscall_test_num++;
    errno = 0;  // Reset errno before the syscall
    int result = syscall(_PROCESS_ANCESTORS_, info_array, size, num_filled);
    int err = errno;  // Capture errno after the syscall

    // Check if result is -1 and the expected error code is set in errno
    TEST(result == -1 && err == ret_code);

    if (result != -1 || err != ret_code) {
        printf("Test failed: expected error code %ld, got result %d, errno = errno %d\n", ret_code, result, err);
    } else {
        printf("Test passed for error code %ld\n", ret_code);
    }
}


