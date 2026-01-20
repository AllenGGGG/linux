按应用划分：内核空间、用户空间
# 内核
- 进程调度、内存管理、虚拟文件系统、网络接口、进程间通信
## 进程调度SCHED
- SCHED_OTHER：分时调度策略，vruntime+红黑树，每次选择左侧节点。针对普通进程的时间片轮转调度策略
- SCHED_FIFO：实时调度策略，针对运行的实时性要求比较高、运行时间短的进程调度策略
- SCHED_RR：实时调度策略，针对实时性要求高，运行时间比较长的进程调度策略
## 内存管理MMU
- 多进程间的内存共享策略，linux中的内存管理主要说的是虚拟内存
- 虚拟内存可以让进程拥有比实际物理内存更大的内存，可以是实际内存的多倍
- 每个进程的虚拟内存有不同的地址空间，多个进程的虚拟内存不会冲突
虚拟地址空间 = 内核区 + 用户区
## 虚拟文件系统VFS
- 最常用的ext2和ext3
## 网络接口
网络接口 = 网络协议 + 驱动程序
## 进程间通信
- 通信方式：管道、信号、消息队列、共享内存和套接字（本地/网络）等
# 文件
## 文件目录
- dev、media、mnt：硬件设备管理，挂载外设
- bin：可执行的二进制文件，mkdir等
- sbin：系统级执行的
- home：普通用户目录
- lib/lib64：要加载库的目录
- proc/run：操作系统使用的目录
- tmp：临时目录，重启后自动删除
- usr：存放系统范围内共享的只读数据和程序
- boot：开机启动项
- etc：存储配置文件目录
- opt：用来安装第三方软件
- var：系统中经常用到并变化的文件，如日志
## 文件类型
- -：普通文件				白、红、绿
- d：目录（directory）			蓝色
- l：软链接文件（link）快捷方式		青色
- c：字符设备（char）,键盘		黄色
- b：块设备（block），硬盘		黄色
- p：管道文件（pipe），进程间通信	棕黄色
- s：本地套接字（socket）		粉色
## 用户类型
- 文件所有者、文件所属组用户（创建用户的时候自动创建）、其他人
## 用户权限
- 读权限：`r` read
- 写权限：`w` write
- 执行权限：`x` excute 没有执行权限则没法进入目录
- 没有任何权限：`-`
drwxrwxr-x  6 allen allen 4.0K Oct 11 21:35 PyBullet/
- d 文件类型
- rwx 文件所有者权限
- rwx 用户组权限
- r-x 其他人权限
- 6 记录有多少的别名，硬连接计数
- allen 文件所有人
- allen 文件所属组
- 4.0K 如果这个文件是目录，仅表示这个目录占的磁盘空间大小，不包括里面文件大小
- Oct 11 21:35 文件修日期

# 指令
- ctrl + e：移动光标到文字最后
- ctrl + a：字首
- history：历史指令
- ctrl + p：上一条
- ctrl + n：下一条
- ctrl + k：删除后面所有
- ctrl + u：删除前面所有
## cd
- cd -：快速切换两个邻近的目录
## ls
- ls -l：详细显示
- ls -lh：文件大小显示成字节大小格式 human
- ls -F：显示是否为文件
- ll： = ls -lFa
- ls -a：显示所有文件
## rm
- rm -ri：有提示的递归删除
## less/more/cat 查看文件
## 链接
软链接和硬链接
### 1 软链接
- ln  -s  /绝对路径/源文件夹/源文件   /目标路径/链接名

  - ```bash
    lrwxrwxrwx 1 allen allen    8 Jan 15 22:00 linux.lnk -> linux.md
    ```

    创建的时候源文件路径最好是绝对路径，因为这样当源文件更改位置的时候，还能继续使用。

  - ```bash
    lrwxrwxrwx 1 allen allen 36 Jan 15 22:06 aa.ln -> /home/allen/Documents/notes/linux.md
    ```

    移动位置也不会发生改变。
### 2 硬链接
- ln  源文件  /目标路径/链接名

### 3 深入解析

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -lFh
total 36K
#权限	 	计数	所有者	组	大小		日期		文件名
lrwxrwxrwx 1 allen allen   36 Jan 15 22:06 aa.ln -> /home/allen/Documents/notes/linux.md	# 软链接
-rw-rw-r-- 2 allen allen 3.6K Jan 15 22:30 bb.ln			# 硬链接
-rw-rw-r-- 1 allen allen 4.7K Oct 10 15:14 isaaclab.md
-rw-rw-r-- 2 allen allen 3.6K Jan 15 22:30 linux.md
-rw-rw-r-- 1 allen allen 2.5K Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen 5.0K Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen 5.2K Oct 23 19:43 vision.md
```

#### a. 链接存储内容

- 软链接会占用物理内存空间（很小），它存储的是一个**路径字符串**，不需要额外的数据块（Data Blocks），访问软链接时，内核会把它读入内存，然后跳转到真正的目标。

  软链接不会增加引用计数（记录多少个文件名/路径指向同一个`Incode`编号）。

- 硬链接是给磁盘上同一个文件起了一个绰号，在`linux`的`ext4`文件系统的底层，一个文件不是由文件名定一个，而是由`Incode`(索引节点)定义的。硬链接实质就是让多个文件名同时指向同一个`Inode`。

  所以创建硬链接时，系统不会复制文件的内容，也不会占用额外的磁盘空间，只是在目录表中附加了一个记录，指向已经存在的`Inode`编号。

**核心**：删除源文件后，软链接不能使用，因为路径已经没了，变成了死链接。而硬链接可以，删除文件的核心其实是删除一个指向该数据块的引用，计数会减1,但是仍然指向的是同一个硬盘空间。

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -i 
 7864355 aa.ln  25296907 isaaclab.md  25298467 python_func.md  25298528 vision.md
25324385 bb.ln  25324385 linux.md     25314521 tulun.md
```

