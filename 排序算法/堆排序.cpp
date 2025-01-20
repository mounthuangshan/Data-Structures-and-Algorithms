#include<stdio.h>
#include<stdlib.h>

int cmp(const void* x, const void* y)
{
	int standard;
	//����Ƚ�ʱҲ����Ҫ��ǿ������ת��
	standard = *(int*)x - *(int*)y;

	return standard;
}

//��������ŵ�������heapify��������Ҫʹ��������ͬ�Ŀռ䣬��Ȼ����Ϊһ����ͬ��Ŀ��
static void heapify(void** ar, int(const void*, const void*), int idx, int max)
{
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;
	int largest;
	//��A[idx].A[left]��A[right]Ѱ������Ԫ��
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

	//������Ĳ��Ǹ��ڵ㣬��ô�������ݹ�ִ��
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
	// ����һ����������  
	int arr[] = { 5, 7, 4, 6, 3, 8, 1, 0, 9, 2 };
	int n = sizeof(arr) / sizeof(arr[0]);

	// ����һ��ָ��������ָ������  
	void** ptrArray = (void**)malloc(n * sizeof(void*));
	for (int i = 0; i < n; i++) {
		ptrArray[i] = (void*)&arr[i];
	}

	// ����ָ������  
	sortPointers(ptrArray, n, cmp);

	// ��ӡ�����Ľ��  
	printf("Sorted array:\n");
	for (int i = 0; i < n; i++) {
		printf("%d ", *(int*)ptrArray[i]);
	}
	printf("\n");

	// �ͷ��ڴ�  
	free(ptrArray);

	return 0;
}
