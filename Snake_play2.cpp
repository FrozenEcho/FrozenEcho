#include <graphics.h>  
#include <conio.h>
#include <stdio.h>
#include <string>
#include <time.h>
#pragma comment(lib,"Winmm.lib")
constexpr auto BLOCK_SIZE = 20; // 每个小格子的长宽大小 ;
constexpr auto HEIGHT = 30; // 高度上一共30个小格子;
constexpr auto WIDTH = 40; //  宽度上一共40个小格子 ;
/*防撞系统+积分系统+随机数种子（初始化是固定的）+食物额外分数和随机饱和度*/
/*双人合作模式中，所有关于蛇的存储副本都要两份，食物会同时出现多份*/
// 全局变量定义
static int Blocks[HEIGHT][WIDTH] = { 0 }; //  二维数组，用于记录所有的游戏数据，蛇1的Node为正数
static char moveDirection1;  //  小蛇移动方向
static char moveDirection2;  //  小蛇移动方向

static int food_i1, food_j1, value1; //  食物的位置
static int food_i2, food_j2, value2; //  食物的位置
static int isFailure = 0; //  是否游戏失败
static int speed;

//各自的积分
static int scores1 = 0;
static int scores2 = 0;

static void set_speed(int speed0) {
	speed = speed0;
}
static int get_scores1() {
	int snake_length1 = 0;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 40; j++) {
			if (Blocks[i][j] > 0)
				snake_length1++;
		}
	}
	float extra = 100 / speed;
	scores1 = scores1 + static_cast<int>((snake_length1 - 5) * extra);
	return scores1;
}
static int get_scores2() {
	int snake_length2 = 0;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 40; j++) {
			if (Blocks[i][j] < 0)
				snake_length2++;
		}
	}
	float extra = 100 / speed;
	scores2 = scores2 + static_cast<int>((snake_length2 - 5) * extra);
	return scores2;
}

static void moveSnake1() //  移动小蛇及相关处理函数
{
	int i, j;
	for (i = 0; i < HEIGHT; i++) // 对行遍历 
		for (j = 0; j < WIDTH; j++) // 对列遍历
			if (Blocks[i][j] > 0) // 大于0的为小蛇元素 
				Blocks[i][j]++; //  让其+1
	int oldTail_i, oldTail_j, oldHead_i, oldHead_j; // 定义变量，存储旧蛇尾、旧蛇头坐标  
	int max = 0; // 用于记录最大值 
	for (i = 0; i < HEIGHT; i++) //  对行列遍历
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (max < Blocks[i][j]) //  如果当前元素值比max大
			{
				max = Blocks[i][j]; // 更新max的值
				oldTail_i = i; //  记录最大值的坐标，就是旧蛇尾的位置
				oldTail_j = j; //  
			}
			if (Blocks[i][j] == 2) // 找到数值为2 
			{
				oldHead_i = i; //  数值为2恰好是旧蛇头的位置
				oldHead_j = j; //  
			}
		}
	}
	int newHead_i = oldHead_i; //  设定变量存储新蛇头的位置
	int newHead_j = oldHead_j;

	//  根据用户按键，设定新蛇头的位置
	if (moveDirection1 == 'w') // 向上移动
		newHead_i = oldHead_i - 1;
	else if (moveDirection1 == 's') // 向下移动
		newHead_i = oldHead_i + 1;
	else if (moveDirection1 == 'a') // 向左移动
		newHead_j = oldHead_j - 1;
	else if (moveDirection1 == 'd') // 向右移动
		newHead_j = oldHead_j + 1;

	//  如果蛇头超出边界，或者蛇头碰到蛇身，游戏失败
	if (newHead_i >= HEIGHT || newHead_i < 0 || newHead_j >= WIDTH || newHead_j < 0
		|| Blocks[newHead_i][newHead_j]>0)
	{
		isFailure = 1;
		return; // 函数返回
	}
	else if (Blocks[newHead_i][newHead_j] < 0) {
		isFailure = 3;
		return;
	}

	Blocks[newHead_i][newHead_j] = 1;  // 新蛇头位置数值为1	
	if (newHead_i == food_i1 && newHead_j == food_j1) //  如果新蛇头正好碰到食物
	{
		srand((unsigned)time(NULL));
		scores1 = scores1 + value1;
		food_i1 = rand() % (HEIGHT - 5) + 2; //  食物重新随机位置
		food_j1 = rand() % (WIDTH - 5) + 2;
		value1 = rand() % (10);
		// 不对旧蛇尾处理，相当于蛇的长度+1
	}
	else // 新蛇头没有碰到食物
		Blocks[oldTail_i][oldTail_j] = 0; // 旧蛇尾变成空白，不吃食物时保持蛇的长度不变
}
static void moveSnake2() //  移动小蛇及相关处理函数
{
	int i, j;
	for (i = 0; i < HEIGHT; i++) // 对行遍历 
		for (j = 0; j < WIDTH; j++) // 对列遍历
			if (Blocks[i][j] < 0)
				Blocks[i][j]--; //  让其-1
	int oldTail_i, oldTail_j, oldHead_i, oldHead_j; // 定义变量，存储旧蛇尾、旧蛇头坐标  
	int min = 0; // 用于记录最小值 
	for (i = 0; i < HEIGHT; i++) //  对行列遍历
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (min > Blocks[i][j]) //  如果当前元素值比max小
			{
				min = Blocks[i][j]; // 更新min的值
				oldTail_i = i; //  记录坐标，就是旧蛇尾的位置
				oldTail_j = j;
			}
			if (Blocks[i][j] == -2)
			{
				oldHead_i = i;
				oldHead_j = j;
			}
		}
	}
	int newHead_i = oldHead_i; //  设定变量存储新蛇头的位置
	int newHead_j = oldHead_j;

	//  根据用户按键，设定新蛇头的位置
	if (moveDirection2 == '5') // 向上移动
		newHead_i = oldHead_i - 1;
	else if (moveDirection2 == '2') // 向下移动
		newHead_i = oldHead_i + 1;
	else if (moveDirection2 == '1') // 向左移动
		newHead_j = oldHead_j - 1;
	else if (moveDirection2 == '3') // 向右移动
		newHead_j = oldHead_j + 1;

	//  如果蛇头超出边界，或者蛇头碰到蛇身，游戏失败
	if (newHead_i >= HEIGHT || newHead_i < 0 || newHead_j >= WIDTH || newHead_j < 0
		|| Blocks[newHead_i][newHead_j] < 0)
	{
		isFailure = 2;
		return; // 函数返回
	}
	else if (Blocks[newHead_i][newHead_j] > 0) {
		isFailure = 4;
		return;
	}

	Blocks[newHead_i][newHead_j] = -1;  // 新蛇头位置数值为1	
	if (newHead_i == food_i2 && newHead_j == food_j2) //  如果新蛇头正好碰到食物
	{
		srand((unsigned)time(NULL));
		scores2 = scores2 + value2;
		food_i2 = rand() % (HEIGHT - 5) + 2; //  食物重新随机位置
		food_j2 = rand() % (WIDTH - 5) + 2;
		value2 = rand() % (10);
		// 不对旧蛇尾处理，相当于蛇的长度+1
	}
	else // 新蛇头没有碰到食物
		Blocks[oldTail_i][oldTail_j] = 0; // 旧蛇尾变成空白，不吃食物时保持蛇的长度不变
}