#### b. Inode

- 用`ls -i`可以查看文件的Inode

- 如果硬盘是一个巨大的仓库：

  **数据块 (Data Blocks)**：是仓库里存放货物的**空间**。

  **Inode (Index Node)**：是仓库的**管理卡片**，记录了货物放在哪、谁能动、什么时候放的。

  **Inode 编号**：就是这张卡片的**索引号**。

- 每一个文件（或目录）都对应一个 `Inode`，它存储了除了“文件名”以外的所有**元数据（Metadata）**：

  - **文件的权限**（`rwx`）。
  - **文件所有者 (UID)** 和 **所属组 (GID)**。
  - **文件大小**。
  - **时间戳**（创建时间、最后修改时间、最后读取时间）。
  - **连接数**（有多少个文件名指向这个 Inode，即我们之前聊的硬链接数）。
  - **数据块指针**：这是最重要的部分，它记录了文件真正的“内容”保存在硬盘的哪些位置。

  **注意**：文件名是不存放在 `Inode` 里的，它存放在**目录文件**中。目录就像一张简单的表格：`文件名 <---> Inode 编号`。

- 软链接与硬链接的`Inode`不同，所以占用的磁盘空间不同，存储内容不同。硬链接与源文件的`Inode`是相同的，所以引用计数要加1,也就是等价于硬链接计数

```bash
allen@allen-Dell-G15-5530:~/Documents$ ls -i notes
 7864355 aa.ln  25296907 isaaclab.md  25298467 python_func.md  25298528 vision.md
25324385 bb.ln  25324385 linux.md     25314521 tulun.md
allen@allen-Dell-G15-5530:~/Documents$ ls -id notes
25306615 notes
```

#### c. ls -i  / ls -id

- ls后面+文件夹：默认的是你想查看这个文件夹里面的内容
  - `ls -i notes`：等价于查看`notes/`里面文件的`Inode`
  - `ls -id notes`：d—dictionary，代表查看这个文件夹的`Inode`

#### d. 有趣场景

##### 1. 为什么mv移动文件很快？

- 如果在**同一个分区**内移动一个10G的视频文件，Linux会瞬间完成，因为mv只是在目录表里改了个名字，对应的**Inode编号**和**底层数据块**完全没动。

##### 2. 硬盘明明有空间，为什么提示“硬盘已满”？

- 因为硬盘在格式化的时候，Inode的总数是固定的。如果产生了几百万个只有1KB的垃圾小文件，虽然数据块还没有用完，但Inode编号用光了

#### e. 硬链接使用场景

##### 1. 情景一：防止意外删除（数据保险）

- 在linux中，只有当指向某个Icode的所有文件名都被删除，文件的数据才会被真正的抹去。

  - 如果你为重要的代码 `main.cpp` 创建了一个硬链接 `main.cpp.bak`。

    哪怕你不小心执行了 `rm main.cpp`，你的数据依然安全地存在于 `main.cpp.bak` 中。

##### 2. 情景二：节省空间的多地存放

假设有一个1GB的仿真数据集，希望在`project1/`和`project2/`中都能访问它

- 使用`cp`：占用2GB空间
- 使用硬链接：只占用1GB空间，但在两个文件夹下看起来都像是一个完整独立的文件。

##### 3. 情景三：文件重命名/移动的平滑过渡

- 如果想更改一个库的名字，但又怕某些老程序找不到旧名字，创建一个硬链接。这样新旧名字都指向同一份数据，任何一方的修改都会**实时同步**。

  同时如果不希望同时修改，互不干扰只能用`cp`

#### f. 不同分区

```bash
allen@allen-Dell-G15-5530:~/Documents$ df -T
Filesystem     Type     1K-blocks      Used Available Use% Mounted on
tmpfs          tmpfs      3253404      2824   3250580   1% /run
/dev/nvme0n1p2 ext4     959786032 358055316 552902508  40% /
tmpfs          tmpfs     16267004     53788  16213216   1% /dev/shm
tmpfs          tmpfs         5120         4      5116   1% /run/lock
efivarfs       efivarfs       438       322       112  75% /sys/firmware/efi/efivars
/dev/nvme0n1p1 vfat        523248     78328    444920  15% /boot/efi
tmpfs          tmpfs      3253400       172   3253228   1% /run/user/1000

allen@allen-Dell-G15-5530:~/Documents$ lsblk
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
loop0         7:0    0     4K  1 loop /snap/bare/5
loop1         7:1    0 330.3M  1 loop /snap/code/215
loop2         7:2    0 332.1M  1 loop /snap/code/217
loop3         7:3    0  63.8M  1 loop /snap/core20/2682
loop4         7:4    0  63.8M  1 loop /snap/core20/2686
loop5         7:5    0    74M  1 loop /snap/core22/2163
loop6         7:6    0    74M  1 loop /snap/core22/2193
loop7         7:7    0 250.4M  1 loop /snap/firefox/7477
loop8         7:8    0 250.8M  1 loop /snap/firefox/7559
loop9         7:9    0 516.2M  1 loop /snap/gnome-42-2204/226
loop10        7:10   0 531.4M  1 loop /snap/gnome-42-2204/247
loop11        7:11   0  91.7M  1 loop /snap/gtk-common-themes/1535
loop12        7:12   0  12.2M  1 loop /snap/snap-store/1216
loop13        7:13   0  12.3M  1 loop /snap/snap-store/959
loop14        7:14   0  50.9M  1 loop /snap/snapd/25577
loop15        7:15   0  48.1M  1 loop /snap/snapd/25935
loop16        7:16   0   576K  1 loop /snap/snapd-desktop-integration/315
loop17        7:17   0   452K  1 loop /snap/snapd-desktop-integration/83
nvme0n1     259:0    0 931.5G  0 disk 
├─nvme0n1p1 259:1    0   512M  0 part /boot/efi
└─nvme0n1p2 259:2    0   931G  0 part /var/snap/firefox/common/host-hunspell
                                      /
nvme1n1     259:3    0 953.9G  0 disk 
├─nvme1n1p1 259:4    0   300M  0 part 
├─nvme1n1p2 259:5    0   128M  0 part 
├─nvme1n1p3 259:6    0 200.1G  0 part 
├─nvme1n1p4 259:7    0   729G  0 part 
├─nvme1n1p5 259:8    0  1009M  0 part 
├─nvme1n1p6 259:9    0  21.7G  0 part 
└─nvme1n1p7 259:10   0   1.6G  0 part 
```

