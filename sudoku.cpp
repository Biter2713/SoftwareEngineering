#include "iostream"
#include "algorithm"
#include "fstream"
#include "string"
#include "cstring"
#include "cmath"
#include "cstdio"
#include "cstdlib"
using namespace std;

ofstream print_sudoku("sudoku.txt"); //ofstream file3("c:\\x.123"); �������ʽ���ļ�
int origin[10] = { 1,2,3,4,6,7,8,9 }; //�Ե�һ�н���ȫ���У���Ϊ��һ������Ϊ(1 + 3)% 9 + 1 = 5��ȫ���в�����5��
int count_num = 0, data_count = 0; //data_count??
char output_sudoku[500000000];//Ϊ�˷������������ռ�,��Լʱ��,һ�����

int sudoku_map[10][9][9]; //
int tempt[9] = { 1,2,3,4,5,6,7,8,9 };

void change_map(char *rule) //���б任��rule�任����
{
	for (int i = 0; i < 3; ++i)
	{   //����������飬�������֡��ո񡢻��з�
		output_sudoku[data_count++] = origin[(8 + rule[i] - '0') % 9] + '0'; //ѧ�ź���������е�β��
		for (int j = 1; j < 17; ++j)
		{
			output_sudoku[data_count++] = ' ';  //��ո�
			j++;
			output_sudoku[data_count++] = origin[((16 - j) / 2 + rule[i] - '0') % 9] + '0';  //������תΪ���Σ�����ƽ��
		}
		output_sudoku[data_count++] = '\n';  //�滻�з�
	}
}
//�����վ�
void BuildSudoku(char *rule1, char *rule2, char*rule3) 
{
	change_map(rule1); //036
	change_map(rule2); //147
	change_map(rule3); //258
	output_sudoku[data_count++] = '\n';
}
void buildMove(int N) //�任�õ�N����ͬ����
{
	char rule1[10][5] = { "036","063" };
	char rule2[10][5] = { "258","285","528","582","825","852" };
	char rule3[10][5] = { "147","174","417","471","714","741" }; //�任����
	while (1)
	{
		if (next_permutation(origin, origin + 8)) //next_permutation??
		{
			origin[8] = 5;  //ѧ�ź����������β��
			for (int i = 0; i < 2; ++i) //1~3�б任��036 063 ����
			{
				for (int j = 0; j < 6; ++j) //4~6�б任��258 ... 852 ����
				{
					for (int t = 0; t < 6; ++t) //7~9�б任��147 ... 741 ����
					{
						BuildSudoku(rule1[i], rule2[j], rule3[t]); //���ݲ�ͬ����仯�վ�
						count_num++;
						if (count_num == N)  //ֱ�������ﵽN
							return;
					}
				}
			}
		}
		else
			break;
	}
}
//����վ�
void print1()
{
	print_sudoku << output_sudoku;
}


//�������
int judge(int s, int x, int y, int num)
{//�ж����Ϸ�
	for (int i = 0; i < 9; i++)
	{    //��ǰ�С��кϷ��ж�
		if (sudoku_map[s][x][i] == num)
			return 0;
		if (sudoku_map[s][i][y] == num)
			return 0;
	}
	int area_x = x - x % 3, area_y = y - y % 3;    //���������������Ͻ�����
	for (int i = area_x; i < area_x + 3; i++)    //��ǰ����Ϸ��ж�
		for (int j = area_y; j < area_y + 3; j++)
			if (sudoku_map[s][i][j] == num)
				return 0;
	return 1;
}

int solve(int s, int a, int b)
{
	int init, next_a, next_b;
	init = sudoku_map[s][a][b];
	next_a = a + (b + 1) / 9;
	next_b = (b + 1) % 9;
	if (a == 9)
		return 1;
	if (sudoku_map[s][a][b] != 0)  //��λ�÷ǿո�
	{
		if (solve(s, next_a, next_b))
			return 1;
	}
	else  //����0����λ��Ϊ�ո�
	{
		for (int i = 0; i < 9; ++i)
		{
			int trynum = tempt[i];
			if (judge(s, a, b, trynum))  //�ж�trynum�����Ϸ�
			{
				sudoku_map[s][a][b] = trynum;  //��λ����Ϊtrynum
				if (solve(s, next_a, next_b))
					return 1;
			}
		}
	}
	sudoku_map[s][a][b] = init;
	return 0;
}





int main(int argc, char const *argv[])
{
	int N = 0;
	//scanf("%d",&N);
	//FILE *fp1 = fopen(argv[2], "r");
	FILE *fp1 = fopen(argv[2], "r");

	//�����վ�
	if (argc == 3 && strcmp(argv[1], "-c") == 0)  //�������ַ���Ϊ��-c��
	{   //�жϺϷ�����
		int len = strlen(argv[2]); 
		for (int i = 0; i < len; ++i)
		{
			if (argv[2][i] >= '0' && argv[2][i] <= '9') 
			{
				N = N * 10 + argv[2][i] - '0';   //תΪ����
				//printf("%d",N);
			}
			else
			{
				printf("Error!\n");
				return 0;
			}
		}
		buildMove(N); //�õ�����N�������վ�
		print1(); //����վ�
	}

	//�������
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)  //�������ַ���Ϊ��-s��
	{   
		//printf("****\n");
		int i = 0, j = 0;
		freopen(argv[2], "r", stdin); //�����ض����������ݽ���argv[2](�����problem.txt)�ļ��ж�ȡ
		printf("%s\n", argv[2]);
		freopen("sudoku.txt", "w", stdout);  //����ض���������ݽ�������sudoku.txt�ļ���
		//printf("****\n");
		while (~scanf("%d", &sudoku_map[0][i][j]))
		{
			//printf("%d\n",sudoku_map[0][i][j]);
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
						if (j == 8)
							putchar('\n');
						else
						{
							putchar(sudoku_map[0][i][j] + '0');
							putchar(' ');
						}
					}
				}
				putchar('\n');
			}
		}
		fclose(stdin);   //�ر��ļ�
		fclose(stdout);  //�ر��ļ�
		return 0;
	}
	return 0;
}