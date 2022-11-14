/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-13 20:21:09
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 01:55:11
 * @FilePath: /lab1/main.c
 * @Description: 
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include <stdio.h>
#include <sys/stat.h>
#include <linux/fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include "string.h"
#include <dirent.h>
#include <stdlib.h>

#include "parallel.h"


extern int ERROR_FILE_TYPE;
extern int ERROR_FILE_PERMISSION;
extern int ERROR_FSTATAT;
extern int ERROR_FILE_OPEN;
extern int ERROR_SUFFIX;

extern int all;
extern int blank_ignore;
extern int recursive;
extern int use_suffix;
extern char suffix_name[MAXLINE];
extern int help;


//多线程相关
extern int enable_thread;       //是否开启多线程
extern int thread_num;             //线程数
extern Queue queues[MAXTHREAD];   //每个线程对应的任务队列
extern int next_queue_idx;
extern int parallel_total;

/**
 * @description: 打印文件/文件夹统计结果
 * @param {char} *path：文件/文件夹路径
 * @param {int} line：行数（错误信息）
 * @return {*}
 */
void print_result(char *path, int line){
    if (all!=1){
        if(line==ERROR_SUFFIX){
            return;
        }else if(line<0){
            printf("%s    %s\n",getError(line),path);
        }else{
            printf("%4d     %s\n",line,path);
        }
    }
}

//打开从命令行获取的文件路径，以及遍历文件夹时调用此函数
/**
 * @description: 检查一个文件类型和访问权限
 * @param {char*} path : 文件路径
 * @return {*} ：<0:文件错误类型 1:普通文件 2:目录文件
 */
int checkType(char* path){
    struct stat buf;
    if(fstatat(AT_FDCWD, path, &buf, 0)<0){
        return ERROR_FSTATAT;
    }
    //首先检查文件权限
    if(buf.st_mode & S_IRUSR){
        if(S_ISDIR(buf.st_mode)){
            return 2;
        }else if(S_ISREG(buf.st_mode)){
            return 1;
        }else{
            return ERROR_FILE_TYPE;
        }
    }else{
        return ERROR_FILE_PERMISSION;
    }
}

/**
 * @description: 检查文件是否满足suffix_name中指定后缀
 * @param {char*} path：待检查文件
 * @return {*}： 1：满足 0：不满足
 */
int checkSuffix(char* path,char* suffix_name){
    int len = strlen(path);
    int len_suffix = strlen(suffix_name);
    if(len < len_suffix){
        return 0;
    }
    for(int i = 0; i < len_suffix; i++){
        if(path[len - len_suffix + i] != suffix_name[i]){
            return 0;
        }
    }
    return 1;
}

/**
 * @description: 统计一个普通文件的行数
 * @param {char*} path:文件路径
 * @param {int} mode：是否忽略空行；1：忽略空行；0：不忽略空行
 * @param {char*} suffix:文件后缀
 * @return {*}：<0:文件错误类型;other:文件行数
 */
//FIXME:将path路径的修改移出calSingle
int calSingle(char* path,int mode,char* suffix){
    if (suffix!=NULL){
        if(!checkSuffix(path,suffix)){
            return ERROR_SUFFIX;
        }
    }
    FILE* fp = fopen(path,"r");
    if(fp == NULL){
        return ERROR_FILE_OPEN;
    }
    int line = 0;
    if(mode == 1){
        //如果忽略空行，用fgets读取文件
        char buf[MAXLINE];
        while(fgets(buf,MAXLINE,fp)!=NULL){
            if(strcmp(buf,"\n")==0){
                continue;
            }
            line++;
        }
    }else{
        //如果不忽略空行，用fgetc读取文件
        char buf;
        while((buf = fgetc(fp))!=EOF){
            if(buf == '\n'){
                line++;
            }
        }
        line++;
    }
    //printf("文件行数为：%d\n",line);
    return line;
}

/**
 * @description: 
 * @param {char*} path：文件路径
 * @param {int} mode：是否忽略空行；1：忽略空行；0：不忽略空行
 * @param {int} recursive：是否递归处理子文件；1：递归；0：不递归
 * @return {*}: <0:文件错误类型;other:文件行数
 */
int calDir(char* path,int mode,int recursive,char* suffix){
    DIR *dir;
    struct dirent *dp;
    int lines = 0;
    if((dir=opendir(path))==NULL){
        //printf("目录打开失败\n");
        return ERROR_FILE_OPEN;
    }
    //这里的处理和从命令行获取的文件是类似的
    while((dp = readdir(dir)) != NULL){
        //跳过.和..目录
        if(strcmp(dp->d_name,".")==0 || strcmp(dp->d_name,"..")==0){
            continue;
        }
        //构造子文件路径
        char* filepath = (char*) malloc(strlen(path)+strlen(dp->d_name)+strlen("/")+10);
        sprintf(filepath,"%s/%s",path,dp->d_name);
        int type = checkType(filepath);
        if(type>0){
            if (type == 1){
                int singleline = calSingle(filepath,mode,suffix);
                print_result(filepath,singleline);
                if(singleline>0){
                    lines += singleline;
                }
            }else if(type==2 && recursive==1){
                int dirline = calDir(filepath,mode,recursive,suffix);
                //print_result(filepath,dirline);
                if(dirline>0){
                    lines += dirline;
                }
            }
        }else{
            print_result(filepath,type);
        }
    }
    closedir(dir);
    return lines;
}

