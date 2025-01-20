#include<stdio.h>
#include<stdlib.h>

#define SWAP(a,p1,p2,type){\
	type tmp_=a[p1];\
    a[p1]=a[p2];\
    a[p2] = tmp_;\
}

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

//寻找在数组ar[left]、ar[left + gap]、ar[left + gap * 2]、ar[left + gap * 3]的4个元素的中值, 在结束的时候确保ar[left + gap * 2]包含有中值。
//原书代码：
/*
static void medianOffour(void** ar, int left, int gap, int(*cmp)(const void*, const void*))
{
    int pos1 = left;
    int pos2;
    int pos3;
    int pos4;
    void* a1 = ar[pos1];
    void* a2 = ar[pos2 = pos1 + gap];
    void* a3 = ar[pos3 = pos2 + gap];
    void* a4 = ar[pos4 = pos3 + gap];
    if (cmp(a1, a2) <= 0)
    {
        if (cmp(a2, a3) <= 0)
        {
            if (cmp(a3, a4) > 0)
            {
                SWAP(ar, pos3, pos4, void *);
            }
            else
            {
                SWAP(ar, pos2, pos3, void*);
            }
        }
        else
        {
            if (cmp(a1, a4) <= 0)
            {
                if (cmp(a2, a4) <= 0)
                {
                    SWAP(ar, pos2, pos3, void*);
                }
                else
                {
                    SWAP(ar, pos3, pos4, void*);
                }
            }
            else
            {
                if (cmp(a1, a3) <= 0)
                {
                    if (cmp(a3, a4) <= 0)
                    {
                        if (cmp(a2, a4) <= 0)
                        {
                            SWAP(ar, pos2, pos3, void*);
                        }
                        else
                        {
                            SWAP(ar, pos3, pos4, void*);
                        }
                    }
                }
                else
                {
                    if (cmp(a1, a4) <= 0)
                    {
                        if (cmp(a2, a4) <= 0)
                        {
                            SWAP(ar, pos2, pos3, void*);
                        }
                        else
                        {
                            SWAP(ar, pos1, pos3, void*);
                        }
                    }
                    else
                    {
                        SWAP(ar, pos1, pos3, void*);
                    }
                }
            }
        }
    }
    else
    {
        if (cmp(a1, a3) <= 0)
        {
            if (cmp(a1, a4) <= 0)
            {
                if (cmp(a3, a4) > 0)
                {
                    SWAP(ar, pos3, pos4, void*);
                }
            }
            else
            {
                ;
            }
        }
        else
        {
            if (cmp(a2, a3) <= 0)
            {
                if (cmp(a3, a4) <= 0)
                {
                    if (cmp(a1, a4) <= 0)
                    {
                        SWAP(ar, pos1, pos3, void*);
                    }
                    else 
                    {
                        SWAP(ar, pos3, pos4, void*);
                    }
                }
                else 
                {
                    SWAP(ar, pos2, pos3, void*);
                }
            }
        }
    }  
}
*/

//优化后的代码：
/*
优化前的代码则采用了更加复杂的逻辑来减少不必要的比较和交换。它首先通过一系列的比较来确定元素之间的相对顺序，
然后根据这些比较的结果来决定是否进行交换。这种方法的优点是它试图通过最少的比较和交换次数来达到排序的目的；优化后的代码逻辑相对直接，
但包含了多个嵌套的条件语句，这可能导致在某些情况下性能不是最优的，因为即使已经确定了元素的相对顺序，代码仍然会继续执行不必要的比较。
两段代码在性能上可能存在差异，但这种差异可能并不总是显著的。在选择使用哪段代码时，需要权衡代码的可读性、可维护性和实际性能需求。
如果性能是关键因素，并且可以通过减少比较和交换次数来显著提高性能，那么可以考虑使用优化前的代码（或对其进行进一步的优化）。
然而，如果代码的可读性和可维护性更加重要，那么优化后的代码可能是一个更好的选择。
*/
static void medianOffour(void** ar, int left, int gap, int (*cmp)(const void*, const void*))
{
    int pos1 = left;
    int pos2 = pos1 + gap;
    int pos3 = pos2 + gap;
    int pos4 = pos3 + gap;

    // Compare and swap adjacent elements to sort in ascending order
    if (cmp(ar[pos1], ar[pos2]) > 0) SWAP(ar, pos1, pos2, void*);
    if (cmp(ar[pos3], ar[pos4]) > 0) SWAP(ar, pos3, pos4, void*);
    if (cmp(ar[pos1], ar[pos3]) > 0) SWAP(ar, pos1, pos3, void*);
    if (cmp(ar[pos2], ar[pos4]) > 0) SWAP(ar, pos2, pos4, void*);
    if (cmp(ar[pos2], ar[pos3]) > 0) SWAP(ar, pos2, pos3, void*);
}

