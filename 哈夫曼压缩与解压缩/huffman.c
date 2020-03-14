#include <stdio.h>

#include "./include/boolean.h"
#include "./include/huffman.h"

boolean IsFileOpen(char *file) {
	FILE *in = NULL;
	if (NULL == file) {
		return FALSE;
	}

	in = fopen(file, "rb");

	if (NULL == in) {
		return FALSE;
	}

	fclose(in);

	return TRUE;
}


boolean initHuffTab(HUFF_TAB **huffTab,FREQ *freq, int *huffManIndex, int alphaCount) {
	HUFF_TAB *res = NULL;
	int ch = 0;
	int index = 0;

	if (NULL == huffTab || NULL == freq || alphaCount < 1) {
		return FALSE;
	}

	res = (HUFF_TAB *) calloc(sizeof(HUFF_TAB), 2 * alphaCount - 1);

	for (index = 0; index < alphaCount; index++) {
			res[index].data.ch = freq[index].ch;
			res[index].data.freq = freq[index].freq;
			res[index].leftChild = res[index].rightChild = -1;
			res[index].code = (char *) calloc(sizeof(char),alphaCount);
			huffManIndex[freq[index].ch] = index;
	}
	*huffTab = res;

	return TRUE;
}


int findMinFre(HUFF_TAB *huffTab, int count) {
	int i;
	int MinIndex = -1;

	for (i = 0; i < count; i++) {
		if (!huffTab[i].isVisited && (
			 -1 == MinIndex || huffTab[i].data.freq < huffTab[MinIndex].data.freq)) {
				MinIndex = i;		
		}
	}
	if (-1 != MinIndex) {
		huffTab[MinIndex].isVisited = TRUE;	
	}
	
	return MinIndex;
}

boolean CreatHuffTree(HUFF_TAB *huffTab, int Count) {
	int i = 0;

	int leftChild  = -1;
	int rightChild = -1;

	if (NULL == huffTab || Count < 1) {
		return FALSE;
	}

	for (i = Count; i < 2 * Count - 1; i++) {
		leftChild = findMinFre(huffTab, i);
		rightChild = findMinFre(huffTab, i);

		huffTab[i].leftChild = leftChild;
		huffTab[i].rightChild = rightChild;

		huffTab[i].data.ch = '*';
		huffTab[i].data.freq = huffTab[leftChild].data.freq + huffTab[rightChild].data.freq;	
	}
	
	return TRUE;
}


void ShowFreq(FREQ *freq, int alphaCount) {
	int i;

	for (i = 0; i < alphaCount; i++) {
		if (0 != freq[i].freq) {
			printf("[%x]->%d\n", freq[i].ch, freq[i].freq);
		}	
	}
}

void ShowHuffCode(char *sourceFileName, HUFF_TAB *huffTab, int *huffManIndex) {
	FILE *in = NULL;
	u8 ch;

	in = fopen(sourceFileName, "rb");

	ch = fgetc(in);
	while (!feof(in)) {
		printf("%s ",huffTab[huffManIndex[ch]].code);
		ch = fgetc(in);	
	}

	fclose(in);
}


void ShowHuffTree(HUFF_TAB *huffTab,int alphaCount) {
	int i = 0;

	printf("%-7s%-7s%-7s%-7s%-7s%-7s\n", "ÏÂ±ê","×ÖÄ¸","ÆµÂÊ","×óº¢×Ó","ÓÒº¢×Ó","±àÂë");
	for (i = 0; i < alphaCount; i++) {
		printf("%-7d%-7d%-7d%-7d%-7d%-7s\n", 
				i,
				huffTab[i].data.ch,
				huffTab[i].data.freq,
				huffTab[i].leftChild,
				huffTab[i].rightChild,
				huffTab[i].code == NULL ? "ÎÞ±àÂë" : huffTab[i].code);
	}
}

void DestoryHuffTree(HUFF_TAB **huffTab,int alphaCount) {
	int i = 0;
	HUFF_TAB *tmp = *huffTab;

	for (i = 0; i < alphaCount; i++) {
		free(tmp[i].code);	
	}
	
	free(*huffTab);
	*huffTab = NULL;
}
