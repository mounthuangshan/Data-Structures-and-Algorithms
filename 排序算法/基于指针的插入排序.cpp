#include<stdio.h>
#include<stdlib.h>

//基于指针的插入排序
int cmp(const void* x, const void* y)
{
	int standard;
	//这里比较时也是需要做强制类型转换
	standard = *(int*)x - *(int*)y;

	return standard;
}
/*cmp是一个指向比较函数的指针，该函数接收两个 const void* 类型的参数，并返回一个整数。
如果第一个参数应该排在第二个参数之前，则返回负数；如果两者相等，则返回零；
如果第一个参数应该排在第二个参数之后，则返回正数。*/
void insertionSortPointers(void** ar, int n, int(*cmp)(const void*, const void*))
{
	int j;
	for (j = 1;j < n;j++)
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
	/*下面是这个循环的过程：
	1.初始化循环变量j为1，这意味着我们从数组的第二个元素开始（因为假设第一个元素自成一个有序序列），一直到数组的最后一个
	元素。
	2.对于每个j（即数组的当前元素），我们首先将它的索引j-1赋值给变量i，并将该元素的值（通过指针value）保存起来。
	这里i用于在已排序的部分（即数组的前j-1个元素）中从后向前遍历，寻找当前元素应该插入的位置。
	3.进入一个内部while循环，条件是i大于等于0，并且cmp(ar[i], value)的结果大于0。这意味着只要i没有越界，并且已排序部分
	的当前元素（ar[i]）大于要插入的元素（value），就继续执行循环体。
	4.在循环体内，将已排序部分的当前元素（ar[i]）向后移动一个位置（即赋值给ar[i+1]），为要插入的元素腾出空间。
	5.将i减1，继续检查前一个元素。
	6.当while循环结束时，说明已经找到了要插入元素的位置（即i+1的位置），或者已经遍历完了已排序的部分（此时i小于0）。
	7.将保存的值（value）插入到找到的位置（ar[i+1]）。
	8.外部循环变量j加1，继续处理数组的下一个元素。
	9.当外部循环结束时，整个数组ar就被排序好了。
	这个过程实际上是插入排序的核心，它通过构建有序序列，对于未排序数据，在已排序序列中从后向前扫描，找到相应位置并插入。
	插入排序在实现上，通常采用in-place排序（即只需用到O(1)的额外空间的排序），因而在从后向前扫描过程中，找到相应位置并
	插入时，不需要移动其它元素，只需将要插入的元素移动到新位置，其它元素相应后移一位。*/
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
	/*使用int* num[10]这样的数组作为参数传入insertionSortPointers中在C语言的编译器中是可以通过编译的，
	这就是void* 的泛型用法，由于void* 无法在C++中执行隐式转换，所以如果是传入void **ar这样的参数中则需要使用void* num[10]这样的数组，
	否则会报实参与形参不兼容的问题，因为在C++中有更加严格的类型安全检查，这是语言特性的问题*/
	void* num[10] = { &a,&b,&c,&d,&e,&f ,&g ,&h ,&i ,&j };
	int n = sizeof(num) / sizeof(num[0]);
	insertionSortPointers(num, n, cmp);
	for (i = 0;i < 10;i++)
	{
		//printf("%d",*num[i]);这是错误的打印void数组方法，下面是正确的：
		// 将void*转换为int*并解引用  
		int* intPtr = (int*)num[i];
		printf("num[%d] = %d\n", i, *intPtr);
	}
	return 0;
}