- Filesystem就是代表着不同分区，跨区操作也只能用软链接。/dev/nvme0n1p2和/dev/nvme0n1p1就是同一个文件的两个区，tmpfs和其又是两个不同的文件系统。
- tmpfs：Temporary File System临时文件系统，不占硬盘，直接在RAM里面。/dev/shm内存共享区

#### g. 硬链接计数

- 把硬链接计数想象成一个文件的“生命值”：
  - **创建文件时**：默认计数为 **1**。
  - **创建硬链接 (`ln`)**：每增加一个硬链接，计数 **+1**。
  - **删除文件 (`rm`)**：当你删除一个文件名时，计数 **-1**。
  - **真正删除数据**：只有当计数归 **0** 时，内核才会认为这个文件已经彻底没人用了，随后释放 Inode 编号并把磁盘上的数据块标记为“空闲”
- 实际作用：
  - **防止误删**：可以给重要的系统库或配置文件创建多个硬链接散落在不同目录。即使你不小心删除了其中一个，只要计数不为 0，数据就依然安全。
  - **原子更新**：在很多底层软件更新时，会先创建一个新文件的硬链接，等一切就绪后再切断旧链接。这样可以确保在更新过程中，系统始终能访问到完整的文件。
  - **磁盘清理诊断**：如果你发现硬盘满了，但 `du` 命令查不到大文件，有时候是因为某些进程还抓着一个已经 `rm` 掉（计数为 0 但进程未关闭）的文件不放。

#### h. 不允许给文件夹创建硬链接

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ stat aa.ln		# 软链接
  File: aa.ln -> /home/allen/Documents/notes/linux.md
  Size: 36        	Blocks: 0          IO Block: 4096   symbolic link
Device: 10302h/66306d	Inode: 7864355     Links: 1
Access: (0777/lrwxrwxrwx)  Uid: ( 1000/   allen)   Gid: ( 1000/   allen)
Access: 2026-01-15 22:17:04.699752243 +0800		#访问
Modify: 2026-01-15 22:06:25.328208454 +0800
Change: 2026-01-15 22:17:01.311953142 +0800
 Birth: 2026-01-15 22:06:25.328208454 +0800
 
allen@allen-Dell-G15-5530:~/Documents/notes$ stat -L aa.ln  # -L 表示穿越链接，到源文件
  File: aa.ln
  Size: 14185     	Blocks: 32         IO Block: 4096   regular file
Device: 10302h/66306d	Inode: 25324385    Links: 2
Access: (0664/-rw-rw-r--)  Uid: ( 1000/   allen)   Gid: ( 1000/   allen)
Access: 2026-01-16 09:58:00.906601876 +0800
Modify: 2026-01-16 09:58:00.906601876 +0800
Change: 2026-01-16 09:58:00.906601876 +0800
 Birth: 2026-01-15 22:05:37.120236654 +0800
```

- stat()：Status，跟随（Dereference、解引用）：如果路径是一个软链接，他会顺着链接找到最终的目标文件，并返回文件信息。

  lstat():Link Status，不跟随（No-dereference）：软链接则直接返回该链接文件本身的信息（文件大小、自身的Inode编号）

- 在ext4文件系统中，Inode 结构体虽然很小（通常 256 字节）大约预留了60字节空间存储指针，Data Block（数据块）最小规格是4KB，文件大小36<60B的时使用Fast Symlink快速/内联链接，存入Inode中，不占Block，因此Blocks = 0；

- 软链接权限是虚的，固定显示为0777，`/l`表示为Link，symbolic link为Inode标记。

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ stat bb.ln 
  File: bb.ln
  Size: 14185     	Blocks: 32         IO Block: 4096   regular file
Device: 10302h/66306d	Inode: 25324385    Links: 2
Access: (0664/-rw-rw-r--)  Uid: ( 1000/   allen)   Gid: ( 1000/   allen)
Access: 2026-01-16 09:58:00.906601876 +0800
Modify: 2026-01-16 09:58:00.906601876 +0800
Change: 2026-01-16 09:58:00.906601876 +0800
 Birth: 2026-01-15 22:05:37.120236654 +0800
```

- stat() 与 lstat()：stat默认调用`lstat()`函数看来链接本身，stat -L查看链接指向的文件对应`stat()`函数
- 软链接的身份标签：
  - 在文件的结构体中，有一个字段叫`st_mode`。对于软链接这个字段标记为`S_IFLNK`

