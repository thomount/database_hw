#pragma once
#include "data.h"

int get_time();								//获取当前时间
int read(int index, int time);				//读取index数据time时的版本
//void get_lock(int index);					//获取index数据的写锁
//void release_lock(int index);				//释放index数据的写锁
//void write(int index, int data, int time);	//在time时刻向index写入data数据
void init(int index, int data);				//初始化index数据为data
//oper parse(string s);						//根据s获得指令
int get_name(char * s);
int start();
int commit(vector<WriteEvent> & wv, int id);
FILE* initData(int & n);
double get_average();