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
    //����Ƚ�ʱҲ����Ҫ��ǿ������ת��
    standard = *(int*)x - *(int*)y;

    return standard;
}

int partition(void** ar, int(*cmp)(const void*, const void*), int left, int right, int pivotIndex)
{
    int idx;
    int store;
    void* pivot = ar[pivotIndex];
    //������ֵ�ƶ��������β
    void* tmp = ar[right];
    ar[right] = ar[pivotIndex];
    ar[pivotIndex] = tmp;
    //������С�ڵ�������ֵ��Ԫ�ض����ƶ��������ǰ�棬Ȼ������ֵ�������Ǻ���
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
    //��һ�ν���������ֵ���ں���
    tmp = ar[right];
    ar[right] = ar[store];
    ar[store] = tmp;
    //������ֵ��λ��
    return store;
}

//Ѱ��������ar[left]��ar[left + gap]��ar[left + gap * 2]��ar[left + gap * 3]��4��Ԫ�ص���ֵ, �ڽ�����ʱ��ȷ��ar[left + gap * 2]��������ֵ��
//ԭ����룺
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

//�Ż���Ĵ��룺
/*
�Ż�ǰ�Ĵ���������˸��Ӹ��ӵ��߼������ٲ���Ҫ�ıȽϺͽ�����������ͨ��һϵ�еıȽ���ȷ��Ԫ��֮������˳��
Ȼ�������Щ�ȽϵĽ���������Ƿ���н��������ַ������ŵ�������ͼͨ�����ٵıȽϺͽ����������ﵽ�����Ŀ�ģ��Ż���Ĵ����߼����ֱ�ӣ�
�������˶��Ƕ�׵�������䣬����ܵ�����ĳЩ��������ܲ������ŵģ���Ϊ��ʹ�Ѿ�ȷ����Ԫ�ص����˳�򣬴�����Ȼ�����ִ�в���Ҫ�ıȽϡ�
���δ����������Ͽ��ܴ��ڲ��죬�����ֲ�����ܲ������������ġ���ѡ��ʹ���Ķδ���ʱ����ҪȨ�����Ŀɶ��ԡ���ά���Ժ�ʵ����������
��������ǹؼ����أ����ҿ���ͨ�����ٱȽϺͽ�������������������ܣ���ô���Կ���ʹ���Ż�ǰ�Ĵ��루�������н�һ�����Ż�����
Ȼ�����������Ŀɶ��ԺͿ�ά���Ը�����Ҫ����ô�Ż���Ĵ��������һ�����õ�ѡ��
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

//ʹ�ü�����룬��Ԫ�ؽ����ض��Ĳ�������
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
Ϊar[left,right]Ѱ�Һ��ʵ�pivotIndex�����Ǵ�СΪb�ļ���,�����������,b=4��
ԭʼ��BFPRT�㷨��b=5��
1.��Ԫ�طֳ�floor(n/b)������,ÿ�����ϰ���b��Ԫ��,��ÿһ��������Ѱ����ֵԪ�ء�
Ȼ����Щ��ֵԪ����ɼ���M��
2.���|M|>b,��ô�ݹ��ִ����һ��,ֱ���ֳ����ڻ��ߵ���b�����ϡ�
3.�ڵݹ�Ļ��������,�򵥵�ʹ�ò�������������С�ڵ���b����ֵ,
Ȼ��ѡ�����Щ��ֵ�е���ֵ��
*/

static int medianOfMedians(void** ar, int(*cmp)(const void*, const void*), int left, int right, int gap)
{
    int s;
    int num;
    int span = 4 * gap;
    //Ԫ�ز�����ɼ��ϣ�����ЩԪ�ز�������Ȼ�󷵻���ֵ
    num = (right - left + 1) / span;
    if (num == 0)
    {
        insertion(ar, cmp, left, right, gap);
        num = (right - left + 1) / gap;
        return left + gap * (num - 1) / 2;
    }
    //�õ����м��ϵ���ֵ
    for (s = left;s + span < right;s += span)
    {
        medianOffour(ar, s, gap, cmp);
    }
    //�����Ȼ���㹻�ļ��ϣ���ô����gap��ֵ���ݹ��Ӧ����������[left,s-1]�ϣ�����Ļ�ִ�в������򣬷�����ֵ
    if (num < 4)
    {
        //�������
        insertion(ar, cmp, left + span / 2, right, span);
        return medianOfMedians(ar, cmp, left + span / 2, s - 1,span);
    }
    else
    {
        return medianOfMedians(ar, cmp, left + span / 2, s - 1, span);
    }
    
}

//��ar[left,right]��Ѱ����ֵ������Ϊ���Ե��㷨���ȽϺ���cmp�����Ƚ�Ԫ�ء�
int selectMedian(void** ar, int(*cmp)(const void*, const void*), int left, int right)
{
    int k = (right - left + 1) / 2;
    while (k > 0)
    {
        //ѡ���зֵ�ʱ��Ӧ�ø���Ԫ�ص�����
        int idx = medianOfMedians(ar, cmp, left, right, 1);
        /*������ֵ���ϵ���ֵx�з��������顣����ܹ��ҵ���k���Ԫ��,����������,
          ������A[left,pivotIndex-1]Ѱ�ҵ�k��Ԫ�ػ�����A[pivotIndex+1,right]��
          Ѱ��(k-p)��Ԫ��
          ��A[Pivot  Index+1,  right]�С� */

        int pivotIndex = partition(ar, cmp, left, right, idx);
        //ע��0<=k<=right-left,����ֵpivotIndex��left<=pivotIndex<=right
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
    //���ִ�е������ôleft=right�����Խ���ֻ��Ҫ���������е�һ����Ϊ��ֵ��
    return left;
}

// ��ӡ����  
void printArray(int* ar, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", ar[i]);
    }
    printf("\n");
}

int main() 
{
    // ʾ����������  
    int arr[] = { 5, 7, 4, 6, 3, 8, 1, 0, 9, 2};
    int size = sizeof(arr) / sizeof(arr[0]);

    // ����ָ��������ָ������  
    void** ptrArr = (void**)malloc(size * sizeof(void*));
    for (int i = 0; i < size; i++) {
        ptrArr[i] = &arr[i];
    }

    // �ҵ���ֵ������  
    int medianIndex = selectMedian(ptrArr, cmp, 0, size - 1);

    // ��ӡ���  
    printf("��ֵ����: %d\n", medianIndex);
    printf("��ֵԪ��: %d\n", arr[medianIndex]);

    // �ͷŶ�̬������ڴ�  
    free(ptrArr);

    return 0;
}

