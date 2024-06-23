# File Descriptions 

### cmpt300 

- ``` ~/cmpt300/linux-5.4.109/cmpt300 ```

- Defines Systemcalls 

### test-syscall

- ``` ~/cmpt300/test-syscall ```

- Defines test applications 

### Makefile

- ``` ~/cmpt300/linux-5.4.109 ```

- Compiles our Systemcall folder in line 1039

### syscall_64.tbl

- ``` ~/cmpt300/linux-5.4.100/arch/x86/entry/syscalls/syscall_64.tbl ```

- Adds our Systemcall in line 437

<br/>

# Build Guide 

Add all files to corresponding locations

Rebuild kernel (systemcall): ```~/cmpt300/linux-5.4.100/make -j4```

Compile test applications: ```gcc -std=c99 -D _GNU_SOURCE -static array_stats_test.c -o array_stats_test```

Boot QEMU and transfer test executable: ```scp -P 10022 array_stats_test ubuntu@localhost:```

Execute test applications in QEMU: ```./array_stats_test```