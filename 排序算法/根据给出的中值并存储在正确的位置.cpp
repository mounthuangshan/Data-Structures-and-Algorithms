#include<stdio.h>

/*������ʱ��, ���ݸ���������ֵ�������������ar[left, right],
������ֵ�洢������ȷ��λ��store, ȷ��������������ar[left, store)�е�
Ԫ�� <= pivot����������ar[store + 1, right]�е�Ԫ�� > pivot��*/

int cmp(const void* x, const void* y)
{
	int standard;
	//����Ƚ�ʱҲ����Ҫ��ǿ������ת��
	standard = *(int*)x - *(int*)y;

	return standard;
}

int partition(void **ar,int(*cmp)(const void *,const void *),int left,int right,int pivotIndex)
{
	int idx;
	int store;
	void* pivot = ar[pivotIndex];
	//������ֵ�ƶ��������β
	void* tmp = ar[right];
	ar[right] = ar[pivotIndex];
	ar[pivotIndex] = tmp;
	//������С�ڵ�������ֵ��Ԫ�ض����ƶ��������ǰ�棬Ȼ������ֵ�������Ǻ���
	store = left;
	for (idx = left;idx < right;idx++)
	{
		if(cmp(ar[idx],pivot)<=0)
		{
			tmp = ar[idx];
			ar[idx] = ar[store];
			ar[store] = tmp;
			store++;
		}
	}
	//��һ�ν���������ֵ���ں���
	tmp = ar[right];
	ar[right] = ar[store];
	ar[store] = tmp;
	return store;
}

int main()
{
	int a = 5;
	int b = 7;
	int c = 4;
	int d = 6;
	int e = 3;
	int f = 8;
	int g = 1;
	int h = 0;
	int i = 9;
	int j = 2;
	void* base[10] = { &a,&b,&c,&d,&e,&f,&g,&h,&i,&j };
	int n = sizeof(base) / sizeof(base[0]);
	partition(base,cmp,0,n-1,5);
	for (i = 0;i < 10;i++)
	{
		//printf("%d",*num[i]);���Ǵ���Ĵ�ӡvoid���鷽������������ȷ�ģ�
		//��void*ת��Ϊint*��������  
		int* intPtr = (int*)base[i];
		printf("base[%d] = %d\n", i, *intPtr);
	}
	return 0;
}

/*
���������ͨ��ʹ��partition���Ӹ�Ч��ѡ����ֵ��?����,�����ǿ������������
���,��һ��16��Ԫ�ص���������Ϊ�������ȵ�һ���ǽ�����ֵ�����ұߵ�Ԫ�ؽ�����
��partitionÿһ��ִ��ѭ����ʱ��,�ؼ��ı�����ͼ4-10��ʾ��store���ֵ��������
ִ�е�����һ��ѭ����ͼ4-10�еĺ���б�ʾ��ÿһ��ִ��partition��ѭ������ѡ��
����һ��A[idx],���Ԫ��С�ڻ��ߵ�������ֵ(��������Ԫ�ء�06��)��һ��������
Ԫ��С�ڻ��ߵ�������ֵ,store���λ�õ�Ԫ�ؽ�������ұߵ�Ԫ�ؽ��н���,����
��ȫ�ؽ�����ֵ�ŵ����ʵ���λ�á�
*/