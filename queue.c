#include "queue.h"

//初始化队列
void InitQueue(Queue* queue){
    Node* head = (Node*)malloc(sizeof(Node));
    Node* tail = (Node*)malloc(sizeof(Node));
    queue->head = head;
    queue->tail = tail;
    head->next = tail;
    tail->pre = head;
}

//删除队列
void QueueDestroy(Queue* queue){
    Node* p = queue->head;
    while(p!=NULL){
        Node* temp = p;
        p = p->next;
        free(temp);
    }
}

//入队
void Push(Queue* queue, Task task){
    Node* node = (Node*)malloc(sizeof(Node));
    node->task = task;
    node->pre = queue->tail->pre;
    node->next = queue->tail;
    queue->tail->pre->next = node;
    queue->tail->pre = node;
}

//出队
Task Pop(Queue* queue){
    Node* node = queue->head->next;
    queue->head->next = node->next;
    node->next->pre = queue->head;
    Task ret = node->task;
    free(node);
    return ret;
}

//判断队列是否为空
int QueueEmpty(Queue* queue){
    if(queue->head->next == queue->tail){
        return 1;
    }else{
        return 0;
    }
}

//打印队列
void PrintQueue(Queue* queue){
    Node* p = queue->head->next;
    while(p!=queue->tail){
        printf("%s->",p->task.path);
    }
}