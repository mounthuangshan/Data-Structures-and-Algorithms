#include<stdio.h>

// 二分查找函数，返回目标值的索引，如果未找到则返回-1  
int binarySearch(int arr[], int n, int target) 
{
    int left = 0;
    int right = n - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2; // 防止(left + right)可能导致的溢出  

        // 检查中间元素是否是目标值  
        if (arr[mid] == target)
        {
            return mid; // 找到目标值，返回索引  
        }

        // 如果目标值大于中间元素，则忽略左半部分  
        if (arr[mid] < target) 
        {
            left = mid + 1;
        }
        else 
        { // 如果目标值小于中间元素，则忽略右半部分  
            right = mid - 1;
        }
    }

    // 如果未找到目标值，则返回-1  
    return -1;
}

int main()
{
    int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 7;

    // 调用二分查找函数  
    int result = binarySearch(arr, n, target);

    if (result != -1) 
    {
        printf("目标值 %d 在数组中的索引为 %d\n", target, result);
    }
    else 
    {
        printf("目标值 %d 未在数组中找到\n", target);
    }

    return 0;
}

