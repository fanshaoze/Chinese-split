#include "stdafx.h"
#include "kill_space.h"

int killspace()
{
	FILE *f_in, *f_out;//输入输出文件 
	char ch;

	f_in = fopen("corpus_for_test.txt", "r");
	f_out = fopen("target.txt", "w");

	ch = getc(f_in);
	while (EOF != ch)
	{
		if (' ' != ch&&'\n' != ch)
			putc(ch, f_out);
		ch = getc(f_in);
	}

	return 0;
}
