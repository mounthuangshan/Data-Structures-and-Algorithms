#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//基于值的插入排序
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

void insertionSortPointers(void* base, int n, int s, int(*cmp)(const void*, const void*))
{
	int j;
	//临时保存一个元素
	void* saved = malloc(s);
	for (j = 1;j < n;j++)
	{
		//从末尾开始逆向遍历,直到i<0或者找到一个相比较小的元素
		int i = j - 1;
		void* value = (char*)base + j * s;
		while (i >= 0 && cmp((char*)base + i * s, value) > 0)
		{
			i--;
		}
		//如果已经在其正确的位置了，那么就没有必要移动，否则保存将要插入的值并且移动这一大块内存，将其插入适当的位置。
		if (++i == j)
		{
			continue;
		}
		//保存要插入的值
		memmove(saved, value, s);
		//将后面的元素向后移动一个位置
		memmove((char*)base + (i + 1) * s, (char*)base + i * s, s * (j - i));
		//将保存的值插入到正确的位置
		memmove((char*)base + i * s, saved, s);
		
	}
	free(saved);  // 释放临时分配的内存
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
	int base[10] = { a,b,c,d,e,f,g,h,i,j };
	int n = sizeof(base) / sizeof(base[0]);
	insertionSortPointers(base,n,sizeof(base[0]), cmp);
	for (i = 0;i < 10;i++)
	{
		//printf("%d",*num[i]);这是错误的打印void数组方法，下面是正确的：
		//将void*转换为int*并解引用  
		int intPtr = (int)base[i];
		printf("base[%d] = %d\n", i, intPtr);
	}
	return 0;
	
}

/*
为什么为什么上面的base要转换强制转换为char类型：
在C语言中，将base指针转换为char*类型（即字符指针）是为了能够正确地按照字节偏移来访问数组中的元素。这是因为char类型
在C中定义为占用1个字节（byte），因此使用char*作为指针类型可以确保每次偏移都是按照1个字节来进行的。
在insertionSortPointers函数中，base是一个void*类型的指针，它指向数组的首元素。由于void*是一个通用指针类型，它不能
直接用于指针算术运算（如加法或减法），因为编译器不知道void*指向的数据类型的大小。因此，为了能够根据元素的大小s来正确
地计算数组中每个元素的地址，我们需要将base转换为一个已知大小的指针类型。
选择char*作为转换目标有几个原因：
标准定义：char类型在C标准中被定义为占用1个字节，这使得char*成为进行字节级操作的理想选择。
跨平台兼容性：使用char*进行指针算术可以确保在不同平台上的一致性，因为char的大小在所有平台上都是相同的（即1个字节）。
易于理解：将base转换为char*并进行偏移计算，可以清晰地表明我们是在进行字节级的操作，而不是基于某种特定数据类型的大
小。
在insertionSortPointers函数中，通过将base转换为char*类型的指针baseChar，我们可以使用baseChar + j * s来计算数组
中第j个元素的地址（假设数组元素的大小为s字节）。这样，无论数组中的元素是什么类型，我们都可以正确地计算出它们的地址，
并进行后续的插入排序操作。
最后，需要注意的是，在将base转换为char*之后，我们在比较函数cmp的调用中仍然需要将指针转换为const void*类型，这是因
为cmp函数的参数类型是const void*。这是为了确保类型安全，并允许cmp函数接受任何类型的指针作为参数。
*/