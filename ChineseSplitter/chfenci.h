#pragma once
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<map>
#include<string>
#include<vector>
#include<stack>

using namespace std;

typedef struct word_pre//单词池内元素
{
	int num;//标记
	int p_begin;//起始位置
	int p_end;//结束位置
	double word_rate;//单词本身概率
	double plus_rate;//单词累进概率
	int best;//最佳左邻词
	string this_word;//词本身
}word_pre;

void dic_init_test(void);//测试用;
void dic_init(void);//初始化词典
string zdgl_fenci(string sentance);
int adgl();
