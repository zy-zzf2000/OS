<!--
 * @Author: zy 953725892@qq.com
 * @Date: 2023-01-12 19:32:45
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-12 20:34:23
 * @FilePath: /lab1/doc/lab2实验报告.md
 * @Description: 
 * 
 * Copyright (c) 2023 by zy 953725892@qq.com, All Rights Reserved. 
-->
# lab2实验报告

## 1.实现思路
主要实现了以下几个函数完成对lc命令的多线程扩展：
- `void InitQueue(Queue* queue)`: 初始化任务队列
- `void QueueDestroy(Queue* queue)`: 销毁任务队列
- `void Push(Queue* queue, Task task)`: 将任务压入指定任务队列
- `Task Pop(Queue* queue)` : 弹出指定队列的队首任务
- `int QueueEmpty(Queue* queue)`: 判断任务队列是否为空
- `void assign_task(Task *task,int queue_id)`: 将任务分配给指定的任务队列
- `void consume_task(void* i)`: 执行任务队列中的任务，直到任务队列为空
- `void calDir_parallel(char* path,int mode,int recursive,char* suffix)`: 对lab1中的calDir进行并行扩展
- `void callfinish()`: 在main线程分配完任务后，向所有队列中分别插入一个结束标志

基本思路是将原本的main线程作为分配任务的master线程，-t 参数指定的线程个数为worker线程个数；对于每个worker线程，master线程会给它初始化一个任务队列，master线程不断将所有待统计的文件作为任务加入到各个worker线程对应的任务队列中，当所有任务分配完毕后，master线程会向各个worker线程对应的任务队列中分别插入一个结束标志；而worker线程则不断从自己对应的任务队列中取出任务并且执行统计工作，直到取出的任务为结束标志时，worker线程结束，并且将自己的计算结果汇总到最终结果上。注意分配任务以及结果汇总时，需要使用pthread_mutex_lock加锁，保证并发安全。

## 2.测试输出结果
### 2.1 测试目录
<figure>
    <img src="pics/lab2_01.png">
</figure>

### 2.2 命令参数
<figure>
    <img src="pics/lab2_02.png">
</figure>

### 2.3 单线程测试
<figure>
    <img src="pics/lab2_03.png">
</figure>

### 2.4 多线程测试
<figure>
    <img src="pics/lab2_04.png">
</figure>

### 2.5 对比不同线程下程序执行时间
<figure>
    <img src="pics/lab2_05.png">
</figure>

| 线程数 | 运行时间(ms) | 加速比 |
| ------ | ------------ | ------ |
| 单线程 | 2.019        | 1.00   |
| 2      | 1.777        | 1.13   |
| 3      | 1.233        | 1.63   |
| 4      | 1.143        | 1.76   |
| 5      | 1.150        | 1.75   |
| 6      | 0.741        | 2.72   |
| 7      | 0.839        | 2.41   |
| 8      | 0.684        | 2.95   |
| 16     | 0.653        | 3.09   |
| 32     | 0.706        | 2.85   |

可以看出，当线程数为8时，多线程lc程序相比原本的单线程版本有最大加速比，其执行效率可以达到原来的3倍，而当线程数超过8时，多线程lc程序的执行效率反而会下降，这主要有两方面原因，一是这是因为当线程数过多时，系统的上下文切换开销会增大，导致效率下降；另一方面是因为测试的文件夹一共包含8个文件，而多线程版本是按照文件来给各个线程分配任务的，当线程多于8个时，后续的线程会没有任务可做，这样就会导致线程的执行效率下降。

C语言实现：
- 采用pthread库完成线程的创建、同步等操作
- 采用pthread_mutex_lock加锁，保证并发安全
- 采用master-worker的并发模式，main线程为每个worker线程分配一个任务队列，每个worker从自己的任务队列中取出任务并且执行
- 最终可执行文件大小为40K