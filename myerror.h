/*
 * @Author: zy 953725892@qq.com
 * @Date: 2022-11-14 13:41:19
 * @LastEditors: zy 953725892@qq.com
 * @LastEditTime: 2022-11-15 01:23:08
 * @FilePath: /lab1/myerror.h
 * @Description: 记录运行过程中的error类型
 * 
 * Copyright (c) 2022 by zy 953725892@qq.com, All Rights Reserved. 
 */
#ifndef _MYERROR_H_ 	
#define _MYERROR_H_

/**
 * @description: 根据错误类型打印错误信息
 * @param {int} error_code：错误类型
 * @return {*}：错误信息
 */
char* getError(int error_code);


#endif