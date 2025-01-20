#include<stdio.h>
#include<stdlib.h>

int cmp(const void* x, const void* y)
{
    int standard;
    //这里比较时也是需要做强制类型转换
    standard = *(int*)x - *(int*)y;

    return standard;
}

int search(void** ar, void* t, int n, int(*cmp)(const void*, const void*))
{
    int i;
    for (i = 0;i < n;i++)
    {
        if (!cmp(ar[i], t))
        {
            return 1;
        }
    }
    return 0;
}


int main() {
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
    void* arr[10] = { &a,&b,&c,&d,&e,&f ,&g ,&h ,&i ,&j };
    int n = sizeof(arr)/sizeof(arr[0]);
    int target = 6;

    // 搜索目标值  
    if (search(arr, &target, n, cmp)) {
        printf("Target %d found in the array.\n", target);
    }
    else {
        printf("Target %d not found in the array.\n", target);
    }

    return 0;
}