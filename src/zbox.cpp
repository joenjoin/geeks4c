#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ZBoxMatch(char* pSrc, int nSrcSize, char* pSubSrc, int nSubSrcSize) {
	int* pZBox = new int[nSrcSize];
	pZBox[0] = 1;
	int left = 0, right = 0;
	for (int i = 0; i < nSrcSize; i++) {
		if (i > right) {
			//ZBox之外,更新ZBox的区间
			int j = 0;
			while (j + i < nSrcSize && pSubSrc[j] == pSrc[i + j]) {
				j++;
			}
			left = i;
			right = i + j - 1;
			pZBox[i] = j;
		} else {
			//ZBox之内
			int p = i - left;
			if (pZBox[p] < right - i + 1) {
				//p的最长公共前缀匹配不超过right与i的宽度
				pZBox[i] = pZBox[p];
			} else {
				//否则接着right 向后继续计算匹配的位置
				int j = right + 1;
				while (j < nSrcSize && pSubSrc[j] == pSrc[j - i]) {
					j++;
				}
				left = i;
				right = j - 1;
				pZBox[i] = j - i;
			}
		}
	}

	for (int i = 0; i < nSrcSize; i++)
		printf("%d ", pZBox[i]);

	printf("\n");

	int nPos = -1;
	for (int i = 0; i < nSrcSize; i++) {
		if (pZBox[i] == nSubSrcSize) {
			nPos = i;
			printf("%d \n", nPos);
			//break;
		}
	}
	delete[] pZBox;
	return nPos;
}

//int main() {
//	char* pSrc = "CABCABABAB";
//	char* pSubSrc = "ABA";
//	int matched = ZBoxMatch(pSrc, strlen(pSrc), pSubSrc, strlen(pSubSrc));
//
//	printf("%d\n", matched);
//
//	return 0;
//}
