<!--
 * @Author: zy 953725892@qq.com
 * @Date: 2023-01-12 19:32:45
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2023-01-12 20:03:25
 * @FilePath: /lab1/doc/lab2实验报告.md
 * @Description: 
 * 
 * Copyright (c) 2023 by zy 953725892@qq.com, All Rights Reserved. 
-->
# lab1实验报告

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

基本思路是将原本的main线程作为分配任务的master线程，-t 参数指定的线程个数为worker线程个数；对于每个worker线程，master线程会给它初始化一个任务队列，master线程不断将所有待统计的文件作为任务加入到各个worker线程对应的任务队列中，当所有任务分配完毕后，master线程会向各个worker线程对应的任务队列中分别插入一个结束标志；而worker线程则不断从自己对应的任务队列中取出任务并且执行统计工作，直到取出的任务为结束标志时，worker线程结束，并且将自己的计算结果汇总到最终结果上。注意分配任务以及结果汇总时，需要使用