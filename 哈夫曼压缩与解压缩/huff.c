#include <stdio.h>
#include <string.h>

#include "./include/huff.h"
#include "./include/huffman.h"
#include "./include/boolean.h"

FREQ *GetFreq(char *file, int *alphaCount) {
	FILE *in = NULL; 
	u8 ch = '0';
	int i = 0;
	FREQ  alpha[256] = {0};
	FREQ *freq = NULL;
	int t = 0;

	if (NULL == file) {
		return NULL;
	}

	in = fopen(file, "rb");
	ch = fgetc(in);
	while (!feof(in)) {
		++alpha[ch].freq;
		ch = fgetc(in);
	}

	for (i = 0; i < 256; i++) {
		if (0 != alpha[i].freq) {
			++*alphaCount;
		}
	}

	freq = (FREQ *) calloc(sizeof(FREQ), *alphaCount);
	for (i = 0; i < 256; i++) {
		if (0 != alpha[i].freq) {		
			freq[t].ch = i;
			freq[t].freq = alpha[i].freq;
			++t;
		}
	}

	fclose(in);

	return freq;
}

void InitHuffmanHead(HUFFMAN_HEAD *head, HUFF_TAB *huffTab, int alphaCount) {
	int i = 0;
	head->alphaCount = alphaCount;

	head->flag[0] = 'X';
	head->flag[1] = 'U';
	head->flag[2] = 'P';
	head->flag[3] = 'T';

	for (i = 0; i < alphaCount; i++) {
		head->btyeCount += strlen(huffTab[i].code) * huffTab[i].data.freq;
	}
}


void ProductHuffCode(HUFF_TAB *huffTab, int root, int index, char *str) {

	if (-1 == huffTab[root].leftChild) {
		str[index] = '\0';
		strcpy(huffTab[root].code, str);
	}
	else {
		str[index] = '0';
		ProductHuffCode(huffTab, huffTab[root].leftChild, index + 1, str);
		str[index] = '1';
		ProductHuffCode(huffTab, huffTab[root].rightChild, index + 1, str);
	}
}

void ProductU8Code(char *sourceFileName, char *targetFileName, HUFF_TAB *huffTab,
						int *huffManIndex, FREQ *freq, int alphaCount) {
	FILE *in = NULL;
	u8 ch;

	FILE *out = NULL;

	u8 btye;
	int btyeIndex = 0;

	char *code = NULL;
	int i      = 0;

	HUFFMAN_HEAD head = {0};

	in = fopen(sourceFileName, "rb");
	out = fopen(targetFileName, "wb");

	InitHuffmanHead(&head, huffTab, alphaCount);

	fwrite(&head, sizeof(HUFFMAN_HEAD), 1, out);
	fwrite(freq, sizeof(FREQ), alphaCount, out);

	ch = fgetc(in);
	while (!feof(in)) {
		code = huffTab[huffManIndex[ch]].code;
		for (i = 0; code[i]; i++) {
			code[i] == '0' ? CLR(btye, btyeIndex) : SET(btye, btyeIndex);
			btyeIndex++;
			if (btyeIndex >= 8) {
				btyeIndex = 0;
				fputc((int)btye, out);
			}
		}
		ch = fgetc(in);
	}
	if (btyeIndex != 0) {
		fputc((int)btye, out);
	}

	fclose(in);
	fclose(out);
}

//0001 1010 1001 0010 0100 0000 0010 0111 0101 1100 0000 1101 1110 0011 0110 0010 1
//	1	a 	9	 2  	4	0	2	 7		5	 C   0	  d    E    3    6    2   	
int main(int argc, char const *argv[]) {
	char sourceFileName[1024] = {0};	
	char targetFileName[1024] = {0};
	FREQ *freq = NULL;
	int alphaCount = 0;

	HUFF_TAB *huffTab = NULL;

	char string[8] = {0};
	
	int huffManIndex[256] = {0};

	HUFFMAN_HEAD head;

	if (argc < 3) {
		printf("用法 huff <源文件> <目标文件>");
		return 0;
	}

	strcpy(sourceFileName, argv[1]);
	if (FALSE == IsFileOpen(sourceFileName)) {
		printf("[%s]文件无法打开!",sourceFileName);
		return 0 ;
	}
	strcpy(targetFileName, argv[2]);

	freq = GetFreq(sourceFileName, &alphaCount);

	initHuffTab(&huffTab, freq, huffManIndex, alphaCount);

	CreatHuffTree(huffTab, alphaCount);

	ProductHuffCode(huffTab, 2 * alphaCount - 2, 0, string);

	ProductU8Code(sourceFileName,targetFileName,huffTab,huffManIndex, freq, alphaCount);

	return 0;
}