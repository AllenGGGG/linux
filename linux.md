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