- 链接文件夹的时候只能用软链接，内核级禁止硬链接文件夹。

##### 情景一：无限循环

- 当目录A包含指向目录B的硬链接，而目录B又包含指向目录A的硬链接，就形成了一个环路

  当ls -R、find、du尝试递归扫描文件夹时，使用的是lstate()函数，硬链接文件与普通文件都是相同的标记`regular file`，就会陷入了死循环。进入 A -> 发现 B -> 进入 B -> 发现 A -> 进入 A... 永无止境

  而当时用软链接给目录创建链接时，尽管形成环，但是发现标记为`symbolic link`，主动停止

##### 情景二： 父目录定位问题

- 每个目录下有两个隐藏目录：`.`指向当前目录，`..`指向父目录。

  如果目录`Dir_C`有两个硬链接，分别位于`Dir_A`和`Dir_B`下面，当在`Dir_C`下面`cd ..`的时候，返回A还是B？

##### 情景三：文件系统自检（fsck）噩梦

- 非正常关机，重启后执行文件自检的时，系统需要检查每个目录的链接数和完整性。

  如果允许硬链接，文件系统不再是“树”，而变成一个复杂的“图”

  计算目录的引用计数也会复杂且易出错，极小逻辑错误可能导致整个硬盘的分区表损坏。

# 修改权限

## chmod修改权限

### 权限所有者

- 文件所有者
- 文件所属组用户
- 其他人

## 语法格式

- ```bash
  chmod who(u/g/o/a)+/-/= mod(r/w/x/-) 文件名
  ```

### 1 . 文字设定法

#### a. who：

- `u`：user -> 文件所有者
- `g`：group -> 文件所属组用户
- `o`：other -> 其他
- `a`：all，以上是三类人 u + g + o

#### b. +/-/=

- `+`：添加权限
- `-`：去除权限
- `=`：权限的覆盖

#### c. mod：

- `r`：read，读
- `w`：write，写
- `x`：execute，执行
- `-`：没有权限

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw--w---- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw--w---- 1 allen allen 16933 Jan 19 12:08 linux.md
-rw--w---- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw--w---- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw--w---- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod go+r *
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-r-- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw-rw-r-- 1 allen allen 16933 Jan 19 12:08 linux.md
-rw-rw-r-- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod go=wr *
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-rw- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw-rw-rw- 1 allen allen 17695 Jan 19 12:14 linux.md
-rw-rw-rw- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-rw- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-rw- 1 allen allen  5259 Oct 23 19:43 vision.md
```

who可以同时使用多个

### 2. 数字设定法

#### a. mod:

- 4 ：read，r
- 2： write，w
- 1： execute，x
- 0：没有权限
- 5： r+x
- 6： r+w
- 7： r+w+x

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-r-- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw-rw-r-- 1 allen allen 18211 Jan 19 12:27 linux.md
-rw-rw-r-- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod -064 *
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw------- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw------- 1 allen allen 18211 Jan 19 12:27 linux.md
-rw------- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw------- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw------- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod +064 *
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-r-- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw-rw-r-- 1 allen allen 18211 Jan 19 12:27 linux.md
-rw-rw-r-- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod 604 linux.md
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-r-- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw----r-- 1 allen allen 19348 Jan 19 12:30 linux.md
-rw-rw-r-- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod 664 linux.md
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-r-- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw-rw-r-- 1 allen allen 19348 Jan 19 12:30 linux.md
-rw-rw-r-- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen  5259 Oct 23 19:43 vision.md
allen@allen-Dell-G15-5530:~/Documents/notes$ chmod +60 linux.md
allen@allen-Dell-G15-5530:~/Documents/notes$ ls -l
total 48
-rw-rw-r-- 1 allen allen  4798 Oct 10 15:14 isaaclab.md
-rw-rw-r-- 1 allen allen 19348 Jan 19 12:30 linux.md
-rw-rw-r-- 1 allen allen  2553 Oct 24 10:18 python_func.md
-rw-rw-r-- 1 allen allen  5029 Jan 13 23:08 tulun.md
-rw-rw-r-- 1 allen allen  5259 Oct 23 19:43 vision.md
```

`+/-0`的时候不改变权限，当指定为0的时候改变

数字设定法和文字设定法的数字和字母不能混合使用

## 修改文件所有者chown

- ```bash
  sudo chown 新所有者 文件名
  sudo chown 新所有者：所属组 文件名
  ```

  跨用户的操作都需要sudo

## 修改文件所属组chgrp

- ```bash
  sudo chgrp 新所属组 文件名
  ```

# 其他命令

## 重定向命令

### 1. 输出重定向

修改输出数据的位置，`>`或`>>`

#### a. >与>>的区别

- `>`：将文件内容写入到**指定文件中**，如果文件中已有数据，则会使用新数据**覆盖**原数据
- `>>`：将输出的内容**追加**到指定的文件**尾部**。

```bash
allen@allen-Dell-G15-5530:~/Documents/notes$ echo PATH: > tmp
allen@allen-Dell-G15-5530:~/Documents/notes$ echo ${PATH} >> tmp
allen@allen-Dell-G15-5530:~/Documents/notes$ cat tmp 
PATH:
/home/allen/.nvm/versions/node/v22.20.0/bin:/opt/ros/humble/bin:/home/allen/miniconda3/condabin:/home/allen/bin:/home/allen/onos/tools/package/runtime/bin:/usr/lib/jvm/java-11-openjdk-amd64/bin:/home/allen/.local/bin:/home/allen/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin:/home/allen/onos/tools/dev/bin:/home/allen/onos/tools/test/bin:/home/allen/onos/tools/test/scenarios/bin:/home/allen/onos/tools/build:/home/allen/Applications/apache-maven-3.3.9/bin
```



