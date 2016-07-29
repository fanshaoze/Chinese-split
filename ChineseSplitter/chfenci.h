#pragma once
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<map>
#include<string>
#include<vector>
#include<stack>

using namespace std;

typedef struct wordlist//单词池内元素
{
	double rateone;//单词本身概率
	double rateall;//单词累进概率
	int bestleft;//最佳左邻词
	int begin;//起始位置
	int end;//结束位置
	int index;//标记
	string wordnow;//词本身
}wordlist;

void init(void);//初始化词典
string zfenci(string sentance);
int adgl();
