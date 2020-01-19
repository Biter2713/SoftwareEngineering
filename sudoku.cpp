#include "cstdio"
#include "cstdlib"
#include "iostream"
#include "fstream"
#include "string"
#include "cstring"
#include "cmath"
#include "algorithm"

using namespace std;

int prmt[10] = { 1,2,3,4,6,7,8,9 }; //��һ��ȫ������ֵ����Ϊ��һ������Ϊ(1 + 3)% 9 + 1 = 5��ȫ���в�����5��
int flag = 0;
char result[200000000];
int sudoku[10][9][9]; 
//�任��ʽ
int move_0[3][3] = { {0, 3, 6} };//1~3��ƽ�Ƶ�λ
int move_1[6][3] = { {1, 4, 7}, {1, 7, 4}, {4, 1, 7}, {4, 7, 1}, {7, 1, 4}, {7, 4, 1} };//4~6�е�ƽ�Ƶ�λ
int move_2[6][3] = { {2, 5, 8}, {2, 8, 5}, {5, 2, 8}, {5, 8, 2}, {8, 2, 5}, {8, 5, 2} };//7~9�е�ƽ�Ƶ�λ


void permutation(int *move) //�任��
{
	for (int i = 0; i < 3; ++i)
	{ 
		result[flag++] = prmt[(move[i] + 8) % 9] + '0';
		for (int j = 1; j < 17; ++j)
		{
			result[flag++] = ' ';  //��ո�
			j++;
			result[flag++] = prmt[(move[i] + (16 - j) / 2) % 9] + '0';
		}
		result[flag++] = '\n';  //�滻�з�
	}
}
//�����վ�
void changeSusoku(int *move_0, int *move_1, int *move_2) 
{
	permutation(move_0); //036
	permutation(move_1); //147
	permutation(move_2); //258
	result[flag++] = '\n';
}
void makeN(int N) //�õ�N������
{

	int makeNum = 0;
	do
	{
		prmt[8] = 5;  //ѧ�ź����������β��
		for (int i = 0; i < 2; ++i) //1~3�б任��036 063 ����
		{
			for (int j = 0; j < 6; ++j) //4~6�б任��258 ... 852 ����
			{
				for (int m = 0; m < 6; ++m) //7~9�б任��147 ... 741 ����
				{
					changeSusoku(move_0[i], move_1[j], move_2[m]); //���ݲ�ͬ��ʽ�仯�վ�
					if (++makeNum == N)  //ֱ�������ﵽN
						return;
				}
			}
		}
	} while (prev_permutation(prmt, prmt + 8));  //prev_permutation�����ٵ�һ��ȫ����
}


//�������
int ifok(int s, int x, int y, int r) //�ж����Ϸ�
{
	for (int i = 0; i < 9; i++)
	{    //�С����ж�
		if (sudoku[s][x][i] == r || sudoku[s][i][y] == r)
			return 0;
	}    //���ж�
	int gong_x = x - x % 3, gong_y = y - y % 3;    //�����Ͻ�����
	for (int i = gong_x; i < gong_x + 3; i++)    
		for (int j = gong_y; j < gong_y + 3; j++)
			if (sudoku[s][i][j] == r)
				return 0;
	return 1;
}

int solve(int s, int a, int b)
{   
	int t = sudoku[s][a][b];
	if (a == 9)   return 1;
	if (sudoku[s][a][b] != 0)  //��λ�÷ǿո�
	{
		if (solve(s, a + (b + 1) / 9, (b + 1) % 9))   return 1;
	}
	else  //����0����λ��Ϊ�ո�
	{
		for (int i = 0; i < 9; ++i)
		{
			int tempt[9] = { 1,2,3,4,5,6,7,8,9 };
			int r = tempt[i];
			if (ifok(s, a, b, r))  //�ж�r�����Ϸ�
			{
				sudoku[s][a][b] = r;  //��λ����Ϊr
				if (solve(s, a + (b + 1) / 9, (b + 1) % 9))   return 1;
			}
		}
	}
	sudoku[s][a][b] = t;
	return 0;
}


int main(int argc, char const *argv[])
{
	//�����վ�
	if(argc != 3) printf("Error!\n");
	else 
	{
		int N = 0; //���������argv[2]����ֵ��
		if (argv[1][0] == '-' && argv[1][1] == 'c')  //�������ַ���Ϊ��-c��
	    {   //�жϺϷ�����
			for (int i = 0; i < strlen(argv[2]); ++i)
		    {
				if (argv[2][i] >= '0' && argv[2][i] <= '9')  N = N * 10 + argv[2][i] - '0';   //תΪ��ֵ
			    else
			    {
				   printf("Error!\n");
				     return 0;
			    }
		    }
		makeN(N); //�õ�����N�������վ�
		//����վ�
		ofstream FinalFile("sudoku.txt"); //ofstream file3("c:\\x.123"); �������ʽ���ļ�
		FinalFile << result;
		printf("�ɹ������վ��ļ�sudoku.txt!\n");
	    }

	//�������
	    else if (argv[1][0] == '-' && argv[1][1] == 's')  //�������ַ���Ϊ��-s��
	    {
		    int i = 0, j = 0;
			FILE *fp1;  
				fp1 = fopen(argv[2], "r");   //��Ŀ�ļ�
			freopen("sudoku.txt", "w", stdout);  //����ļ�
			while (~fscanf(fp1,"%d", &sudoku[0][i][j]))
			{
				i += (j + 1) / 9;
				j = (j + 1) % 9;
				if (i == 9)
				{
					solve(0, 0, 0);
					i = j = 0;
					for (int i = 0; i < 9; ++i)
					{
						for (int j = 0; j < 9; ++j)
						{
							if (j == 8) putchar('\n');
							else
							{
								putchar(sudoku[0][i][j] + '0');
								putchar(' ');
							}
						}
					}
					putchar('\n');
				}
			}
			fclose(fp1);   //�ر���Ŀ�ļ�
			fclose(stdout);  //�رս���ļ�
			return 0;
	}
 }
	return 0;
}
