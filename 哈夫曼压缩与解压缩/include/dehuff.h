#ifndef _DEHUFF_H_
#define _DEHUFF_H_ 

#include "huffman.h"

HUFFMAN_HEAD ReadHuffmanHead(char *sourceFileName);
FREQ *ReadFreq(char *file, int alphaCount);
boolean IsHuffmanHead(HUFFMAN_HEAD head);
void Decoding(char *sourceFileName, char *targetFileName,
			 HUFF_TAB *huffTab, HUFFMAN_HEAD *head);

#endif
