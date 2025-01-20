#include<stdio.h>

// ���ֲ��Һ���������Ŀ��ֵ�����������δ�ҵ��򷵻�-1  
int binarySearch(int arr[], int n, int target) 
{
    int left = 0;
    int right = n - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2; // ��ֹ(left + right)���ܵ��µ����  

        // ����м�Ԫ���Ƿ���Ŀ��ֵ  
        if (arr[mid] == target)
        {
            return mid; // �ҵ�Ŀ��ֵ����������  
        }

        // ���Ŀ��ֵ�����м�Ԫ�أ��������벿��  
        if (arr[mid] < target) 
        {
            left = mid + 1;
        }
        else 
        { // ���Ŀ��ֵС���м�Ԫ�أ�������Ұ벿��  
            right = mid - 1;
        }
    }

    // ���δ�ҵ�Ŀ��ֵ���򷵻�-1  
    return -1;
}

int main()
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 7;

    // ���ö��ֲ��Һ���  
    int result = binarySearch(arr, n, target);

    if (result != -1) 
    {
        printf("Ŀ��ֵ %d �������е�����Ϊ %d\n", target, result);
    }
    else 
    {
        printf("Ŀ��ֵ %d δ���������ҵ�\n", target);
    }

    return 0;
}

