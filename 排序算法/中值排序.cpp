#include<stdio.h>

/*
在ar中寻找第k大元素的位置的算法,其在平均情况下为线性时间。
A随着计算的进行不断被修改更新。注意1kright-left+1。
比较函数cmp将会严格地比较元素。最坏情况下性能为二次方,即o(n2)。
*/

int cmp(const void* x, const void* y)
{
	int standard;
	//这里比较时也是需要做强制类型转换
	standard = *(int*)x - *(int*)y;

	return standard;
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

int selectKth(void** ar, int(*cmp)(const void*, const void*), int k, int left, int right)
{
	//三数取中法找出中值
	int idx = selectpivotIndex(ar, left, right);
	//进行排序，返回排序好的数组的中间值位置
	int pivotIndex = partition(ar, cmp, left, right, idx);
	//这里的比较很巧妙，排序好的数组的中间值的位置就是第几大，通过这种方法无需将数组完全排序完成就能找到第k大的元素
	if (left + k - 1 == pivotIndex)
	{
		return pivotIndex;
	}
	//继续循环，并缩小范围，如果我们在中值的左边，那么k将不变。
	if (left + k - 1 < pivotIndex)
	{
		return selectKth(ar, cmp, k, left, pivotIndex - 1);
	}
	else
	{
		return selectKth(ar, cmp, k - (pivotIndex - left + 1), pivotIndex + 1, right);
	}

}

//使用medianSort方法排序ar[left,right]。比较函数cmp将会严格的比较元素。
void mediansort(void** ar, int(*cmp)(const void*, const void*), int left, int right)
{
	//如果待排序的子数组只有一个（或者更少）元素，返回。
	if (right <= left)
	{
		return;
	}
	//得到中点和中值元素位置k，(1<=k<=right-left-1)
	int mid = (right - left + 1) / 2;
	int me = selectKth(ar, cmp, mid + 1, left, right);
	mediansort(ar, cmp, left, left + mid - 1);
	mediansort(ar, cmp, left + mid + 1, right);
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
	int z = selectKth(base, cmp, 5, 0, n - 1);
	mediansort(base, cmp, 0, n - 1);
	for (i = 0;i < 10;i++)
	{
		//printf("%d",*num[i]);这是错误的打印void数组方法，下面是正确的：
		// 将void*转换为int*并解引用  
		int* intPtr = (int*)base[i];
		printf("base[%d] = %d\n", i, *intPtr);
	}
	printf("中值为base[%d] = % d\n", z, *(int*)base[z]);
	return 0;
}