#include <stdlib.h>
#include <time.h>

#define N 32

float a[N][N],b[N][N],c[N][N];


void matrix_multiply()
{
	int i, j, k;
	for(i = 0;i < N;i++)
		for(j = 0;j < N;j++)
		{
			c[i][j] = 0;
			for(k = 0;k < N;k++)
				c[i][j] += a[i][k] * b[k][j];
		}


}

int main()
{
	int i,j;
	srand(time(NULL));
	for(i = 0;i < N;i++)
		for(j = 0;j < N;j++)
		{
			a[i][j] = (float)rand() / rand();
			b[j][i] = (float)rand() / rand();
		}
	matrix_multiply(); 
	return 0;
}
