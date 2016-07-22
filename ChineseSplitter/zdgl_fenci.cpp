#include"zdgl_fenci.h"
#include "stdafx.h"

const char *target = "target.txt";//�����ļ�
const char *out_put = "2011211366.txt";//����ļ� 
const char *dic_text2 = "dic.txt";//����ʵ��ļ� 

const int max_word = 20;//����һ���������10������


double laplace;//laplaceƽ��

map<string, double> dicz;//�ʵ�
map <string, double>::iterator dic_it;
//map��ֵ��
void dic_init_test(void)//������
{
	int count_text = 10000000;
	laplace = (double)1 / (count_text + 1);
	dicz.insert(pair<string, double>("��", 0.018));
	dicz.insert(pair<string, double>("����", 0.0005));
	dicz.insert(pair<string, double>("���", 0.001));
	dicz.insert(pair<string, double>("��", 0.0002));
	dicz.insert(pair<string, double>("����", 0.0001));
}

void dic_init(void)//��ʼ���ʵ�
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

string zdgl_fenci(string sentance)//�����ʷִʣ�����Ϊ�����������ľ��ӣ����Ϊ�ֺõľ���
{
	if (sentance.size() <= 2)
		return sentance;//������ֱ�ӷ���
	int min = 0, max = sentance.size() - 1;//����λ�ñ��

										   //��һ���������ʳ�
	vector<word_pre>word_pool;//���ʳ�
	int word_pre_num = 0;
	//stack<int>word_link;
	string temp_word;

	//��һ�ַ���������δ���ִʣ�ֻ���浥����
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
			if (dic_it != dicz.end() || j == i)//�м�¼�����ǵ�һ����
			{
				word_pre w_pre;
				word_pre_num++;
				w_pre.num = word_pre_num;

				if (dic_it != dicz.end())
					w_pre.word_rate = dic_it->second;
				else
					w_pre.word_rate = laplace;

				if (0 == i)//��ͷ��
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
				word_pool.push_back(w_pre);//���
			}
		}
	}
	//�ڶ��ַ�������¼���п������
	/*
	//�ϸ�ȫ�����
	for(int i=0; i<=max; i+=2)
	{

	temp_word.clear();
	for(int j=i; j<=max&&j<max_word+i; j+=2)
	{
	temp_word.append(1, sentance.at(j));
	temp_word.append(1, sentance.at(j+1));
	//���
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
	word_pool.push_back(w_pre);//���
	}
	}

	*/


	//�ڶ�������������ڴ�
	bool fail = true;//���
	while (fail)
	{
		fail = false;
		for (int i = 0; i<word_pool.size(); i++)//�����������ʳ�
		{
			if (-1 == (word_pool.at(i)).best)//û��������ڴ�
			{
				int p_begin_temp = (word_pool.at(i)).p_begin;
				vector<int>best_word_temp;//����������ڴ���
				for (int j = 0; j<word_pool.size(); j++)//�����������ʳ�
				{
					if (p_begin_temp == (word_pool.at(j)).p_end + 1)//�ٿ��Ƿ�Χ��
					{
						if (-1 == (word_pool.at(j)).best)//���ǵ����ڴʱ������ݲ�ȫ
						{
							fail = true;//���δ���
							best_word_temp.clear();
							break;//����ѭ��
						}
						else//
						{
							best_word_temp.push_back(j);//��¼
						}
					}
				}
				if (0 != best_word_temp.size())//���б�ѡ�������걸
				{
					int max_p = 0;//best_word_temp���
					for (int k = 1; k<best_word_temp.size(); k++)//�������ҵ���������
					{
						if ((word_pool.at(best_word_temp.at(k))).plus_rate>(word_pool.at(best_word_temp.at(max_p))).plus_rate)
						{
							max_p = k;
						}
					}
					//��¼���ڴʺ͸���
					(word_pool.at(i)).best = best_word_temp.at(max_p);
					(word_pool.at(i)).plus_rate = ((word_pool.at(i)).word_rate)*(word_pool.at(best_word_temp.at(max_p)).plus_rate);

				}

			}

		}

	}


	//��������ѡ����Ѿ�β�ʣ���ͨ��������ڴʣ�����ֱ����ͷ�ʡ�
	vector<int>end_word_temp;
	for (int i = 0; i<word_pool.size(); i++)
	{
		if (max == (word_pool.at(i)).p_end)//�ǽ�β��
		{
			end_word_temp.push_back(i);
		}
	}

	int best_end_word = 0;//��ʼ��
	for (int i = 1; i<end_word_temp.size(); i++)
	{
		if ((word_pool.at(end_word_temp.at(i))).plus_rate>(word_pool.at(end_word_temp.at(best_end_word))).plus_rate)
		{
			best_end_word = i;
		}
	}
	int position = end_word_temp.at(best_end_word);
	vector<string>word_complete;
	while (0 != (word_pool.at(position)).p_begin)//������
	{
		word_complete.push_back((word_pool.at(position)).this_word);
		position = (word_pool.at(position)).best;
	}
	word_complete.push_back((word_pool.at(position)).this_word);//���һ��

																//�ִ���ɣ�ÿ���ʶ�����word_complete���� 

	string complete;
	for (int i = word_complete.size() - 1; i >= 0; i--)//�ÿո�ֿ���ƴ�ɳ�Ʒ 
	{
		complete += word_complete.at(i);
		if (0 != i)
			complete += "  |  ";
	}


	return complete;//���� 
}


int adgl()
{

	//dic_init_test();//������
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
		if ("��" == word)//һ������
		{
			
			s_complete.clear();
			s_complete = zdgl_fenci(sentance);
			s_complete += " �� ";//���ϡ�����
			f_out << s_complete;//���
			sentance.clear();//��ԭ
		}
		else//����һ������
		{
			sentance += word;
		}

		word.clear();//��ԭ
		ch1 = fgetc(f_in);
		if (EOF == ch1)
		{
			if (sentance.size()>0)//��ֹ©�����һ�仰
			{
				s_complete.clear();
				s_complete = zdgl_fenci(sentance);
				s_complete += " �� ";//���ϡ�����
				f_out << s_complete;//���
			}
			break;
		}
		ch2 = fgetc(f_in);
		if (EOF == ch2)
		{
			if (sentance.size()>0)//��ֹ©�����һ�仰
			{
				s_complete.clear();
				s_complete = zdgl_fenci(sentance);
				s_complete += " �� ";//���ϡ�����
				f_out << s_complete;//���
			}
			break;
		}
	}


	/*����
	s_complete=zdgl_fenci("���������");
	s_complete+=" �� ";//���ϡ�����
	f_out<<s_complete;//���
	*/

	fclose(f_in);
	f_out.close();

	return 0;
}
