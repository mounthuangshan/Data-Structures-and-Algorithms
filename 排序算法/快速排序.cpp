#include<stdio.h>
#include<stdlib.h>

//使用插入排序的数组大小的阈值
#define MINSIZE 4

int cmp(const void* x, const void* y)
{
	int standard;
	//这里比较时也是需要做强制类型转换
	standard = *(int*)x - *(int*)y;

	return standard;
}

int selectpivotIndex(void** ar, int left, int right)
{
	
	// 计算中间位置的索引
	int mid = (left + right) / 2;

	// 获取左端、中间和右端的值
	int left_val = *((int*)ar[left]);
	int mid_val = *((int*)ar[mid]);
	int right_val = *((int*)ar[right]);

	// 找出这三个值的中值
	if ((left_val < mid_val && mid_val < right_val) || (right_val < mid_val && mid_val < left_val))
	{
		return mid;
	}
	else if ((mid_val < left_val && left_val < right_val) || (right_val < left_val && left_val < mid_val))
	{
		return left;
	}
	else
	{
		return right;
	}

}

int partition(void** ar, int(*cmp)(const void*, const void*), int left, int right, int pivotIndex)
{
	int idx;
	int store;
	void* pivot = ar[pivotIndex];
	//将中枢值移动到数组结尾
	void* tmp = ar[right];
	ar[right] = ar[pivotIndex];
	ar[pivotIndex] = tmp;
	//将所有小于等于中枢值的元素都会移动到数组的前面，然后将中枢值插在它们后面
	store = left;
	for (idx = left;idx < right;idx++)
	{
		if (cmp(ar[idx], pivot) <= 0)
		{
			tmp = ar[idx];
			ar[idx] = ar[store];
			ar[store] = tmp;
			store++;
		}
	}
	//又一次交换，中枢值插在后面
	tmp = ar[right];
	ar[right] = ar[store];
	ar[store] = tmp;
	//返回中值的位置
	return store;
}

//之前实现的基于指针的插入排序函数，但由于左值不再确定为0，因此有所改动
void insertionSortPointers(void** ar,int(*cmp)(const void*, const void*),int m,int n)
{
	int j;
	for (j = m+1;j <= n;j++)
	{
		int i = j - 1;
		void* value = ar[j];
		while (i >= 0 && cmp(ar[i], value) > 0)
		{
			ar[i + 1] = ar[i];
			i--;
		}
		ar[i + 1] = value;
	}
}

//使用快速排序来对ar[left,right]排序，比较函数cmp用来严格的排序元素
void do_qsort(void** ar, int(*cmp)(const void* ,const void*), int left, int right)
{
	int pivotIndex;
	if (right <= left)
	{
		return;
	}
	//切分
	pivotIndex = selectpivotIndex(ar, left, right);
	pivotIndex = partition(ar, cmp, left, right, pivotIndex);
	if (pivotIndex - 1 - left <= MINSIZE)
	{
		insertionSortPointers(ar, cmp, left, pivotIndex - 1);
	}
	else
	{
		do_qsort(ar, cmp, left, pivotIndex-1);
	}
	if(right-pivotIndex-1<=MINSIZE)
	{
		insertionSortPointers(ar, cmp,pivotIndex +1,right);
	}
	else
	{
		do_qsort(ar, cmp,pivotIndex + 1,right);
	}
}

//直接快速排序
void sortPointers(void** vals, int total_elems, int(*cmp)(const void*, const void*))
{
	do_qsort(vals, cmp, 0, total_elems - 1);
}

// 打印整数数组（通过指针数组）  
void printArray(void** ar, int total_elems) {
	for (int i = 0; i < total_elems; i++) {
		printf("%d ", *((int*)ar[i]));
	}
	printf("\n");
}

int main() {
	//int nums[] = { 34, 7, 23, 32, 5, 62, 32, 19, 45, 2, 90, 35 };
	int nums[] = { 5, 7, 4, 6, 3, 8, 1, 0, 9, 2};
	int total_elems = sizeof(nums) / sizeof(nums[0]);

	// 创建指针数组  
	void** vals = (void**)malloc(total_elems * sizeof(void*));
	for (int i = 0; i < total_elems; i++) {
		vals[i] = (void*)&nums[i];
	}

	// 排序  
	sortPointers(vals, total_elems, cmp);

	// 打印排序后的数组  
	printArray(vals, total_elems);

	// 释放内存  
	free(vals);

	return 0;
}