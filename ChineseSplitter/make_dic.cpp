#include "make_dic.h"
#include "stdafx.h"
const char *train = "train.txt";//ѵ���ļ� 
const char *dicdoc = "dic.txt";//����ʵ��ļ� 


map <string, int> dic;//�ʵ��
map <string, int>::iterator nowdic;

//map<string, double> dic_in_text;//test

int makedic()
{
	double gailv = 0;
	int number = 0,x = 0;
	// yes
	char ch;
	FILE *fin;
	string atom;
	string temps = "";
	fin = fopen(train, "r");
	ch = fgetc(fin);
	ofstream fout(dicdoc);
	while (ch != EOF) {
		if (ch != '\n'  && ch != ' ') {
			atom.append(1, ch);
			if ("��" == atom) atom.clear();
		}
		else {
			if (" " == atom || 0 == atom.size()) {
				atom.clear();
				ch = fgetc(fin);
				continue;
			}
			else {
				nowdic = dic.find(atom);
				if (dic.end() == nowdic) {
					dic.insert(pair<string, int>(atom, 1));
					number++;
					atom.clear();
				}
				else {
					nowdic->second += 1;
					atom.clear();
				}
			}
		}
		ch = fgetc(fin);
	}
	nowdic = dic.begin();
	nowdic++;
	fout << number << endl;
	while (nowdic != dic.end())
	{
		for (x = 0;; x++) if (nowdic->first[x] == '/') break;
		gailv = (double)(nowdic->second) / number;
		fout << nowdic->first.substr(0, x) << endl;
		fout << gailv << endl;
		nowdic++;
	}
	fout.close();
	fclose(fin);
	return 0;
}