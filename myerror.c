/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-15 01:15:09
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 01:34:40
 * @FilePath: /lab1/myerror.c
 * @Description: 
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#include "myerror.h"

const int ERROR_FILE_TYPE = -1;    //文件类型错误
const int ERROR_FILE_PERMISSION = -2;    //文件权限错误
const int ERROR_FSTATAT = -3;    //fstatat error
const int ERROR_FILE_OPEN = -4;    //文件打开失败
const int ERROR_SUFFIX = -5;    //文件后缀名错误

char* getError(int error_code)
{
    switch(error_code){
        case -1:
            return "文件类型错误";
        case -2:
            return "文件权限错误";
        case -3:
            return "fstatat error";
        case -4:
            return "文件打开失败";
        case -5:
            return "文件后缀名错误";
        default:
            return "未知错误";
    }
}