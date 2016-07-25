#include "stdafx.h"
#include "splitall.h"

int splitall()
{
	char ch;
	FILE *f_in, *f_out;//输入输出文件 
	f_in = fopen("test.txt", "r");
	f_out = fopen("divresult.txt", "w");

	ch = getc(f_in);
	while (EOF != ch)
	{
		if (' ' != ch&&'\n' != ch) putc(ch, f_out);
		ch = getc(f_in);
	}
	return 0;
}
