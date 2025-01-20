#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//����ֵ�Ĳ�������
int cmp(const void* x, const void* y)
{
	int standard;
	//����Ƚ�ʱҲ����Ҫ��ǿ������ת��
	standard = *(int*)x - *(int*)y;

	return standard;
}
/*cmp��һ��ָ��ȽϺ�����ָ�룬�ú����������� const void* ���͵Ĳ�����������һ��������
�����һ������Ӧ�����ڵڶ�������֮ǰ���򷵻ظ��������������ȣ��򷵻��㣻
�����һ������Ӧ�����ڵڶ�������֮���򷵻�������*/

void insertionSortPointers(void* base, int n, int s, int(*cmp)(const void*, const void*))
{
	int j;
	//��ʱ����һ��Ԫ��
	void* saved = malloc(s);
	for (j = 1;j < n;j++)
	{
		//��ĩβ��ʼ�������,ֱ��i<0�����ҵ�һ����Ƚ�С��Ԫ��
		int i = j - 1;
		void* value = (char*)base + j * s;
		while (i >= 0 && cmp((char*)base + i * s, value) > 0)
		{
			i--;
		}
		//����Ѿ�������ȷ��λ���ˣ���ô��û�б�Ҫ�ƶ������򱣴潫Ҫ�����ֵ�����ƶ���һ����ڴ棬��������ʵ���λ�á�
		if (++i == j)
		{
			continue;
		}
		//����Ҫ�����ֵ
		memmove(saved, value, s);
		//�������Ԫ������ƶ�һ��λ��
		memmove((char*)base + (i + 1) * s, (char*)base + i * s, s * (j - i));
		//�������ֵ���뵽��ȷ��λ��
		memmove((char*)base + i * s, saved, s);
		
	}
	free(saved);  // �ͷ���ʱ������ڴ�
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
		//printf("%d",*num[i]);���Ǵ���Ĵ�ӡvoid���鷽������������ȷ�ģ�
		//��void*ת��Ϊint*��������  
		int intPtr = (int)base[i];
		printf("base[%d] = %d\n", i, intPtr);
	}
	return 0;
	
}

/*
ΪʲôΪʲô�����baseҪת��ǿ��ת��Ϊchar���ͣ�
��C�����У���baseָ��ת��Ϊchar*���ͣ����ַ�ָ�룩��Ϊ���ܹ���ȷ�ذ����ֽ�ƫ�������������е�Ԫ�ء�������Ϊchar����
��C�ж���Ϊռ��1���ֽڣ�byte�������ʹ��char*��Ϊָ�����Ϳ���ȷ��ÿ��ƫ�ƶ��ǰ���1���ֽ������еġ�
��insertionSortPointers�����У�base��һ��void*���͵�ָ�룬��ָ���������Ԫ�ء�����void*��һ��ͨ��ָ�����ͣ�������
ֱ������ָ���������㣨��ӷ������������Ϊ��������֪��void*ָ����������͵Ĵ�С����ˣ�Ϊ���ܹ�����Ԫ�صĴ�Сs����ȷ
�ؼ���������ÿ��Ԫ�صĵ�ַ��������Ҫ��baseת��Ϊһ����֪��С��ָ�����͡�
ѡ��char*��Ϊת��Ŀ���м���ԭ��
��׼���壺char������C��׼�б�����Ϊռ��1���ֽڣ���ʹ��char*��Ϊ�����ֽڼ�����������ѡ��
��ƽ̨�����ԣ�ʹ��char*����ָ����������ȷ���ڲ�ͬƽ̨�ϵ�һ���ԣ���Ϊchar�Ĵ�С������ƽ̨�϶�����ͬ�ģ���1���ֽڣ���
������⣺��baseת��Ϊchar*������ƫ�Ƽ��㣬���������ر����������ڽ����ֽڼ��Ĳ����������ǻ���ĳ���ض��������͵Ĵ�
С��
��insertionSortPointers�����У�ͨ����baseת��Ϊchar*���͵�ָ��baseChar�����ǿ���ʹ��baseChar + j * s����������
�е�j��Ԫ�صĵ�ַ����������Ԫ�صĴ�СΪs�ֽڣ������������������е�Ԫ����ʲô���ͣ����Ƕ�������ȷ�ؼ�������ǵĵ�ַ��
�����к����Ĳ������������
�����Ҫע����ǣ��ڽ�baseת��Ϊchar*֮�������ڱȽϺ���cmp�ĵ�������Ȼ��Ҫ��ָ��ת��Ϊconst void*���ͣ�������
Ϊcmp�����Ĳ���������const void*������Ϊ��ȷ�����Ͱ�ȫ��������cmp���������κ����͵�ָ����Ϊ������
*/