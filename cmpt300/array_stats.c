#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include "array_stats.h"

SYSCALL_DEFINE3(array_stats, struct array_stats __user *, stats, long __user *, data, long, size) {
    long min, max, sum, val;
    int i;

    if (size <= 0)
        return -EINVAL;

    if (!access_ok(stats, sizeof(struct array_stats)) || !access_ok(data, size * sizeof(long)))
        return -EFAULT;

    if (copy_from_user(&val, &data[0], sizeof(long)))
        return -EFAULT;

    min = max = sum = val;

    for (i = 1; i < size; i++) {
        if (copy_from_user(&val, &data[i], sizeof(long)))
            return -EFAULT;

        if (val < min)
            min = val;
        if (val > max)
            max = val;
        sum += val;
    }

    if (copy_to_user(&stats->min, &min, sizeof(long)) ||
        copy_to_user(&stats->max, &max, sizeof(long)) ||
        copy_to_user(&stats->sum, &sum, sizeof(long)))
        return -EFAULT;

    return 0;
}
