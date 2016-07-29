#include"chfenci.h"
#include "stdafx.h"

const char *divresult = "divresult.txt";//输入文件
const char *result = "result.txt";//输出文件 
const char *dictest2 = "dic(1).txt";//输入词典文件 






const int max_word = 20;//假设一个词最长包括10个汉字


double laplace;//laplace平滑

map<string, double> dic;//词典
map <string, double>::iterator dic_it;





void init(void)//初始化词典
{
	ifstream file(dictest2);
	int count_text;
	file >> count_text;
	laplace = (double)1 / (count_text + 1);
	string word_text;
	double rate_text;
	for (int i = 0; i<count_text; i++)
	{
		file >> word_text;
		file >> rate_text;
		dic.insert(pair<string, double>(word_text, rate_text));
	}
	file.close();
}

string zfenci(string sentance)//最大概率分词，输入为不带“。”的句子，输出为分好的句子
{
	if (sentance.size() <= 2)
		return sentance;//单个字直接返回
	int min = 0, max = sentance.size() - 1;//单词位置标记

										   //第一步建立单词池
	vector<wordlist>word_pool;//单词池
	int word_pre_num = 0;
	//stack<int>word_link;
	string temp_word;

	//第一种方法，对于未出现词，只保存单个字
	for (int i = 0; i <= max; i += 2)
	{
		temp_word.clear();
		for (int j = i; j <= max&&j<max_word + i; j += 2)
		{
			temp_word.append(1, sentance.at(j));
			temp_word.append(1, sentance.at(j + 1));

			dic_it = dic.find(temp_word);
			if (dic_it != dic.end() || j == i)//有记录，或是第一个词
			{
				wordlist wordlist;
				word_pre_num++;
				wordlist.index = word_pre_num;

				if (dic_it != dic.end())
					wordlist.rateone = dic_it->second;
				else
					wordlist.rateone = laplace;

				if (0 == i)//句头词
				{
					wordlist.rateall = wordlist.rateone;
					wordlist.bestleft = 0;
				}
				else
				{
					wordlist.bestleft = -1;
					wordlist.rateall = (double)-1;
				}
				wordlist.begin = i;
				wordlist.end = j + 1;
				wordlist.wordnow = temp_word;
				word_pool.push_back(wordlist);//入池
			}
		}
	}
	//第二步计算最佳左邻词
	bool fail = true;//标记
	while (fail)
	{
		fail = false;
		for (int i = 0; i<word_pool.size(); i++)//遍历整个单词池
		{


			if (-1 == (word_pool.at(i)).bestleft)//没有算出左邻词
			{
				int p_begin_temp = (word_pool.at(i)).begin;
				vector<int>best_word_temp;//计算最佳左邻词用
				for (int j = 0; j<word_pool.size(); j++)//遍历整个单词池
				{
					if (p_begin_temp == (word_pool.at(j)).end + 1)//再考虑范围内
					{
						if (-1 == (word_pool.at(j)).bestleft)//考虑的左邻词本身数据不全
						{
							fail = true;//标记未完成
							best_word_temp.clear();
							break;//跳出循环
						}
						else//
						{
							best_word_temp.push_back(j);//记录
						}
					}
				}
				if (0 != best_word_temp.size())//所有备选项资料完备
				{
					int max_p = 0;//best_word_temp序号
					for (int k = 1; k<best_word_temp.size(); k++)//遍历，找到概率最大的
					{
						if ((word_pool.at(best_word_temp.at(k))).rateall>(word_pool.at(best_word_temp.at(max_p))).rateall)
						{
							max_p = k;
						}
					}
					//记录左邻词和概率
					(word_pool.at(i)).bestleft = best_word_temp.at(max_p);
					(word_pool.at(i)).rateall = ((word_pool.at(i)).rateone)*(word_pool.at(best_word_temp.at(max_p)).rateall);

				}

			}

		}

	}


	//第三步，选出最佳句尾词，并通过最佳左邻词，返回直到句头词。
	vector<int>end_word_temp;
	for (int i = 0; i<word_pool.size(); i++)
	{
		if (max == (word_pool.at(i)).end)//是结尾词
		{
			end_word_temp.push_back(i);
		}
	}

	int best_end_word = 0;//初始化
	for (int i = 1; i<end_word_temp.size(); i++)
	{
		if ((word_pool.at(end_word_temp.at(i))).rateall>(word_pool.at(end_word_temp.at(best_end_word))).rateall)
		{
			best_end_word = i;
		}
	}
	int position = end_word_temp.at(best_end_word);
	vector<string>word_complete;
	while (0 != (word_pool.at(position)).begin)//往回找
	{
		word_complete.push_back((word_pool.at(position)).wordnow);
		position = (word_pool.at(position)).bestleft;
	}
	word_complete.push_back((word_pool.at(position)).wordnow);//最后一个

																//分词完成，每个词都放在word_complete里面

	string complete;
	for (int i = word_complete.size() - 1; i >= 0; i--)//用空格分开，拼成成品
	{
		complete += word_complete.at(i);
		if (0 != i)
			complete += "\n";
	}


	return complete;//返回
}


int adgl()
{

	//dic_init_test();//测试用
	init();
	FILE *f_in;
	ofstream f_out(result);
	f_in = fopen(divresult, "r");
	char ch1 = 0, ch2 = 0;
	string word, sentance, s_complete;

	ch1 = fgetc(f_in);
	if (EOF == ch1)
		cout << "file id empty";
	ch2 = fgetc(f_in);

	while (EOF != ch1&&EOF != ch2)
	{

		word.append(1, ch1);
		word.append(1, ch2);
		if ("。" == word)//一个句子
		{
			s_complete.clear();
			s_complete = zfenci(sentance);
			s_complete += " 。 ";//加上“。”
			f_out << s_complete;//输出
			sentance.clear();//还原
		}
		else//不是一个句子
		{
			sentance += word;
		}

		word.clear();//还原
		ch1 = fgetc(f_in);
		if (EOF == ch1)
		{
			if (sentance.size()>0)//防止漏掉最后一句话
			{
				s_complete.clear();
				s_complete = zfenci(sentance);
				s_complete += " 。 ";//加上“。”
				f_out << s_complete;//输出
			}
			break;
		}
		ch2 = fgetc(f_in);
		if (EOF == ch2)
		{
			if (sentance.size()>0)//防止漏掉最后一句话
			{
				s_complete.clear();
				s_complete = zfenci(sentance);
				s_complete += " 。 ";//加上“。”
				f_out << s_complete;//输出
			}
			break;
		}
	}


	/*测试
	s_complete=zfenci("有意见分歧");
	s_complete+=" 。 ";//加上“。”
	f_out<<s_complete;//输出
	*/

	fclose(f_in);
	f_out.close();

	return 0;
}