//使用间隔距离，对元素进行特定的插入排序
static void insertion(void** ar, int(*cmp)(const void*, const void*), int low, int right, int gap)
{
    int loc;
    for (loc = low + gap;loc <= right;loc += gap)
    {
        int i = loc - gap;
        void* value = ar[loc];
        while (i >= low && cmp(ar[i], value) > 0)
        {
            ar[i + gap] = ar[i];
            i -= gap;
        }
        ar[i + gap] = value;
    }
}

/*
为ar[left,right]寻找合适的pivotIndex。考虑大小为b的集合,在这个代码中,b=4。
原始的BFPRT算法中b=5。
1.将元素分成floor(n/b)个集合,每个集合包含b个元素,在每一个集合中寻找中值元素。
然后将这些中值元素组成集合M。
2.如果|M|>b,那么递归地执行上一步,直到分成少于或者等于b个集合。
3.在递归的基础情况中,简单地使用插入排序来排序小于等于b个中值,
然后选择出这些中值中的中值。
*/

static int medianOfMedians(void** ar, int(*cmp)(const void*, const void*), int left, int right, int gap)
{
    int s;
    int num;
    int span = 4 * gap;
    //元素不够组成集合？对这些元素插入排序，然后返回中值
    num = (right - left + 1) / span;
    if (num == 0)
    {
        insertion(ar, cmp, left, right, gap);
        num = (right - left + 1) / gap;
        return left + gap * (num - 1) / 2;
    }
    //得到所有集合的中值
    for (s = left;s + span < right;s += span)
    {
        medianOffour(ar, s, gap, cmp);
    }
    //如果仍然有足够的集合，那么增加gap的值，递归的应用在子数组[left,s-1]上，否则的话执行插入排序，返回中值
    if (num < 4)
    {
        //基础情况
        insertion(ar, cmp, left + span / 2, right, span);
        return medianOfMedians(ar, cmp, left + span / 2, s - 1,span);
    }
    else
    {
        return medianOfMedians(ar, cmp, left + span / 2, s - 1, span);
    }
    
}

//在ar[left,right]中寻找中值的最坏情况为线性的算法，比较函数cmp用来比较元素。
int selectMedian(void** ar, int(*cmp)(const void*, const void*), int left, int right)
{
    int k = (right - left + 1) / 2;
    while (k > 0)
    {
        //选择切分的时候应该根据元素的索引
        int idx = medianOfMedians(ar, cmp, left, right, 1);
        /*根据中值集合的中值x切分输入数组。如果能够找到第k大的元素,返回其素引,
          否则在A[left,pivotIndex-1]寻找第k大元素或者在A[pivotIndex+1,right]中
          寻找(k-p)大元素
          在A[Pivot  Index+1,  right]中。 */

        int pivotIndex = partition(ar, cmp, left, right, idx);
        //注意0<=k<=right-left,返回值pivotIndex是left<=pivotIndex<=right
        int p = left + k;
        if (p == pivotIndex)
        {
            return pivotIndex;
        }
        else if(p<pivotIndex)
        {
            right = pivotIndex - 1;
        }
        else
        {
            k = k - (pivotIndex - left + 1);
            left = pivotIndex + 1;
        }
    }
    //如果执行到这里，那么left=right，所以仅仅只需要返回两个中的一个作为中值。
    return left;
}

// 打印数组  
void printArray(int* ar, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", ar[i]);
    }
    printf("\n");
}

int main() 
{
    // 示例整数数组  
    int arr[] = { 5, 7, 4, 6, 3, 8, 1, 0, 9, 2};
    int size = sizeof(arr) / sizeof(arr[0]);

    // 创建指向整数的指针数组  
    void** ptrArr = (void**)malloc(size * sizeof(void*));
    for (int i = 0; i < size; i++) {
        ptrArr[i] = &arr[i];
    }

    // 找到中值的索引  
    int medianIndex = selectMedian(ptrArr, cmp, 0, size - 1);

    // 打印结果  
    printf("中值索引: %d\n", medianIndex);
    printf("中值元素: %d\n", arr[medianIndex]);

    // 释放动态分配的内存  
    free(ptrArr);

    return 0;
}