int main(int argc,char *argv[])
{
    int options;
    int long_option_idx;
    char* optstring = "Abrs:h";
    static struct option long_options[] = {
        {"all",     no_argument,       0,  'A' },
        {"suffix",  required_argument, 0,  's' },
        {"help",    no_argument,       0,  'h' },
        {"thread",  required_argument, 0,  't' },
        {0,         0,                 0,   0  } 
    };
    
    while((options = getopt_long(argc, argv, optstring, long_options, &long_option_idx)) != -1){
        switch(options){
            case 'A':
                all = 1;
                break;
            case 'b':
                blank_ignore = 1;
                break;
            case 'r':
                recursive = 1;
                break;
            case 's':
                use_suffix = 1;
                strcpy(suffix_name,optarg);
                break;
            case 'h':
                help = 1;
                break;
            case 't':
                enable_thread = 1;
                thread_num = atoi(optarg);
                if(thread_num<1){
                    printf("线程数必须大于0\n");
                    return 0;
                }
                parallel_init();
                break;
            default:
                printf("err command!\n");
                return 0;
                break;
        }
    }

    //TODO:处理help指令(Done)
    if(help == 1){
        printf("Usage: lc [OPTION] ... [FILE | DIR] ...\n");
        printf("Print newline counts for each FILE or DIR, and a total line if more than one FILE is specified.\n");
        printf("\n");
        printf("With no FILE, or when FILE is -, read standard input.\n");
        printf("\n");
        printf("Mandatory arguments to long options are mandatory for short options too.\n");
        printf("    -A, --all                     print the total line number only\n");
        printf("    -b                            ignore the Blank line when counting\n");
        printf("    -r                            count directory Recursively\n");
        printf("    -s, --suffix=FILE-SUFFIX      only count the file with specified Suffix\n");
        printf("    -h, --help                    display this Help and exit\n");
        printf("    -t, --thread=NUMBER           use multithread to count \n");
        return 0;
    }

    int total = 0;
    printf("line    file\n");
    printf("----    ----\n");
    //TODO:将处理过程分类为单线程和多线程
    for(int i = optind; i < argc; i++){
        char* path = argv[i];
        int type = checkType(path);
        if (type>0){
            if(type==1){
                if(enable_thread==0){
                    char* filepath = (char*) malloc(strlen(path)+strlen("./"));
                    sprintf(filepath,"./%s",path);
                    int singleline = use_suffix==1?calSingle(filepath,blank_ignore,suffix_name):calSingle(filepath,blank_ignore,NULL);
                    print_result(path,singleline);
                    if(singleline>0){
                        total += singleline;
                    }
                }else{
                    //TODO:在这里实现多线程处理单个文件
                    //需要将该文件加入到任务队列中去
                    char* filepath = (char*) malloc(strlen(path)+strlen("./"));
                    sprintf(filepath,"./%s",path);
                    Task task;
                    task.path = filepath;
                    task.id = 1;
                    task.mode = blank_ignore;
                    task.suffix = use_suffix==1?suffix_name:NULL;

                    pthread_mutex_lock(&queueidxlock);
                    int queue_id = next_queue_idx;
                    Push(&queues[queue_id],task);
                    next_queue_idx = (next_queue_idx+1)%thread_num;
                    pthread_mutex_unlock(&queueidxlock);
                }
            }else if(type==2){
                if(enable_thread==0){
                    char* dirpath = (char*) malloc(strlen(path)+strlen("./"));
                    if((strcmp(path,".")==0 || strcmp(path,"..")==0)){
                        dirpath = path;
                    }else{
                        sprintf(dirpath,"./%s",path);
                    }
                    int dirline = use_suffix==1?calDir(dirpath,blank_ignore,recursive,suffix_name):calDir(dirpath,blank_ignore,recursive,NULL);
                    //print_result(path,dirline);
                    if(dirline>0){
                        total += dirline;
                    }
                }else{
                    //TODO:在这里实现多线程处理文件夹
                    char* dirpath = (char*) malloc(strlen(path)+strlen("./"));
                    if((strcmp(path,".")==0 || strcmp(path,"..")==0)){
                        dirpath = path;
                    }else{
                        sprintf(dirpath,"./%s",path);
                    }
                    if(use_suffix==1){
                        calDir_parallel(dirpath,blank_ignore,recursive,suffix_name);
                    }else{
                        calDir_parallel(dirpath,blank_ignore,recursive,NULL);
                    }
                }
            }
        }else{
            print_result(argv[i],type);
        }
    }

    if(enable_thread==0){
        //到这里，所有的任务都已经加入到任务队列中去了,向每个任务队列中加入一个结束标志
        callfinish();
        //创建线程
        pthread_t threads[thread_num];
        for(int i = 0; i < thread_num; i++){
            int ret = pthread_create(&threads[i],NULL,consume_task,(void*)i);
            if(ret!=0){
                printf("创建线程失败\n");
            }
        }
        //等待线程结束
        for(int i = 0; i < thread_num; i++){
            pthread_join(threads[i],NULL);
        }
        printf("%4d     %s\n",total,"total");
    }
    else{
        printf("%4d     %s\n",parallel_total,"total");
    }
    return 0;
}

