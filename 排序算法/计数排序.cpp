#include<stdio.h>
#include<stdlib.h>

//����ar�е�n��Ԫ�أ���Χ��[0,k)
int countingSort(int* ar, int n, int k)
{
	int i;
	int idx = 0;
	int* B =(int*) calloc(k,sizeof(int));
	//ʵ����ɶ�λ�õ�����
    for (i = 0;i < n;i++)
	{
		B[ar[i]]++;
	}
    //�������õ�λ��
	for (i = 0;i < k;i++)
	{
		while (B[i]-- > 0)
		{
			ar[idx++] = i;
		}
	}
	free(B);
    return 0;
}

int main() {
    int ar[] = { 4, 2, 2, 8, 3, 3, 1 };
    int n = sizeof(ar) / sizeof(ar[0]);
    int k = 9; // ����Ԫ�ط�Χ��[0, 9)֮��  

    printf("Original array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", ar[i]);
    }
    printf("\n");

    // ����countingSort����  
    if (countingSort(ar, n, k) != 0) {
        fprintf(stderr, "Sorting failed\n");
        return 1;
    }

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", ar[i]);
    }
    printf("\n");

    return 0;
}
