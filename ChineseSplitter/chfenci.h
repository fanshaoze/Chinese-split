#pragma once
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<map>
#include<string>
#include<vector>
#include<stack>

using namespace std;

typedef struct wordlist//���ʳ���Ԫ��
{
	double rateone;//���ʱ������
	double rateall;//�����۽�����
	int bestleft;//������ڴ�
	int begin;//��ʼλ��
	int end;//����λ��
	int index;//���
	string wordnow;//�ʱ���
}wordlist;

void init(void);//��ʼ���ʵ�
string zfenci(string sentance);
int adgl();
