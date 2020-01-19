#include "cstdio"
#include "cstdlib"
#include "iostream"
#include "fstream"
#include "string"
#include "cstring"
#include "cmath"
#include "algorithm"

using namespace std;

int prmt[10] = { 1,2,3,4,6,7,8,9 }; //第一行全排列数值（因为第一个数字为(1 + 3)% 9 + 1 = 5，全排列不考虑5）
int flag = 0;
char result[200000000];
int sudoku[10][9][9]; 
//变换方式
int move_0[3][3] = { {0, 3, 6} };//1~3行平移单位
int move_1[6][3] = { {1, 4, 7}, {1, 7, 4}, {4, 1, 7}, {4, 7, 1}, {7, 1, 4}, {7, 4, 1} };//4~6行的平移单位
int move_2[6][3] = { {2, 5, 8}, {2, 8, 5}, {5, 2, 8}, {5, 8, 2}, {8, 2, 5}, {8, 5, 2} };//7~9行的平移单位

//生成终局
void permutation(int *move) //变换；
{
	for (int i = 0; i < 3; ++i)
	{ 
		result[flag++] = prmt[(move[i] + 8) % 9] + '0';
		for (int j = 1; j < 17; ++j)
		{
			result[flag++] = ' ';  //存空格
			j++;
			result[flag++] = prmt[(move[i] + (16 - j) / 2) % 9] + '0';
		}
		result[flag++] = '\n';  //存换行符
	}
}
void changeSusoku(int *move_0, int *move_1, int *move_2) 
{
	permutation(move_0); //036
	permutation(move_1); //147
	permutation(move_2); //258
	result[flag++] = '\n';
}
void makeN(int N) //得到N个数独
{

	int makeNum = 0;
	do
	{
		prmt[8] = 5;  //学号号码插入排列尾部
		for (int i = 0; i < 2; ++i) //1~3行变换：036 063 两种
		{
			for (int j = 0; j < 6; ++j) //4~6行变换：258 ... 852 六种
			{
				for (int m = 0; m < 6; ++m) //7~9行变换：147 ... 741 六种
				{
					changeSusoku(move_0[i], move_1[j], move_2[m]); //根据不同方式变化终局
					if (++makeNum == N)  //直到数量达到N
						return;
				}
			}
		}
	} while (prev_permutation(prmt, prmt + 8));  //prev_permutation函数再得一个全排列
}


//求解数独
int solve(int s, int a, int b)
{   
	int t = sudoku[s][a][b];
	if (a == 9)   return 1;
	if (sudoku[s][a][b] != 0)  //该位置非空格
	{
		if (solve(s, a + (b + 1) / 9, (b + 1) % 9))   return 1;
	}
	else  //等于0，该位置为空格
	{
		for (int i = 0; i < 9; ++i)
		{
			int tempt[9] = { 1,2,3,4,5,6,7,8,9 };
			int r = tempt[i];
			int tag = 0;
			for (int i = 0; i < 9; i++)
			{    //行、列判断
				if (sudoku[s][a][i] == r || sudoku[s][i][b] == r)
					tag++;
			}    //宫判断
			int gong_x = a - a % 3, gong_y = b - b % 3;    //宫左上角坐标
			for (int i = gong_x; i < gong_x + 3; i++)
				for (int j = gong_y; j < gong_y + 3; j++)
					if (sudoku[s][i][j] == r)
						tag++;
			if (tag == 0)  //判断r若填充合法
			{
				sudoku[s][a][b] = r;  //该位置填为r
				if (solve(s, a + (b + 1) / 9, (b + 1) % 9))   return 1;
			}
		}
	}
	sudoku[s][a][b] = t;
	return 0;
}


int main(int argc, char const *argv[])
{
	//生成终局
	if(argc != 3) printf("Error!\n");
	else 
	{
		int N = 0; //存输入个数argv[2]的数值型
		if (argv[1][0] == '-' && argv[1][1] == 'c')  //命令行字符串为“-c”
	    {   //判断合法输入
			for (int i = 0; i < strlen(argv[2]); ++i)
		    {
				if (argv[2][i] >= '0' && argv[2][i] <= '9')  N = N * 10 + argv[2][i] - '0';   //转为数值
			    else
			    {
				   printf("Error!\n");
				     return 0;
			    }
		    }
		makeN(N); //得到所需N个数独终局
		//输出终局
		ofstream FinalFile("sudoku.txt"); //ofstream file3("c:\\x.123"); 以输出方式打开文件
		FinalFile << result;
		printf("成功生成终局文件sudoku.txt!\n");
	    }

	//求解数独
	    else if (argv[1][0] == '-' && argv[1][1] == 's')  //命令行字符串为“-s”
	    {
		    int i = 0, j = 0;
			FILE *fp1;  
				fp1 = fopen(argv[2], "r");   //题目文件
			freopen("sudoku.txt", "w", stdout);  //结果文件
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
			fclose(fp1);   //关闭题目文件
			fclose(stdout);  //关闭结果文件
			return 0;
	}
 }
	return 0;
}
