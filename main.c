/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-13 20:21:09
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-14 00:19:51
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


#define MAXLINE 1024


/**
 * @description: 检查一个文件类型和访问权限
 * @param {char*} path : 文件路径
 * @return {*} ： -1：文件类型错误 -2：文件权限错误  1：文件为普通文件  2：文件为目录文件
 */
int checkType(char* path){
    struct stat buf;
    fstatat(AT_FDCWD, path, &buf, 0);
    //首先检查文件类型
    if(S_ISREG(buf.st_mode)){
        //检查文件权限
        if(buf.st_mode & S_IRUSR){
            printf("文件为普通文件，且有读权限\n");
            return 1;
        }
        else{
            return -2;
        }
    }else if(S_ISDIR(buf.st_mode)){
        printf("文件为目录文件\n");
        return 2;
    }
    return -1;
}

/**
 * @description: 统计一个普通文件的行数
 * @param {char*} path:文件路径
 * @param {int} mode：是否忽略空行；1：忽略空行；0：不忽略空行
 * @return {*}：-1：文件打开错误；other：文件行数
 */
int calLine(char* path,int mode){
    char* filepath = (char*) malloc(strlen(path)+strlen("./"));
    sprintf(filepath,"./%s",path);
    FILE* fp = fopen("/home/zy/sourcecode/c/lab1/build/test.txt","r");
    if(fp == NULL){
        printf("文件打开失败\n");
        return -1;
    }
    int line = 0;
    char buf[MAXLINE];
    while(fgets(buf,MAXLINE,fp)){
        if(strcmp(buf,"\n")==0 && mode == 1){
            continue;
        }
        line++;
    }
    //fgets会读取最后一个换行符，所以如果不忽略换行，需要加一
    return mode==1?line:line+1;
}

//TODO:完成命令行参数解析(Done)
int main(int argc,char *argv[])
{
    int options;
    int long_option_idx;
    char* optstring = "Abrs:h";
    static struct option long_options[] = {
        {"all",     no_argument,       0,  'A' },
        {"suffix",  required_argument, 0,  's' },
        {"help",    no_argument,       0,  'h' },
        {0,         0,                 0,   0  } 
    };
    
    while((options = getopt_long(argc, argv, optstring, long_options, &long_option_idx)) != -1){
        switch(options){
            case 'A':
                printf("A or all\n");
                break;
            case 'b':
                printf("b\n");
                break;
            case 'r':
                printf("r\n");
                break;
            case 's':
                printf("s or suffix\n");
                break;
            case 'h':
                printf("h or help\n");
                break;
            default:
                printf("err!\n");
                break;
        }
    }

    //输出剩余文件名
    for(int i = optind; i < argc; i++){
        printf("argv[%d] is %s\n", i, argv[i]);
        int cal = calLine(argv[i],1);
        printf("文件行数为：%d\n",cal);
    }

    return 0;
}

