#include "make_dic.h"
#include "stdafx.h"
const char *train_text = "corpus_for_train.txt";//训练文件 
const char *dic_text = "dic.txt";//输出词典文件 


map <string, int> dic;//词典表
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
		if (' ' != ch&&'\n' != ch)//词的一部分 
		{
			word.append(1, ch);
			if ("。" == word)
				word.clear();

		}
		else//单词结束 
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
				//找到 
				dic_it->second = dic_it->second + 1;
				word.clear();
			}
			else
			{
				//新单词
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
	/*测试用
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