# 多用户

## 用户

### 1. 切换用户

- ```bash
  su 用户名
  exit 	# 退出
  ```

  只切换用户，不切换工作目录

- ```bash
  su - 用户名
  ```

  工作目录同步切换，都是位于同一个路径下面，但是前面的用户名发生了改变

### 2. 添加删除用户

- ```bash
  sudo adduser 用户名	# 只允许使用小写用户名
  ```

  - `vim /etc/passwd`可以查看是否创建成功

  ```bash
  vim /etc/passwd
  fff:x:1001:1001:,,,:/home/fff:/bin/bash
  #用户名：密码：用户id：用户组id：，，，：当前用户的组目录：使用的命令解析器
  ```

- ```bash
  sudo userdel 用户名 -r	# 删除用户的家目录
  ```

  - 如果没有使用-r，则需要使用`rm 用户名/home -r`

  - ```bash
    which userdel
    /usr/sbin/userdel
    ```

### 3. 添加和删除用户组

- ```bash
  sudo groupadd 组名	# 添加
  sudo groupdel 组名	# 删除
  ```

  - 可以用切换组来判断是否创建成功

  ```bash
  sudo chgrp 组名 文件名
  ```

  - 可以用vim查看是否存在

  ```bash
  vim /etc/group
  ```

### 4. 修改密码

- ```bash
  passwd	# 修改当前用户
  passwd 用户	# 修改其他用户密码，没有权限则使用sudo
  sudo paaswd root	# 有sudo权限即可修改root用户
  ```

  因此创建用户尽可能不要授予root权限

# 压缩与解压缩

常见压缩格式：

- `tar.gz`和`.tgz`，完全相同
- `.tar.bz2`
- `.zip`
- `.tar.xz`
- `.rar`

## tar

- tar只能进行打包，原始的`gzip`与`bzip2`只能进行压缩（只能生成单个压缩文件），甚至有可能把源文件压缩

### 1. 压缩（.tar.gz  .tar.bz2  .tgz）

```bash
tar 参数 生成的压缩包名字 要压缩的文件（文件或目录）
```

#### a. 相关参数

- `c`：创建压缩文件
- `z`：使用`gzip`方式进行文件压缩
- `j`：使用`bzip2`方式压缩
- `v`：压缩过程中显示压缩日志，可以省略
- `f`：指定压缩包的名字

压缩一定要写后缀明确使用的方法，方便解压`.tar.gz`和`.tar.bz2`

### 2.解压缩

```bash
tar 参数 压缩包名		# 当前目录
tar 参数 压缩包名 -C 解压目录		# 注意是大写C
```

#### a.相关参数

- `x`：释放压缩文件内容—eXtract
- `z`：使用`gzip`的方式
- `j`：使用`bzip2`方式
- `v`：解压缩过程中显示压缩信息
- `f`：指定压缩包名字

## zip/uzip

- tar会默认进行递归压缩，zip需要指定-r参数
- zip压缩后会自动产生.zip后缀，tar需要指定后缀.tar.gz等

### 1. zip

- ```bash
  zip -r 压缩包名 要压缩的文件
  ```

  `-r`：递归压缩

### 2. uzip

- ```bash
  uzip 压缩包名				   # 解压到当前目录
  unzip 压缩包名 -d 解压目录		# 指定目录
  ```

## rar/unrar

#### 1. rar

- ```bash
  rar a 压缩包名 要压缩的文件 -r
  ```

  - archive：归档
  - 如果要压缩的文件是目录，则需要加`-r`

#### 2. rar/unrar

- ```bash
  rar/unrar x 压缩包名字 			# 当前目录
  rar/unrar x 压缩包名字 目标目录	 # 指定目录 
  ```

## xz

#### 1. 打包 + 压缩

- ```bash
  tar cvf xx.tar 要压缩的文件		# 打包文件
  xz -z xx.tar 					# 生成.tar.xz文件
  ```

#### 2. 解压缩 + 解包

- ```bash
  xz -d xx.tar.xz
  tar xvf xx.tar
  ```


# 搜索

## find

基于文件的属性进行搜索，但是不能基于内容（grep）

### 1. 基于文件名和类型

#### a.基于文件名

- ```bash
  find 搜索的路径 -name 要搜索的文件名
  ```

​	路径默认是递归搜索的

- ```bash
  find ./ -name "*.cpp"
  find ./ -name '*.cpp'
  ```

  使用通配符必须要搭配`''`或者`""`

#### b. 基于文件类型

7种文件类型：f-普通文件，d-目录，l-软链接，c-字符设备，b-块设备，p-管道，s-本地套接字类型

- ```bash
  find 搜索的路径 -type 文件类型
  ```

### 2. 基于文件大小

- ``` bash
  find 搜索的路径 -size [+/-]文件大小
  # 文件大小需要加单位 k、M、G, +/-表示范围，不加默认为-1的区间
  ```

```bash
find ./ -size +3M	# file > 3M
find ./ -size 3M	# 3-1M < file <= 3M
find ./ -size -3M	# 0M <= file <= 2M
find ./ -size +1M -size -4M		# 1M < file <= 3M
```

- `.`目录的大小是4k
- 核心是向上取整，所以都是左开又闭的。+就是大于数，-就是小于这个数

### 3. 基于目录层级

先指定层级参数，再指定其他参数。

- ```bash
  find ./ -maxdepth 3 -name 文件名	# 搜索层级最多三层
  find ./ -mindepth 3 -name 文件名	# 搜索层级最少从三层开始搜索
  ```

