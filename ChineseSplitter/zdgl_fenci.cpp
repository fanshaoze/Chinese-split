#include"zdgl_fenci.h"
#include "stdafx.h"

const char *target = "target.txt";//输入文件
const char *out_put = "2011211366.txt";//输出文件 
const char *dic_text2 = "dic.txt";//输入词典文件 

const int max_word = 20;//假设一个词最长包括10个汉字


double laplace;//laplace平滑

map<string, double> dicz;//词典
map <string, double>::iterator dic_it;
//map键值对
void dic_init_test(void)//测试用
{
	int count_text = 10000000;
	laplace = (double)1 / (count_text + 1);
	dicz.insert(pair<string, double>("有", 0.018));
	dicz.insert(pair<string, double>("有意", 0.0005));
	dicz.insert(pair<string, double>("意见", 0.001));
	dicz.insert(pair<string, double>("见", 0.0002));
	dicz.insert(pair<string, double>("分歧", 0.0001));
}

void dic_init(void)//初始化词典
{
	ifstream file(dic_text2);
	int count_text;
	file >> count_text;
	laplace = (double)1 / (count_text + 1);
	string word_text;
	double rate_text;
	for (int i = 0; i<count_text; i++)
	{
		file >> word_text;
		file >> rate_text;
		dicz.insert(pair<string, double>(word_text, rate_text));
	}
	file.close();
}

string zdgl_fenci(string sentance)//最大概率分词，输入为不带“。”的句子，输出为分好的句子
{
	if (sentance.size() <= 2)
		return sentance;//单个字直接返回
	int min = 0, max = sentance.size() - 1;//单词位置标记

										   //第一步建立单词池
	vector<word_pre>word_pool;//单词池
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
			dic_it = dicz.find(temp_word);
			//cout << dic_it << endl;
			//cout << dicz.end();
			if (dic_it != dicz.end() || j == i)//有记录，或是第一个词
			{
				word_pre w_pre;
				word_pre_num++;
				w_pre.num = word_pre_num;

				if (dic_it != dicz.end())
					w_pre.word_rate = dic_it->second;
				else
					w_pre.word_rate = laplace;

				if (0 == i)//句头词
				{
					w_pre.plus_rate = w_pre.word_rate;
					w_pre.best = 0;
				}
				else
				{
					w_pre.best = -1;
					w_pre.plus_rate = (double)-1;
				}
				w_pre.p_begin = i;
				w_pre.p_end = j + 1;
				w_pre.this_word = temp_word;
				word_pool.push_back(w_pre);//入池
			}
		}
	}
	//第二种方法，记录所有可能情况
	/*
	//严格全部入池
	for(int i=0; i<=max; i+=2)
	{

	temp_word.clear();
	for(int j=i; j<=max&&j<max_word+i; j+=2)
	{
	temp_word.append(1, sentance.at(j));
	temp_word.append(1, sentance.at(j+1));
	//入池
	word_pre_num++;
	word_pre w_pre;

	w_pre.num=word_pre_num;
	dic_it=dicz.find(temp_word);
	if(dic_it!=dicz.end())
	{
	w_pre.word_rate=dic_it->second;
	}
	else
	{
	w_pre.word_rate=laplace;
	}
	if(0==i)
	{
	w_pre.plus_rate=w_pre.word_rate;
	w_pre.best=0;
	}
	else
	{
	w_pre.best=-1;
	w_pre.plus_rate=double(-1);
	}
	w_pre.p_begin=i;
	w_pre.p_end=j+1;
	w_pre.this_word=temp_word;
	word_pool.push_back(w_pre);//入池
	}
	}

	*/


	//第二步计算最佳左邻词
	bool fail = true;//标记
	while (fail)
	{
		fail = false;
		for (int i = 0; i<word_pool.size(); i++)//遍历整个单词池
		{
			if (-1 == (word_pool.at(i)).best)//没有算出左邻词
			{
				int p_begin_temp = (word_pool.at(i)).p_begin;
				vector<int>best_word_temp;//计算最佳左邻词用
				for (int j = 0; j<word_pool.size(); j++)//遍历整个单词池
				{
					if (p_begin_temp == (word_pool.at(j)).p_end + 1)//再考虑范围内
					{
						if (-1 == (word_pool.at(j)).best)//考虑的左邻词本身数据不全
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
						if ((word_pool.at(best_word_temp.at(k))).plus_rate>(word_pool.at(best_word_temp.at(max_p))).plus_rate)
						{
							max_p = k;
						}
					}
					//记录左邻词和概率
					(word_pool.at(i)).best = best_word_temp.at(max_p);
					(word_pool.at(i)).plus_rate = ((word_pool.at(i)).word_rate)*(word_pool.at(best_word_temp.at(max_p)).plus_rate);

				}

			}

		}

	}


	//第三步，选出最佳句尾词，并通过最佳左邻词，返回直到句头词。
	vector<int>end_word_temp;
	for (int i = 0; i<word_pool.size(); i++)
	{
		if (max == (word_pool.at(i)).p_end)//是结尾词
		{
			end_word_temp.push_back(i);
		}
	}

	int best_end_word = 0;//初始化
	for (int i = 1; i<end_word_temp.size(); i++)
	{
		if ((word_pool.at(end_word_temp.at(i))).plus_rate>(word_pool.at(end_word_temp.at(best_end_word))).plus_rate)
		{
			best_end_word = i;
		}
	}
	int position = end_word_temp.at(best_end_word);
	vector<string>word_complete;
	while (0 != (word_pool.at(position)).p_begin)//往回找
	{
		word_complete.push_back((word_pool.at(position)).this_word);
		position = (word_pool.at(position)).best;
	}
	word_complete.push_back((word_pool.at(position)).this_word);//最后一个

																//分词完成，每个词都放在word_complete里面 

	string complete;
	for (int i = word_complete.size() - 1; i >= 0; i--)//用空格分开，拼成成品 
	{
		complete += word_complete.at(i);
		if (0 != i)
			complete += "  |  ";
	}


	return complete;//返回 
}


int adgl()
{

	//dic_init_test();//测试用
	dic_init();
	FILE *f_in;
	ofstream f_out(out_put);
	f_in = fopen(target, "r");
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
			s_complete = zdgl_fenci(sentance);
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
				s_complete = zdgl_fenci(sentance);
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
				s_complete = zdgl_fenci(sentance);
				s_complete += " 。 ";//加上“。”
				f_out << s_complete;//输出
			}
			break;
		}
	}


	/*测试
	s_complete=zdgl_fenci("有意见分歧");
	s_complete+=" 。 ";//加上“。”
	f_out<<s_complete;//输出
	*/

	fclose(f_in);
	f_out.close();

	return 0;
}
