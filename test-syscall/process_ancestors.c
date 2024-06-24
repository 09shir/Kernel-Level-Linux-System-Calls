#include "process_ancestors.h"
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/sched.h>

SYSCALL_DEFINE3(process_ancestors, struct process_info __USER *, info_array, long, size, long __USER *, num_filled)
{
	struct process_info kernel_info;	
	struct task_struct *task;
	long count = 0;

	if (size <= 0) return -EINVAL;
	if (!info_array || !num_filled) return -EFAULT;

	task = current;

	while (task->pid != 0 && count < size) {
		kernel_info.pid = task->pid;
		strncpy(kernel_info.name, task->comm, ANCESTOR_NAME_LEN);
		kernel_info.state = task->state;
		kernel_info.uid = task->cred->uid.val;
		kernel_info.nvcsw = task->nvcsw;
		kernel_info.nivcsw = task->nivcsw;
		kernel_info.num_children = list_length(&task->children);
		kernel_info.num_siblings = list_length(&task->sibling) - 1;

		if (copy_to_user(&info_array[count], &kernel_info, sizeof(struct process_info)))
			return -EFAULT;

		task = task->parent;
		count++;
	}

	if (copy_to_user(num_filled, &count, sizeof(long)))
		return -EFAULT;

	return 0;
}
