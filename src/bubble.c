#include <stdio.h>
void Sort_bubble(int* pData,int Count)
#if 0
{
	int iTemp;
	for(int i=1;i<Count;i++)	{
		for(int j=Count-1;j>=i;j--)	{
			if(pData[j]<pData[j-1])	{
				iTemp = pData[j-1];
				pData[j-1] = pData[j];
				pData[j] = iTemp;
			}
		}
	}
}
#else
{
	int iTemp;
	for(int i = 0; i<Count;i++)	{
		for(int j = 0; j < Count-1;j++)	{
			if (pData[j]>pData[j+1])	{
				iTemp = pData[j+1];
				pData[j+1] = pData[j];
				pData[j] = iTemp;
			}
		}
	}
}
#endif
