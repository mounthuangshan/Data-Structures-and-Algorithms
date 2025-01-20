#include<stdio.h>
#include<stdlib.h>
#include<iostream>

//extern int hash(void* elt);
//extern int numBuckets(int numElements);

//���ڴ�[0, 1)�о���ѡ������֣����漴hash��numBuckets����ʵ�ֵ����ӣ�
static int num=0;
//ʹ�õ�Ͱ������Ԫ������һ��
int numBuckets(int numElements)
{
	num = numElements;
	return numElements;
}
//ɢ�к�����Ԫ��ӳ�䵽Ͱ�У���Ϊɢ�к�����ֵ�ķ�Χ��[0,1);�������ǽ�ÿ��Ͱ�Ĵ�С��Ϊ1/num
int hash(void* d)
{
	double* s = reinterpret_cast<double*>(d);
	int bucket = num * (*s);
	return bucket;
}

//Ͱ�е�Ԫ������
typedef struct entry
{
	void* element;
	struct entry* next;
}ENTRY;

typedef struct 
{
	int size;
	ENTRY* head;
}BUCKET;

//Ͱ��ָ�룬�Լ�Ͱ������
static BUCKET* buckets = 0;
//static int num = 0;

int cmp(const void* x, const void* y)
{
	int standard;
	//����Ƚ�ʱҲ����Ҫ��ǿ������ת��
	standard = *(int*)x - *(int*)y;

	return standard;
}

//һ����һ���Ƴ������Ҹ���ar
void extract(BUCKET* buckets, int(*cmp)(const void*, const void*), void** ar, int n)
{
	int i;
	int low;
	int idx = 0;
	for (i = 0;i < num;i++)
	{
		ENTRY* ptr, * tmp;
		//��Ͱ
		if (buckets[i].size == 0)
		{
			continue;
		}
		ptr = buckets[i].head;
		if (buckets[i].size == 1)
		{
			ar[idx++] = ptr->element;
			free(ptr);
			buckets[i].size = 0;
			continue;
		}
		//�������е�Ԫ��ִ�в�������Ȼ����뵽�����С�Ȼ���ͷ�����
		low = idx;
		ar[idx++] = ptr->element;
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
		while (ptr != NULL)
		{
			int i = idx - 1;
			while (i >= low && cmp(ar[i], ptr->element) > 0)
			{
				ar[i + 1] = ar[i];
				i--;
			}
			ar[i + 1] = ptr->element;
			tmp = ptr;
			ptr = ptr->next;
			free(tmp);
			idx++;
		}
		buckets[i].size = 0;
	}
}

void sortPointers(void** ar, int n, int(*cmp)(const void*, const void*))
{
	int i;
	num = numBuckets(n);
	buckets = (BUCKET*)calloc(num, sizeof(BUCKET));
	for (i = 0;i < n;i++)
	{
		int k = hash(ar[i]);
		//����ÿ��Ԫ�ز������Ӽ���
		ENTRY* e = (ENTRY*)calloc(1, sizeof(ENTRY));
		e->element = ar[i];
		if (buckets[k].head == NULL)
		{
			buckets[k].head = e;
		}
		else
		{
			e->next = buckets[k].head;
			buckets[k].head = e;
		}
		buckets[k].size++;
	}
	//�������Ҹ���ar
	extract(buckets, cmp, ar, n);
	free(buckets);
}

int main() 
{
	int data[] = { 34, 7, 23, 32, 5, 62, 32, 15, 78, 9 };
	int n = sizeof(data) / sizeof(data[0]);
	//�� main �����ڲ������䳤ָ������
	//int* dataPtrs[n];
	int**  dataPtrs = (int**)malloc(sizeof(int) * n);

	// �������ָ������  
	for (int i = 0; i < n; i++) {
		dataPtrs[i] = &data[i];
	}
	void** datas= reinterpret_cast<void**>(dataPtrs);
	// ����  
	sortPointers(datas, n, cmp);

	// ��ӡ����������  
	printf("Sorted array:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", *dataPtrs[i]);
	}
	printf("\n");

	return 0;
}


//ʹ�õ�Ͱ������
int  numBuckets(int  numElements)
{
	return  26 * 26 * 26;
}

	
//ɢ�к��������ÿ��Ԫ��Ӧӳ�䵽��Ͱ��
int  hash(void* elt)
{
	return  (((char*)elt)[o] - 'a') * 676 +(((char*)elt)[1] - 'a') * 26 +(((char*)elt)[2] - 'a');
}
