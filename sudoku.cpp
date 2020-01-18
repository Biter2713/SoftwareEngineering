#include "iostream"
#include "algorithm"
#include "fstream"
#include "string"
#include "cstring"
#include "cmath"
#include "cstdio"
#include "cstdlib"
using namespace std;

ofstream print_sudoku("sudoku.txt"); //ofstream file3("c:\\x.123"); 以输出方式打开文件
int origin[10] = { 1,2,3,4,6,7,8,9 }; //对第一行进行全排列（因为第一个数字为(1 + 3)% 9 + 1 = 5，全排列不考虑5）
int count_num = 0, data_count = 0; //data_count??
char output_sudoku[500000000];//为了方便我们牺牲空间,节约时间,一次输出

int sudoku_map[10][9][9]; //
int tempt[9] = { 1,2,3,4,5,6,7,8,9 };

void change_map(char *rule) //进行变换；rule变换规则
{
	for (int i = 0; i < 3; ++i)
	{   //最终输出数组，包含数字、空格、换行符
		output_sudoku[data_count++] = origin[(8 + rule[i] - '0') % 9] + '0'; //学号号码插入排列的尾部
		for (int j = 1; j < 17; ++j)
		{
			output_sudoku[data_count++] = ' ';  //存空格
			j++;
			output_sudoku[data_count++] = origin[((16 - j) / 2 + rule[i] - '0') % 9] + '0';  //将数组转为环形，方便平移
		}
		output_sudoku[data_count++] = '\n';  //存换行符
	}
}
//生成终局
void BuildSudoku(char *rule1, char *rule2, char*rule3) 
{
	change_map(rule1); //036
	change_map(rule2); //147
	change_map(rule3); //258
	output_sudoku[data_count++] = '\n';
}
void buildMove(int N) //变换得到N个不同数独
{
	char rule1[10][5] = { "036","063" };
	char rule2[10][5] = { "258","285","528","582","825","852" };
	char rule3[10][5] = { "147","174","417","471","714","741" }; //变换规则
	while (1)
	{
		if (next_permutation(origin, origin + 8)) //next_permutation??
		{
			origin[8] = 5;  //学号号码插入排列尾部
			for (int i = 0; i < 2; ++i) //1~3行变换：036 063 两种
			{
				for (int j = 0; j < 6; ++j) //4~6行变换：258 ... 852 六种
				{
					for (int t = 0; t < 6; ++t) //7~9行变换：147 ... 741 六种
					{
						BuildSudoku(rule1[i], rule2[j], rule3[t]); //根据不同规则变化终局
						count_num++;
						if (count_num == N)  //直到数量达到N
							return;
					}
				}
			}
		}
		else
			break;
	}
}
//输出终局
void print1()
{
	print_sudoku << output_sudoku;
}


//求解数独
int judge(int s, int x, int y, int num)
{//判断填充合法
	for (int i = 0; i < 9; i++)
	{    //当前行、列合法判断
		if (sudoku_map[s][x][i] == num)
			return 0;
		if (sudoku_map[s][i][y] == num)
			return 0;
	}
	int area_x = x - x % 3, area_y = y - y % 3;    //计算所处宫格左上角坐标
	for (int i = area_x; i < area_x + 3; i++)    //当前宫格合法判断
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
	if (sudoku_map[s][a][b] != 0)  //该位置非空格
	{
		if (solve(s, next_a, next_b))
			return 1;
	}
	else  //等于0，该位置为空格
	{
		for (int i = 0; i < 9; ++i)
		{
			int trynum = tempt[i];
			if (judge(s, a, b, trynum))  //判断trynum若填充合法
			{
				sudoku_map[s][a][b] = trynum;  //该位置填为trynum
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

	//生成终局
	if (argc == 3 && strcmp(argv[1], "-c") == 0)  //命令行字符串为“-c”
	{   //判断合法输入
		int len = strlen(argv[2]); 
		for (int i = 0; i < len; ++i)
		{
			if (argv[2][i] >= '0' && argv[2][i] <= '9') 
			{
				N = N * 10 + argv[2][i] - '0';   //转为数字
				//printf("%d",N);
			}
			else
			{
				printf("Error!\n");
				return 0;
			}
		}
		buildMove(N); //得到所需N个数独终局
		print1(); //输出终局
	}

	//求解数独
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)  //命令行字符串为“-s”
	{   
		//printf("****\n");
		int i = 0, j = 0;
		freopen(argv[2], "r", stdin); //输入重定向，输入数据将从argv[2](输入的problem.txt)文件中读取
		printf("%s\n", argv[2]);
		freopen("sudoku.txt", "w", stdout);  //输出重定向，输出数据将保存在sudoku.txt文件中
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
		fclose(stdin);   //关闭文件
		fclose(stdout);  //关闭文件
		return 0;
	}
	return 0;
}