static void startup()  //  初始化函数
{
	int i;
	//initialize Snake 1
	Blocks[HEIGHT / 4][WIDTH / 4] = 1; // 画面中间画蛇头，数字为1
	for (i = 1; i <= 4; i++) //  向左依次4个蛇身，数值依次为2,3,4,5
		Blocks[HEIGHT / 4][WIDTH / 4 - i] = i + 1;
	moveDirection1 = 'd';	 //  初始向右移动

	//initialize Snake 2
	Blocks[3 * HEIGHT / 4][3 * WIDTH / 4] = -1; // 画面中间画蛇头，数字为-1
	for (i = 1; i <= 4; i++) //  向左依次4个蛇身，数值依次为2,3,4,5
		Blocks[3 * HEIGHT / 4][3 * WIDTH / 4 + i] = -(i + 1);
	moveDirection2 = '1';	 //  初始向左移动

	//initialize the food 1&2
	food_i1 = rand() % (HEIGHT - 5) + 2; //  初始化随机食物位置
	food_j1 = rand() % (WIDTH - 5) + 2; //  
	value1 = rand() % (10);
	food_i2 = rand() % (HEIGHT - 5) + 2; //  初始化随机食物位置
	food_j2 = rand() % (WIDTH - 5) + 2; //  
	value2 = rand() % (10);
	initgraph(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE); //  新开画面
	setlinecolor(RGB(200, 200, 200)); // 设置线条颜色
	BeginBatchDraw(); // 开始批量绘制
}

static void show()  // 绘制函数
{
	cleardevice(); // 清屏
	int i, j;
	for (i = 0; i < HEIGHT; i++) //  对二维数组所有元素遍历
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (Blocks[i][j] > 0) // 元素大于0表示是蛇，这里让蛇的身体颜色色调渐变
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 0.9));
			else if (Blocks[i][j] < 0)
				setfillcolor(HSVtoRGB(-(Blocks[i][j] * 10 - 72), 0.9, 0.9));
			else
				setfillcolor(RGB(150, 150, 150)); // 元素为0表示为空，颜色为灰色
			// 在对应位置处，以对应颜色绘制小方格
			fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE,
				(j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
		}
	}

	setfillcolor(RGB(0, (food_i1 - food_j1) * 6, 0)); //  食物为绿色
	//  绘制食物小方块1
	fillrectangle(food_j1 * BLOCK_SIZE, food_i1 * BLOCK_SIZE,
		(food_j1 + 1) * BLOCK_SIZE, (food_i1 + 1) * BLOCK_SIZE);

	setfillcolor(RGB(255, (food_i2 - food_j2) * 6, 255)); //  食物为绿色
	//  绘制食物小方块2
	fillrectangle(food_j2 * BLOCK_SIZE, food_i2 * BLOCK_SIZE,
		(food_j2 + 1) * BLOCK_SIZE, (food_i2 + 1) * BLOCK_SIZE);

	FlushBatchDraw(); // 批量绘制
}


