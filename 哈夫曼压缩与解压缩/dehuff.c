#include <stdio.h>
#include <string.h>

#include "./include/dehuff.h"
#include "./include/huffman.h"

HUFFMAN_HEAD ReadHuffmanHead(char *sourceFileName) {
	FILE *in = NULL;
	HUFFMAN_HEAD head = {0};

	in = fopen(sourceFileName, "rb");

	fread(&head, sizeof(HUFFMAN_HEAD), 1, in);

	fclose(in);

	return head;
}

FREQ *ReadFreq(char *file, int alphaCount) {
	FILE *in = NULL;

	FREQ *freq = NULL;

	if (NULL == file) {
		return NULL;
	}

	in = fopen(file, "rb");

	freq = (FREQ *) calloc(sizeof(FREQ), alphaCount);

	fseek(in, sizeof(HUFFMAN_HEAD), SEEK_SET);
	fread(freq, sizeof(FREQ), alphaCount, in);

	fclose(in);

	return freq;
}

boolean IsHuffmanHead(HUFFMAN_HEAD head) {
	return head.flag[0] == 'X'
			&& head.flag[1] == 'U'
			&& head.flag[2] == 'P'
			&& head.flag[3] == 'T';
}

void Decoding(char *sourceFileName, char *targetFileName,
			 HUFF_TAB *huffTab, HUFFMAN_HEAD *head) {
	FILE *in = NULL;
	FILE *out = NULL;

	int ch = 0;
	int count = 0;
	int bitIndex = 8;
	int root =  2 * (head->alphaCount) - 2;

	in = fopen(sourceFileName, "rb");
	out = fopen(targetFileName, "wb");

	fseek(in, sizeof(HUFFMAN_HEAD) + sizeof(FREQ) * (head->alphaCount), SEEK_SET);

	while (count <= head->btyeCount) {
		
		if (huffTab[root].leftChild == -1) {
			fputc(huffTab[root].data.ch, out);
			root =  2 * (head->alphaCount) - 2;
		}
		else {
			if (bitIndex >= 8) {
				ch = fgetc(in);
				bitIndex = 0;
			}
			if (GET(ch, bitIndex) == 0) {
				root = huffTab[root].leftChild;
			}
			else {
				root = huffTab[root].rightChild;
			}
			count++;
			bitIndex++;
		}
	}

	fclose(in);
	fclose(out);
}

int main(int argc, char const *argv[]) {
	char sourceFileName[1024] = {0};
	char targetFileName[1024] = {0};

	FREQ *freq = NULL;

	HUFFMAN_HEAD head = {0};

	HUFF_TAB *huffTab = NULL;
	int huffManIndex[256] = {0};

	char string[8] = {0};

	if (argc < 3) {
		printf("用法 dehuff <源文件> <目标文件> (名称 + 文件后缀)\n");
		return 0;
	}

	strcpy(sourceFileName, argv[1]);
	strcpy(targetFileName, argv[2]);
	if (FALSE == IsFileOpen(sourceFileName)) {
		printf("[%s]文件无法打开!",sourceFileName);
		return 0;
	}

	head = ReadHuffmanHead(sourceFileName);
	if (!IsHuffmanHead(head)) {
		printf("[%s]不可识别文件", sourceFileName);
		return 0;
	} 

	freq = ReadFreq(sourceFileName, head.alphaCount);

	initHuffTab(&huffTab, freq, huffManIndex, head.alphaCount);

	CreatHuffTree(huffTab, head.alphaCount);
	
	Decoding(sourceFileName, targetFileName, huffTab, &head);

	DestoryHuffTree(&huffTab, head.alphaCount);
	free(freq);
	
	return 0;
}