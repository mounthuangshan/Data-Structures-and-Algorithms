#include<stdio.h>
#include<stdlib.h>

//ʹ�ò�������������С����ֵ
#define MINSIZE 4

int cmp(const void* x, const void* y)
{
	int standard;
	//����Ƚ�ʱҲ����Ҫ��ǿ������ת��
	standard = *(int*)x - *(int*)y;

	return standard;
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

//֮ǰʵ�ֵĻ���ָ��Ĳ�������������������ֵ����ȷ��Ϊ0����������Ķ�
void insertionSortPointers(void** ar,int(*cmp)(const void*, const void*),int m,int n)
{
	int j;
	for (j = m+1;j <= n;j++)
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
}

//ʹ�ÿ�����������ar[left,right]���򣬱ȽϺ���cmp�����ϸ������Ԫ��
void do_qsort(void** ar, int(*cmp)(const void* ,const void*), int left, int right)
{
	int pivotIndex;
	if (right <= left)
	{
		return;
	}
	//�з�
	pivotIndex = selectpivotIndex(ar, left, right);
	pivotIndex = partition(ar, cmp, left, right, pivotIndex);
	if (pivotIndex - 1 - left <= MINSIZE)
	{
		insertionSortPointers(ar, cmp, left, pivotIndex - 1);
	}
	else
	{
		do_qsort(ar, cmp, left, pivotIndex-1);
	}
	if(right-pivotIndex-1<=MINSIZE)
	{
		insertionSortPointers(ar, cmp,pivotIndex +1,right);
	}
	else
	{
		do_qsort(ar, cmp,pivotIndex + 1,right);
	}
}

//ֱ�ӿ�������
void sortPointers(void** vals, int total_elems, int(*cmp)(const void*, const void*))
{
	do_qsort(vals, cmp, 0, total_elems - 1);
}

// ��ӡ�������飨ͨ��ָ�����飩  
void printArray(void** ar, int total_elems) {
	for (int i = 0; i < total_elems; i++) {
		printf("%d ", *((int*)ar[i]));
	}
	printf("\n");
}

int main() {
	//int nums[] = { 34, 7, 23, 32, 5, 62, 32, 19, 45, 2, 90, 35 };
	int nums[] = { 5, 7, 4, 6, 3, 8, 1, 0, 9, 2};
	int total_elems = sizeof(nums) / sizeof(nums[0]);

	// ����ָ������  
	void** vals = (void**)malloc(total_elems * sizeof(void*));
	for (int i = 0; i < total_elems; i++) {
		vals[i] = (void*)&nums[i];
	}

	// ����  
	sortPointers(vals, total_elems, cmp);

	// ��ӡ����������  
	printArray(vals, total_elems);

	// �ͷ��ڴ�  
	free(vals);

	return 0;
}