/*Avoid self collision & double direction acceleration system*/
static void updateWithInput()  // 和输入有关的更新函数
{
	if (_kbhit() && isFailure == 0)  //  如果有按键输入，并且不失败
	{
		char input = _getch(); //  获得按键输入
		if (input == 'a' && 'd' != moveDirection1) // 如果是asdw 
		{
			moveDirection1 = input;  // 设定移动方向
		}
		else if (input == 's' && 'w' != moveDirection1) // 如果是asdw 
		{
			moveDirection1 = input;  // 设定移动方向
		}
		else if (input == 'd' && 'a' != moveDirection1) // 如果是asdw 
		{
			moveDirection1 = input;  // 设定移动方向
		}
		else if (input == 'w' && 's' != moveDirection1) // 如果是asdw 
		{
			moveDirection1 = input;  // 设定移动方向
		}
		if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
			moveSnake1();
		}
		if (input == '2' && '5' != moveDirection2) // 如果是asdw 
		{
			moveDirection2 = input;  // 设定移动方向
		}
		else if (input == '3' && '1' != moveDirection2) // 如果是asdw 
		{
			moveDirection2 = input;  // 设定移动方向
		}
		else if (input == '5' && '2' != moveDirection2) // 如果是asdw 
		{
			moveDirection2 = input;  // 设定移动方向
		}
		else if (input == '1' && '3' != moveDirection2) // 如果是asdw 
		{
			moveDirection2 = input;  // 设定移动方向
		}
		if (input == '1' || input == '2' || input == '3' || input == '5') {
			moveSnake2();
		}
	}
}
int main() //  主函数
{
	startup();  // 初始化函数，仅执行一次	
	// 打开音乐
	mciSendString(_T("open music.mp3 alias mymusic"), NULL, 0, NULL);
	// 播放音乐
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);

	//设置速度窗口
		// 初始化绘图窗口
	initgraph(800,600);
	wchar_t s[5];
	InputBox(s, 10, L"请输入难度等级 （0/1/2/3/4/5）");
	int speed0 = _wtoi(s);
	int speed1;
	switch (speed0) {
	case 1:
		speed1 = 20; break;
	case 2:
		speed1 = 15; break;
	case 3:
		speed1 = 12; break;
	case 4:
		speed1 = 10; break;
	case 5:
		speed1 = 8; break;
	default:
		speed1 = 30;
	}
	set_speed(speed1);

	while (1)   // 一直循环
	{
		show();  // 进行绘制
		static int waitIndex = 1; // 静态局部变量，初始化时为1
		waitIndex++; // 每一帧+1
		if (waitIndex == speed) // 经过speed帧才执行
		{
			moveSnake1(); //  调用小蛇移动函数
			moveSnake2();
			waitIndex = 1; // 再变成1
		}
		updateWithInput();    // 和输入有关的更新   
		if (isFailure == 1 || isFailure == 2 || isFailure == 3 || isFailure == 4) //  如果游戏失败 isFailure =1||2||3||4
		{
			setbkmode(TRANSPARENT); // 文字字体透明    
			settextcolor(RGB(255, 0, 0));// 设定文字颜色
			settextstyle(60, 0, _T("黑体")); //  设定文字大小、样式
			outtextxy(100, 120, _T("游戏失败")); //  输出文字内容
			outtextxy(100, 180, _T("您的得分为：")); //  输出文字内容

			TCHAR s1[5];
			_stprintf_s(s1, _T("%d"), get_scores1());		// 高版本 VC 推荐使用 _stprintf_s 函数
			outtextxy(100, 240, s1); //  输出scores
			TCHAR s2[5];
			_stprintf_s(s2, _T("%d"), get_scores2());		// 高版本 VC 推荐使用 _stprintf_s 函数
			outtextxy(100, 300, s2); //  输出scores
			TCHAR s[5];
			_stprintf_s(s, _T("%d"), get_scores1() + get_scores2());		// 高版本 VC 推荐使用 _stprintf_s 函数
			outtextxy(100, 360, s); //  输出scores

			FlushBatchDraw(); // 批量绘制
			Sleep(3000);
			break;
		}
	}
	return 0;
}
