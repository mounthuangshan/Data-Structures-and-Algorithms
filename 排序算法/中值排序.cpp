#include<stdio.h>

/*
��ar��Ѱ�ҵ�k��Ԫ�ص�λ�õ��㷨,����ƽ�������Ϊ����ʱ�䡣
A���ż���Ľ��в��ϱ��޸ĸ��¡�ע��1kright-left+1��
�ȽϺ���cmp�����ϸ�رȽ�Ԫ�ء�����������Ϊ���η�,��o(n2)��
*/

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

int selectpivotIndex(void** ar, int left, int right)
{
	// �����м�λ�õ�����
	int mid = (left + right) / 2;

	// ��ȡ��ˡ��м���Ҷ˵�ֵ
	int left_val = *((int*)ar[left]);
	int mid_val = *((int*)ar[mid]);
	int right_val = *((int*)ar[right]);

	// �ҳ�������ֵ����ֵ
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
	//����ȡ�з��ҳ���ֵ
	int idx = selectpivotIndex(ar, left, right);
	//�������򣬷�������õ�������м�ֵλ��
	int pivotIndex = partition(ar, cmp, left, right, idx);
	//����ıȽϺ��������õ�������м�ֵ��λ�þ��ǵڼ���ͨ�����ַ������轫������ȫ������ɾ����ҵ���k���Ԫ��
	if (left + k - 1 == pivotIndex)
	{
		return pivotIndex;
	}
	//����ѭ��������С��Χ�������������ֵ����ߣ���ôk�����䡣
	if (left + k - 1 < pivotIndex)
	{
		return selectKth(ar, cmp, k, left, pivotIndex - 1);
	}
	else
	{
		return selectKth(ar, cmp, k - (pivotIndex - left + 1), pivotIndex + 1, right);
	}

}

//ʹ��medianSort��������ar[left,right]���ȽϺ���cmp�����ϸ�ıȽ�Ԫ�ء�
void mediansort(void** ar, int(*cmp)(const void*, const void*), int left, int right)
{
	//����������������ֻ��һ�������߸��٣�Ԫ�أ����ء�
	if (right <= left)
	{
		return;
	}
	//�õ��е����ֵԪ��λ��k��(1<=k<=right-left-1)
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
		//printf("%d",*num[i]);���Ǵ���Ĵ�ӡvoid���鷽������������ȷ�ģ�
		// ��void*ת��Ϊint*��������  
		int* intPtr = (int*)base[i];
		printf("base[%d] = %d\n", i, *intPtr);
	}
	printf("��ֵΪbase[%d] = % d\n", z, *(int*)base[z]);
	return 0;
}