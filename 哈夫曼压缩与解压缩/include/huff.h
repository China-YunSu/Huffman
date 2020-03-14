#ifndef _HUFF_H_
#define _HUFF_H_ 

#include "huffman.h"

void InitHuffmanHead(HUFFMAN_HEAD *head, HUFF_TAB *huffTab, int alphaCount);
void ProductU8Code(char *sourceFileName, char *targetFileName, HUFF_TAB *huffTab,
						int *huffManIndex, FREQ *freq, int alphaCount);
FREQ *GetFreq(char *file, int *alphaCount);

#endif