#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include "array_stats.h"

SYSCALL_DEFINE3(array_stats, struct array_stats __user *, stats, long __user *, data, long, size) {
    struct array_stats kernel_stats;
    long val;
    int i;

    if (size <= 0)
        return -EINVAL;

    if (!access_ok(stats, sizeof(struct array_stats)) || !access_ok(data, size * sizeof(long)))
        return -EFAULT;

    // Perform the first data read:
    if (copy_from_user(&val, &data[0], sizeof(long)))
        return -EFAULT;

    kernel_stats.min = kernel_stats.max = kernel_stats.sum = val;

    for (i = 1; i < size; i++) {
        if (copy_from_user(&val, &data[i], sizeof(long)))
            return -EFAULT;

        if (val < kernel_stats.min)
            kernel_stats.min = val;
        if (val > kernel_stats.max)
            kernel_stats.max = val;
        kernel_stats.sum += val;
    }

    // Copy entire struct at once:
    if (copy_to_user(stats, &kernel_stats, sizeof(struct array_stats)))
        return -EFAULT;

    return 0;
}
