#include "make_dic.h"
#include "stdafx.h"
const char *train_text = "corpus_for_train.txt";//ѵ���ļ� 
const char *dic_text = "dic.txt";//����ʵ��ļ� 


map <string, int> dic;//�ʵ��
map <string, int>::iterator dic_it;

//map<string, double> dic_in_text;//test

int makedic()
{
	FILE *f_in;
	f_in = fopen(train_text, "r");
	ofstream f_out(dic_text);

	double rate = 0;
	int count = 0;
	char ch;
	string word;
	ch = fgetc(f_in);
	while (EOF != ch)
	{
		if (' ' != ch&&'\n' != ch)//�ʵ�һ���� 
		{
			word.append(1, ch);
			if ("��" == word)
				word.clear();

		}
		else//���ʽ��� 
		{

			if (" " == word || 0 == word.size())
			{
				word.clear();
				ch = fgetc(f_in);
				continue;
			}
			dic_it = dic.find(word);
			if (dic_it != dic.end())
			{
				//�ҵ� 
				dic_it->second = dic_it->second + 1;
				word.clear();
			}
			else
			{
				//�µ���
				count++;
				dic.insert(pair<string, int>(word, 1));
				word.clear();
			}

		}
		ch = fgetc(f_in);
	}
	f_out << count << endl;
	dic_it = dic.begin();
	while (dic_it != dic.end())
	{
		f_out << dic_it->first << endl;
		rate = (double)(dic_it->second) / count;
		f_out << rate << endl;
		dic_it++;
	}

	f_out.close();
	fclose(f_in);
	/*������
	ifstream file(dic_text);
	int count_text;
	file>>count_text;
	string word_text;
	double rate_text;
	for(int i=0; i<count_text; i++)
	{
	file>>word_text;
	file>>rate_text;
	dic_in_text.insert(pair<string,double>(word_text,rate_text));
	}

	file.close();
	*/

	return 0;
}