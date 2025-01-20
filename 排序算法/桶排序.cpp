#include<stdio.h>
#include<stdlib.h>
#include<iostream>

//extern int hash(void* elt);
//extern int numBuckets(int numElements);

//对于从[0, 1)中均匀选择的数字，下面即hash和numBuckets函数实现的例子：
static int num=0;
//使用的桶数量和元素数量一样
int numBuckets(int numElements)
{
	num = numElements;
	return numElements;
}
//散列函数将元素映射到桶中，因为散列函数的值的范围是[0,1);所以我们将每个桶的大小分为1/num
int hash(void* d)
{
	double* s = reinterpret_cast<double*>(d);
	int bucket = num * (*s);
	return bucket;
}

//桶中的元素链表
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

//桶的指针，以及桶的数量
static BUCKET* buckets = 0;
//static int num = 0;

int cmp(const void* x, const void* y)
{
	int standard;
	//这里比较时也是需要做强制类型转换
	standard = *(int*)x - *(int*)y;

	return standard;
}

//一个接一个移除，并且覆盖ar
void extract(BUCKET* buckets, int(*cmp)(const void*, const void*), void** ar, int n)
{
	int i;
	int low;
	int idx = 0;
	for (i = 0;i < num;i++)
	{
		ENTRY* ptr, * tmp;
		//空桶
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
		//对链表中的元素执行插入排序，然后插入到数组中。然后释放链表
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
		//插入每个元素并且增加计数
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
	//读出并且覆盖ar
	extract(buckets, cmp, ar, n);
	free(buckets);
}

int main() 
{
	int data[] = { 34, 7, 23, 32, 5, 62, 32, 15, 78, 9 };
	int n = sizeof(data) / sizeof(data[0]);
	//在 main 函数内部声明变长指针数组
	//int* dataPtrs[n];
	int**  dataPtrs = (int**)malloc(sizeof(int) * n);

	// 填充数据指针数组  
	for (int i = 0; i < n; i++) {
		dataPtrs[i] = &data[i];
	}
	void** datas= reinterpret_cast<void**>(dataPtrs);
	// 排序  
	sortPointers(datas, n, cmp);

	// 打印排序后的数组  
	printf("Sorted array:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", *dataPtrs[i]);
	}
	printf("\n");

	return 0;
}


//使用的桶数量。
int  numBuckets(int  numElements)
{
	return  26 * 26 * 26;
}

	
//散列函数计算出每个元素应映射到的桶。
int  hash(void* elt)
{
	return  (((char*)elt)[o] - 'a') * 676 +(((char*)elt)[1] - 'a') * 26 +(((char*)elt)[2] - 'a');
}
