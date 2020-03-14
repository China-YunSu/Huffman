#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_ 

#include "boolean.h"

#pragma pack(push)
#pragma pack(1)

typedef struct FREQ {
	u8 ch;
	int freq;
}FREQ;

typedef struct HUFF_TAB {
	FREQ data;
	int leftChild;
	int rightChild;
	boolean isVisited;
	char *code;
}HUFF_TAB;

typedef struct HUFFMAN_HEAD {
	char flag[4];
	int	alphaCount;
	int btyeCount;
	u8 unused[4];
}HUFFMAN_HEAD;


boolean IsFileOpen(char *file);
boolean initHuffTab(HUFF_TAB **huffTab,FREQ *freq, int *huffManIndex, int alphaCount);
int findMinFre(HUFF_TAB *huffTab, int count);
boolean CreatHuffTree(HUFF_TAB *huffTab, int Count);
void ShowFreq(FREQ *freq, int alphaCount);
void ShowHuffCode(char *sourceFileName, HUFF_TAB *huffTab, int *huffManIndex);
void ShowHuffTree(HUFF_TAB *huffTab,int alphaCount);
void DestoryHuffTree(HUFF_TAB **huffTab,int alphaCount);

#pragma pack(pop)

#endif