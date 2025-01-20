#include<stdio.h>
#include<stdlib.h>

int cmp(const void* x, const void* y)
{
	int standard;
	//这里比较时也是需要做强制类型转换
	standard = *(int*)x - *(int*)y;

	return standard;
}

//堆排序的优点在于其heapify函数，它要使用两个不同的空间，虽然都是为一个共同的目的
static void heapify(void** ar, int(const void*, const void*), int idx, int max)
{
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	int largest;
	//在A[idx].A[left]和A[right]寻找最大的元素
	if (left < max && cmp(ar[left], ar[idx])>0)
	{
		largest = left;
	}
	else
	{
		largest = idx;
	}
	if (right < max && cmp(ar[right], ar[largest])>0)
	{
		largest = right;
	}

	//如果最大的不是父节点，那么交换并递归执行
	if (largest != idx)
	{
		void* tmp;
		tmp = ar[idx];
		ar[idx] = ar[largest];
		ar[largest] = tmp;
		heapify(ar, cmp, largest, max);
	}
}

static void buildHeap(void** ar, int(*cmp)(const void*, const void*), int n)
{
	int i;
	for (i = n / 2 - 1;i >= 0;i--)
	{
		heapify(ar, cmp, i, n);
	}
}

void sortPointers(void** ar, int n, int(*cmp)(const void*, const void*))
{
	int i;
	buildHeap(ar, cmp, n);
	for (i = n - 1;i >= 1;i--)
	{
		void* tmp;
		tmp = ar[0];
		ar[0] = ar[i];
		ar[i] = tmp;
		heapify(ar, cmp, 0, i);
	}
}

int main() {
	// 创建一个整数数组  
	int arr[] = { 5, 7, 4, 6, 3, 8, 1, 0, 9, 2 };
	int n = sizeof(arr) / sizeof(arr[0]);

	// 创建一个指向整数的指针数组  
	void** ptrArray = (void**)malloc(n * sizeof(void*));
	for (int i = 0; i < n; i++) {
		ptrArray[i] = (void*)&arr[i];
	}

	// 排序指针数组  
	sortPointers(ptrArray, n, cmp);

	// 打印排序后的结果  
	printf("Sorted array:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", *(int*)ptrArray[i]);
	}
	printf("\n");

	// 释放内存  
	free(ptrArray);

	return 0;
}