### 4. 同时执行多个操作

#### a. -exec

- ```bash
  find 路径 参数 参数值 -exec shell命令2 {} \;
  find 路径 参数 参数值 -exec shell命令2 {} ';'
  find 路径 参数 参数值 -exec shell命令2 {} ";"
  ```

  `-exec`是find的参数，添加其他需要被执行的shell命令，处理的是find的结果。

```bash
allen@allen-Dell-G15-5530:~/My_Tasks/Linux$ find ./ -maxdepth 1 -size +4k -exec ls -lFah {} \;
-rw-rw-r-- 1 allen allen 27K Jan 19 21:00 ./linux.md
```

- ```bash
  find ./ -name "*.md" -exec shell命令2 {} +
  ```

  - `{}`：表找到的文件名

  - `+`：表示一次性把所有文件名传给命令。不能对单个文件进行精细操作。类似于xargs

  - `;`：表示对找到的文件逐条处理，不管成功/失败

  - `\;`：如果没有`\`，shell会优先处理`;`，其真实身份是命令终止符。导致`-exec`缺少参数，所以要用转义字符`\`告诉系统，这个`;`是find的参数，不要处理。

    同理，也可以使用`';'`或者`";"`

#### b. -ok

- ```bash
  allen@allen-Dell-G15-5530:~/My_Tasks/Linux$ find ./ -maxdepth 1 -size +4k -ok ls -lFah {} \;	# 重点是\；
  < ls ... ./linux.md > ? y
  -rw-rw-r-- 1 allen allen 27K Jan 19 21:01 ./linux.md
  allen@allen-Dell-G15-5530:~/My_Tasks/Linux$ find ./ -maxdepth 1 -size +4k -ok ls -lFah {} \;
  < ls ... ./linux.md > ? n
  ```

  `exec`不会进行交互，`ok`参数会与用户进行交互，y(es)、n(o)、ctrl + c 退出

#### c. xargs

- ```bash
  find 路径 参数 参数值 | xargs shell命令2
  ```

  - `|`代表管道，把find的数据流传给后面的参数

## grep

用于查找文件里符合条件的字符串

### 1. 语法格式

- ```bash
  grep "搜索的内容" 搜索的路径/文件 参数
  grep 参数 "搜索的内容" 搜索的路径/文件 
  ```

#### a. 参数

- `-r`：搜索目录中的文件内容，进行递归操作
- `-i`：忽略大小写
- `-n`：标出搜索到的行号

注：grep和find后面跟的参数都是反着的。

#### b.正则表达式使用

```bash
allen@allen-Dell-G15-5530:~/My_Tasks/Cmake$ grep "INCLUDE <IOS.*>" ./ -rni
./cmake/demo/src/main.cpp:2:#include <iostream>
./cmake/multi/test2/sort.cpp:1:#include <iostream>
./cmake/multi/test1/calc.cpp:2:#include <iostream>
./cmake/v3/main.cpp:2:#include <iostream>
./cmake/v4/main.cpp:2:#include <iostream>
./cmake/v2/main.cpp:2:#include <iostream>
./cmake/v5/src/main.cpp:2:#include <iostream>
./cmake/multi_plus/test/sort.cpp:1:#include <iostream>
allen@allen-Dell-G15-5530:~/My_Tasks/Cmake$ grep -rni "INCLUDE <IOS.*>" ./
./cmake/demo/src/main.cpp:2:#include <iostream>
./cmake/multi/test2/sort.cpp:1:#include <iostream>
./cmake/multi/test1/calc.cpp:2:#include <iostream>
./cmake/v3/main.cpp:2:#include <iostream>
./cmake/v4/main.cpp:2:#include <iostream>
./cmake/v2/main.cpp:2:#include <iostream>
./cmake/v5/src/main.cpp:2:#include <iostream>
./cmake/multi_plus/test/sort.cpp:1:#include <iostream>
```

- 如果是使用通配符，使用`*`即可 ——shell命令使用

  如果是正则表达式Regex，则是`.*`——多为文本文件操作使用，精细操作

  `?`与`*`类似，但是只占用一个字符位置

  - ```bash
    ls a?.cpp  		# a1.cpp a2.cpp
    ls a??.cpp		# a12.cpp 
    ls a???.cpp		# a123.cpp
    ls a*.cpp		# a1.cpp a2.cpp a12.cpp a123.cpp
    ```

## locate

根据文件名搜索本地的磁盘文件，即搜索本地的数据库文件，其中包含本地所有文件信息。Linux自动创建且每天自动更新一次，所以locate查不到最新变动过的文件，为避免，需要在使用`updatedb`指令更新数据库

- ```bash
  sudo updatedb
  ```

### 1. 参数

#### a. 搜索所有目录下以某个关键字开头的文件

- ```bash
  locate 关键字	
  locate test
  ```

#### b. 指定路径，必须是绝对路径

- ```bash
  locate /home/allen/test
  ```

#### c. 其他参数

- ```bash
  locate TEST -I		# 不区分大小写
  ```

- ```bash
  locate test -n 5	# 列出前5条匹配的信息
  ```

- ```bash
  locate -r "\.cpp$"	# -r基于正则表达式进行匹配
  ```

  `$`表示行尾/结尾锚点，`\.cpp`表示以.cpp结尾的字符串，就是对应`*.cpp`的写法

```bash
find ./ -regex ".*\.cpp$"	# 寻找所有以 .cpp 结尾的文件
```

## Vim使用

## 命令模式下的操作

- ```vim
  gg=G	# 对代码排序
  0		# 移动到行首
  gg 		# 移动到文件头部
  G		# 移动到最后一行的行首
  ZZ		# 退出文件
  nG		# 到第n行行首，行跳转
  number	# 从光标当前位置向下移动number行
  删除的核心是剪切
  x		# 删除光标后面的字符
  X		# 删除光标前面的字符
  dw		# 删除单词
  d0		# 删除光标前的字符串
  d$/D	# 删除光标后面的字符串
  dd		# 删除光标所在行
  ndd		# 删除n行
  
  p		# 粘贴在光标所在行的后面
  P		# 粘贴在光标行的前面
  yy		# 复制光标所在行 
  nyy		# 复制光标下面n行
  
  u		# 撤销
  ctrl+r	# 反撤销
  
  r		# 替换光标后的单个字符
  R		# 替换多个字符，替换后ESC退出
  
  /关键字	 # 搜索，n向下，N向上
  ？关键字	# n向上，N向上
  #光标指向关键字	# 把光标放在关键字上，按下#
  
  vim -o 文件1 文件2 ...		# 水平分屏
  vim -O 文件1 文件2 ...		# 垂直分屏
  ```

## 可视模式

- ```vim
  v:进入字符可视模式，文本选择以字符为单位
  V：行可视模式
  ctrl+v：块可视模式，选择矩形文本
  
  d：删除（剪切）选中内容
  D：删除全部内容
  y：复制
  p：粘贴光标后面
  P：粘贴到光标前面
  ```

## 命令模式切换到编辑模式

- ```vim
  i		# 光标前输入
  a		# 光标后
  o		# 光标下新建一行
  I		# 当前行的行首输入
  A		# 当前行的行尾输入
  O		# 光标上边创建新行输入
  ```

## 末行模式

从命令模式输入`:`即可

- ```vim
  q!				# 不保存直接强制退出
  wq = x			# 保存退出
  
  替换
  s/被替换的关键字/新的关键字/		# 替换当前行的第一个关键字
  s/被替换的关键字/新的关键字/g		# 替换当前行所有关键字
  行号1,行号2s/被替换的关键字/新的关键字/g	# [行号1, 行号2]范围内替换
  %s/被替换的关键字/新的关键字/g		# 替换文本中所有关键字
  
  分屏
  sp				# 窗口垂直排列,水平分屏
  sp 文件名		  # 分屏同时指定打开的文件名字
  vsp				# 窗口水平排列，垂直分屏
  vsp 文件名
  ctrl + w + w    # 在打开的屏幕之间切换
  :qall			# 同时退出多个屏幕
  :wqall			# 保存并退出
  
  调整分屏分界线
  :set mouse=a	# 开启vim支持鼠标，通过鼠标拖动
  ctrl+w < 		# 减少宽度
  ctrl+w >		# 增加宽度
  ctrl+w =		# 宽度等分
  
  :行号			  # 直接跳转到指定的行号
  :!shell命令	  # 在末行模式执行shell命令
  ```

- 从末行模式回到命令模式，按两次`Esc`

  执行完指令默认回到命令模式

## 配置文件

- `~/.vimrc`用户级别，只对当前用户有效
- `etc/vim/vimrc`系统级别的配置文件，对所有Linux用户都有效
- 如果两个配置文件都设置了，用户级别的配置文件起作用

## 插件

  - 空格 + w - 保存文件                                                         
  - 空格 + q - 退出                                                             
  - 空格 + n - 打开/关闭文件树                                                  
  - 空格 + p - 模糊搜索文件                                                     
  - 空格 + g - 全局搜索内容                                                     
  - gcc - 注释/取消注释当前行                                                   
  - Ctrl + h/j/k/l - 分屏导航

### 应用1：代码注释

#### a. 添加注释

- ctrl+v，选中行，输入I（insert）并输入//，再按esc就可以完成

#### b. 删除注释

- ctrl + v，选中内容，输入d删除选中的内容

# gcc、g++

c语言使用gcc，c++语言使用g++

## 工作流程

### 1. 预处理、编译、汇编、链接

- 预处理（预编译）：gcc调用预处理器，展开头文件、宏替换、去掉注释行，最终得到的还是源文件，文本文件.i

  gcc参数：-E

  ```bash
  gcc -E test.c -o test.i
  ```

- 编译：调用编译器对文件进行编译，最终得到一个汇编文件.s

​	-S

```bash
gcc -S test.i -o test.s
```

- 汇编：汇编器，得到二进制文件.o

  -c

  ```bash
  gcc -c test.s -o test.o
  ```

- 链接：调用链接器对程序需要调用的库进行链接，得到一个可执行的二进制文件

  ```bash
  gcc test.o -o test
  ./test
  ```

一步操作完成

- ```bash
  gcc test.c -o aa
  gcc test.c			# 默认生成为a.out可执行程序
  ```

  虽然直接进行了第四步操作，但是前三步自动被完成了

### 相关参数

- ```bash
  -I directory		# 指定include包含文件的搜索目录，一般都是不在同一个文件夹的情况
  -D宏名			   # 程序编译的时候指定一个宏，程序可以使用
  -l 					# 指定使用的库的名字，有无空格都可以
  -L					# 指定编译时候，搜索的库的路径（相对/绝对路径都可以）
  ```
  
  ```bash
  gcc main.c -o calc -L ./ -lcalc			# 库的名字去掉lib和.a
  ```
  
  

### 区别

- 无论是gcc还是g++都可以编译C程序，规则和参数都是相同的
- g++可以直接编译C++程序，gcc编译C++程序需要链接`-lstdc++`
- 不管gcc还是g++都可以定义`__cplusplus`宏

# man

```bash
man man			# 查询指令
```

查询shell里面的某个命令

- ```bash
  man 1 cp
  ```

  1代表第一章节，cp相关的内容

```
section 1		Linux中提供的所有shell命令					用户命令`ls、pwd、cat`
section 2		系统函数（由内核提供）						  系统调用`read、write、fork`
section 3		库调函数（程序库中的函数）					 库函数`printf、mallac`
section 4		特殊文件（通常在/dev目录中可以找到）		   特殊文件`console、null`
```

- ```bash
  man 2 read
  ```

- ```bash
  man 3 printf
  ```


# 库

## 静态库

### 1. 制作静态库文件

#### a. 源文件汇编

- ```bash
  gcc 源文件（*.c） -c			# 得到.o文件
  ```

#### b. 将得到的.o文件打包，得到静态库

- ```bash
  ar rcs 静态库的名字（libxxx.a）	原材料（*.o）	# 后缀是固定的
  ```

### 2. 制作动态库文件

#### a. 制作

- ```bash
  gcc -c -fpic 源文件（*.c）		# 得到.o文件
  ```

- ```bash
  gcc -shared *.o -o lib名字.so		# 得到动态库文件
  ```

如果文件所有人、用户组、其他人对某个文件（比如动态库）都有执行的权限，那么在文件的后面就会有一个`*`，例如`libcalc.so*`

- 调用法是都是一样的，无论是静态还是动态

  ```bash
  gcc main.c -L . -lcalc -o app
  ```

  如果是相对路径，把最后的app文件移动位置，就加载不到动态库。如果是静态库，不会出现

#### b. 动态链接器`ld`—linker

查找顺序：

- 可执行文件内部的`DT_RPATH`段

- 系统的环境变量`LD_LIBRARY_PATH`

- 系统动态库的缓存文件`/etc/ld.so.cache`

  这个缓存文件不能直接修改，通过修改其对应的`ld.so.conf`文本文件才能把相应的内容同步过去

- 存储动态库/静态库的系统目录`/lib/`，`/user/lib`等

如果找不到库的位置，第一个没办法修改，只能通过修改后面三个查找

**ldd**： **List Dynamic Dependencies**（列出动态依赖关系）

- ```bash
  ldd 可执行文件名字
  ```

  如果能加载到，后面每个动态库都会有相应的地址，如果没有则显示not found

#### c. 解决方案

##### 方案一：拼接

字符串拼接使用   **`:`**

- ```bash
  LD_LIBRARY_PATH=/home/allen/tmp/calc/test:$LD_LIBRARY_PATH		# 使用字符串拼接 
  ```

  - 只对当前终端有效，相当于一个测试。

  - 如果永久生效可以放到配置文件里面`.bashrc`或`/etc/profile`（系统级别）

    ```bash
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:动态库的绝对路径
    ```

    使用`source`或者`.`使其生效

##### 方案二：更新/etc/ld.so.cache文件

- ```bash
  allen@allen-Dell-G15-5530:~$ sudo vim /etc.ld.so.conf
  /etc/ld.so.conf：
  		include /etc/ld.so.conf.d/*.conf                               
  ```

  在文件的下面添加动态库的绝对路径

- ```bash
  sudo ldconfig
  sudo ldconfig -v 			# 可以看见更新的信息，较多
  ```

  添加后必须执行一次，这样`/etc/ld.so.conf`中的数据就会更新到`/etc/ld.so.cache`

##### 方案三： 拷贝动态库文件到`lib`或`/user/lib`或者放入软链接

- ```bash
  sudo cp /xxx/xxx/libxxx.so /user/lib
  sudo ln -s /xxx/xxx/libxxx.so /user/lib/libxxx.so	
  ```

  如果用软链接的方法，后面更新动态库之后不用重新创建，拷贝则需要反复操作

# makefile

## 规则

- 组成部分：目标（target）、依赖（depend）和命令（command）

### 1.格式

```makefile
target1,target2,...: depend1,depend2,...
		command								# 每行都要加TAB缩进
		command
		.....
```

- 单目标

  ```makefile
  app:a.c b.c c.c
  		gcc a.c b.c c.c -o app 
  ```

- 多目标

  ```makefile
  app,app1:a.c b.c c.c d.c
  		gcc a.c b.c -o app
  		gcc c.c d.c -o app1
  ```

- 规则间的嵌套

  ```makefile
  app:a.o b.o c.o						# 规则执行完毕时，目标还没有被生成出来（伪目标）
  		gcc a.o b.o c.o -o app
  a.o:a.c								# 也可能会出现不存在依赖的情况，直接空着就行
  		gcc -c a.c
  b.o:b.c
  		gcc -c b.c
  c.o:c.c
  		gcc -c c.c
  ```

### 2. 工作原理

- ```bash
  make	# make 只会执行makefile里面的第一条规则
  ```

  当发现依赖不存在的时候，make就会查找其他的规则，看哪一条规则用来生成需要的这个依赖的

- ```bash
  make 规则里面的目标名	# 只执行某一个规则
  ```

- make是通过比较target和depend的时间戳来判定是否需要更新，目标时间戳一定是大于依赖的时间戳

### 3. 自动推导

- 如果规则不全，自动进行推导并不完全依赖makefile

  ```makefile
  app:a.o b.o c.o						
  		gcc a.o b.o c.o -o app
  ```

  make的时候发现缺失依赖，会自动去`gcc -c a.c`生成`a.o`这个依赖

### 4. 文件编写

- 两种格式：makefile或Makefile











