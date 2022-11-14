/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 20:05:16
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 00:46:05
 * @FilePath: /lab1/queue.h
 * @Description: 任务队列数据结构实现
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _QUEUE_H_ 	
#define _QUEUE_H_
#include<stdio.h>
#include<stdlib.h>
//calSingle函数参数数据结构
typedef struct Task
{
    int id;         //任务id
    char* path;     //待处理文件路径
    int mode;       //任务模式
    char* suffix;   //后缀
}Task;

//队列节点数据结构
typedef struct Node
{
    Task task;      //任务数据
    struct Node* next;  //下一个节点
    struct Node* pre;   //前一个节点
}Node;

//队列数据结构
typedef struct Queue
{
    Node* head;     //队列头
    Node* tail;     //队列尾
    int size;       //队列大小
}Queue;

//初始化队列
void InitQueue(Queue* queue);

//删除队列
void QueueDestroy(Queue* queue);

//入队
void Push(Queue* queue, Task task);

//出队
Task Pop(Queue* queue);

//判断队列是否为空
int QueueEmpty(Queue* queue);

//打印队列
void PrintQueue(Queue* queue);

#endif