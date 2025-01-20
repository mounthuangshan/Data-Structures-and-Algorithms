#include<stdio.h>

/*在线性时间, 根据给定的中枢值重新组合子序列ar[left, right],
将中枢值存储在其正确的位置store, 确保所有在子序列ar[left, store)中的
元素 <= pivot并且所有在ar[store + 1, right]中的元素 > pivot。*/

int cmp(const void* x, const void* y)
{
	int standard;
	//这里比较时也是需要做强制类型转换
	standard = *(int*)x - *(int*)y;

	return standard;
}

int partition(void **ar,int(*cmp)(const void *,const void *),int left,int right,int pivotIndex)
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
		if(cmp(ar[idx],pivot)<=0)
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
		//printf("%d",*num[i]);这是错误的打印void数组方法，下面是正确的：
		//将void*转换为int*并解引用  
		int* intPtr = (int*)base[i];
		printf("base[%d] = %d\n", i, *intPtr);
	}
	return 0;
}

/*
我们是如何通过使用partition更加高效地选择中值呢?首先,让我们看看这个方法的
结果,以一个16个元素的无序数组为例。首先第一步是将中枢值和最右边的元素交换。
在partition每一次执行循环的时候,关键的变量如图4-10所示。store这个值用来区分
执行的是哪一次循环。图4-10中的后继行表示了每一次执行partition的循环都挑选出
来了一个A[idx],这个元素小于或者等于中枢值(在这里是元素“06”)。一旦不再有
元素小于或者等于中枢值,store这个位置的元素将会和最右边的元素进行交换,这样
安全地将中枢值放到了适当的位置。
*/