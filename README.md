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

- ``` ~/cmpt300/linux-5.4.109/arch/x86/entry/syscalls/syscall_64.tbl ```

- Adds our Systemcall in line 437

<br/>

# Build Guide 

Replace all files to corresponding locations:

> Download ZIP into ```~/Downloads``` &#8594; Extract
```bash
cp -r ~/Downloads/CMPT-300-Assignment-2-master/cmpt300 ~/cmpt300/linux-5.4.109/

cp -r ~/Downloads/CMPT-300-Assignment-2-master/test-syscall ~/cmpt300/

cp ~/Downloads/CMPT-300-Assignment-2-master/Makefile ~/cmpt300/linux-5.4.109

cp ~/Downloads/CMPT-300-Assignment-2-master/syscall_64.tbl ~/cmpt300/linux-5.4.109/arch/x86/entry/syscalls/syscall_64.tbl
```

Rebuild kernel (systemcall): 

```bash 
cd ~/cmpt300/linux-5.4.109

make -j4
```

Compile test applications: 

```bash
cd ~/cmpt300/test-syscall 

gcc -std=c99 -D _GNU_SOURCE -static array_stats_test.c -o array_stats_test 

gcc -std=c99 -D _GNU_SOURCE -static process_ancestors_test.c -o process_ancestors_test
```

Boot QEMU and transfer test executable: 

```bash
cd ~/cmpt300/

qemu-system-x86_64 -m 3G \
-drive file=./ubuntu-20.04-server-cloudimg-amd64.qcow2,format=qcow2 \
-smp 4 \
-nic user,hostfwd=tcp::10022-:22 \
-kernel ./linux-5.4.109/arch/x86/boot/bzImage \
-nographic \
-append "root=/dev/sda1 console=ttyS0,115200n8 console=tty0"

```
Open a new terminal to transfer file: 

```bash
cd ~/cmpt300/test-syscall 

scp -P 10022 array_stats_test process_ancestors_test ubuntu@localhost:
```


Execute test applications in QEMU: 

```bash
./array_stats_test

./process_ancestors_test
```
