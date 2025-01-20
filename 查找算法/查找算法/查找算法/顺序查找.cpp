#include<stdio.h>
#include<stdlib.h>

int cmp(const void* x, const void* y)
{
    int standard;
    //����Ƚ�ʱҲ����Ҫ��ǿ������ת��
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
    /*ʹ��int* num[10]������������Ϊ��������insertionSortPointers����C���Եı��������ǿ���ͨ������ģ�
    �����void* �ķ����÷�������void* �޷���C++��ִ����ʽת������������Ǵ���void **ar�����Ĳ���������Ҫʹ��void* num[10]���������飬
    ����ᱨʵ�����ββ����ݵ����⣬��Ϊ��C++���и����ϸ�����Ͱ�ȫ��飬�����������Ե�����*/
    void* arr[10] = { &a,&b,&c,&d,&e,&f ,&g ,&h ,&i ,&j };
    int n = sizeof(arr)/sizeof(arr[0]);
    int target = 6;

    // ����Ŀ��ֵ  
    if (search(arr, &target, n, cmp)) {
        printf("Target %d found in the array.\n", target);
    }
    else {
        printf("Target %d not found in the array.\n", target);
    }

    return 0;
}