#pragma once
#include<iostream>
#include<stdio.h>
#include<fstream>
#include<map>
#include<string>
#include<vector>
#include<stack>

using namespace std;

typedef struct word_pre//���ʳ���Ԫ��
{
	int num;//���
	int p_begin;//��ʼλ��
	int p_end;//����λ��
	double word_rate;//���ʱ������
	double plus_rate;//�����۽�����
	int best;//������ڴ�
	string this_word;//�ʱ���
}word_pre;

void dic_init_test(void);//������;
void dic_init(void);//��ʼ���ʵ�
string zdgl_fenci(string sentance);
int adgl();
