## Part A:Writing a Cache Simulator

### 参数说明
组相连高速缓存的三个主要参数：
- S：组数
- E：每组的行数
- B：2^b,块大小

### 设计思路
- 整个高速缓存的结构可以看做是一个二维数组，每个数组项对应于一个高速缓存行的变长结构体的指针。
- 二维数组的行对应高速缓存的组，列对应高速缓存的每一行的指针。
- 高速缓存行的结构设计：
1. 由于块大小由参数确定，因此该结构体应该是一个变长结构体。
2. 高速缓存的替换算法是LRU，因此结构体内应包含LRU计数位，可以直接用long型变量来表示。
```C
typedef struct {
	bool valid;
	unsigned long key;
	unsigned long LRU_count;
	char b[0];
}cache_entry;
```
### TRACE_FILE的解析
- trace file 是由linux程序valgrind生成的，用来跟踪程序执行过程中的缓存命中情况。
```shell
linux> valgrind --log-fd=1 --tool=lackey -v --trace-mem=yes ls -l
```

-  trace file 的字段解释：
```shell
[space]operation address,size
其中，operation包含I 指令加载，L 数据加载，S 数据写入，M 数据修改；
只有I指令前没有空格；
```

- L指令：先看是否命中，如果不命中，判断组中是否有空闲的缓存块，如果没有则根据LRU选择牺牲一个缓存项，然后加载数据。

- S指令：先看是否命中，如果不命中，判断组中是否有空闲的缓存块，如果没有则根据LRU选择牺牲一个缓存项，然后写入数据，代码实现上和L指令完全相同。

- M指令：先看是否命中，如果不命中则加载，然后将新值写入，类似L和S的融合。

### LRU替换算法的实现
- 此处采用软件模拟的方式，为每一个缓存项维护一个LRU_count域，当所在组被访问时，组中所有缓存项的LRU_count右移一位，同时命中的缓存项加0x8000000000000000。

- 如果某组的缓存项已满且需要置换时，选择LRU_count值最小的缓存项置换。

### 结果
```shell
$./test-csim 
                        Your simulator     Reference simulator
Points (s,E,b)    Hits  Misses  Evicts    Hits  Misses  Evicts
     3 (1,1,1)       9       8       6       9       8       6  traces/yi2.trace
     3 (4,2,4)       4       5       2       4       5       2  traces/yi.trace
     3 (2,1,4)       2       3       1       2       3       1  traces/dave.trace
     3 (2,1,3)     167      71      67     167      71      67  traces/trans.trace
     3 (2,2,3)     201      37      29     201      37      29  traces/trans.trace
     3 (2,4,3)     212      26      10     212      26      10  traces/trans.trace
     3 (5,1,5)     231       7       0     231       7       0  traces/trans.trace
     6 (5,1,5)  265189   21775   21743  265189   21775   21743  traces/long.trace
    27
TEST_CSIM_RESULTS=27
```

### 细节注意
- LRU_count 和 key 域都应该声明成无符号长整型，这是因为C语言的移位操作会根据有无符号选择逻辑移位还是符号移位，这里很有可能影响计算结果。

- free和malloc要成对使用，malloc后应当及时free，即便这个simulator里不做free不会造成问题，但是不作free很有可能造成内存溢出。

- 官方文档建议采用getopt.h解析命令行参数，初步实现时没有注意到这一点，采用了原始的字符串比较，后面有时间可以adopt这个建议。