#include <graphics.h>  
#include <conio.h>
#include <stdio.h>
#include <string>
#include <time.h>
#pragma comment(lib,"Winmm.lib")
constexpr auto BLOCK_SIZE = 20; // ÿ��С���ӵĳ����С ;
constexpr auto HEIGHT = 30; // �߶���һ��30��С����;
constexpr auto WIDTH = 40; //  �����һ��40��С���� ;
/*��ײϵͳ+����ϵͳ+��������ӣ���ʼ���ǹ̶��ģ�+ʳ����������������Ͷ�*/
/*˫�˺���ģʽ�У����й����ߵĴ洢������Ҫ���ݣ�ʳ���ͬʱ���ֶ��*/
// ȫ�ֱ�������
static int Blocks[HEIGHT][WIDTH] = { 0 }; //  ��ά���飬���ڼ�¼���е���Ϸ���ݣ���1��NodeΪ����
static char moveDirection1;  //  С���ƶ�����
static char moveDirection2;  //  С���ƶ�����

static int food_i1, food_j1, value1; //  ʳ���λ��
static int food_i2, food_j2, value2; //  ʳ���λ��
static int isFailure = 0; //  �Ƿ���Ϸʧ��
static int speed;

//���ԵĻ���
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

static void moveSnake1() //  �ƶ�С�߼���ش�����
{
	int i, j;
	for (i = 0; i < HEIGHT; i++) // ���б��� 
		for (j = 0; j < WIDTH; j++) // ���б���
			if (Blocks[i][j] > 0) // ����0��ΪС��Ԫ�� 
				Blocks[i][j]++; //  ����+1
	int oldTail_i, oldTail_j, oldHead_i, oldHead_j; // ����������洢����β������ͷ����  
	int max = 0; // ���ڼ�¼���ֵ 
	for (i = 0; i < HEIGHT; i++) //  �����б���
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (max < Blocks[i][j]) //  �����ǰԪ��ֵ��max��
			{
				max = Blocks[i][j]; // ����max��ֵ
				oldTail_i = i; //  ��¼���ֵ�����꣬���Ǿ���β��λ��
				oldTail_j = j; //  
			}
			if (Blocks[i][j] == 2) // �ҵ���ֵΪ2 
			{
				oldHead_i = i; //  ��ֵΪ2ǡ���Ǿ���ͷ��λ��
				oldHead_j = j; //  
			}
		}
	}
	int newHead_i = oldHead_i; //  �趨�����洢����ͷ��λ��
	int newHead_j = oldHead_j;

	//  �����û��������趨����ͷ��λ��
	if (moveDirection1 == 'w') // �����ƶ�
		newHead_i = oldHead_i - 1;
	else if (moveDirection1 == 's') // �����ƶ�
		newHead_i = oldHead_i + 1;
	else if (moveDirection1 == 'a') // �����ƶ�
		newHead_j = oldHead_j - 1;
	else if (moveDirection1 == 'd') // �����ƶ�
		newHead_j = oldHead_j + 1;

	//  �����ͷ�����߽磬������ͷ����������Ϸʧ��
	if (newHead_i >= HEIGHT || newHead_i < 0 || newHead_j >= WIDTH || newHead_j < 0
		|| Blocks[newHead_i][newHead_j]>0)
	{
		isFailure = 1;
		return; // ��������
	}
	else if (Blocks[newHead_i][newHead_j] < 0) {
		isFailure = 3;
		return;
	}

	Blocks[newHead_i][newHead_j] = 1;  // ����ͷλ����ֵΪ1	
	if (newHead_i == food_i1 && newHead_j == food_j1) //  �������ͷ��������ʳ��
	{
		srand((unsigned)time(NULL));
		scores1 = scores1 + value1;
		food_i1 = rand() % (HEIGHT - 5) + 2; //  ʳ���������λ��
		food_j1 = rand() % (WIDTH - 5) + 2;
		value1 = rand() % (10);
		// ���Ծ���β�����൱���ߵĳ���+1
	}
	else // ����ͷû������ʳ��
		Blocks[oldTail_i][oldTail_j] = 0; // ����β��ɿհף�����ʳ��ʱ�����ߵĳ��Ȳ���
}
static void moveSnake2() //  �ƶ�С�߼���ش�����
{
	int i, j;
	for (i = 0; i < HEIGHT; i++) // ���б��� 
		for (j = 0; j < WIDTH; j++) // ���б���
			if (Blocks[i][j] < 0)
				Blocks[i][j]--; //  ����-1
	int oldTail_i, oldTail_j, oldHead_i, oldHead_j; // ����������洢����β������ͷ����  
	int min = 0; // ���ڼ�¼��Сֵ 
	for (i = 0; i < HEIGHT; i++) //  �����б���
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (min > Blocks[i][j]) //  �����ǰԪ��ֵ��maxС
			{
				min = Blocks[i][j]; // ����min��ֵ
				oldTail_i = i; //  ��¼���꣬���Ǿ���β��λ��
				oldTail_j = j;
			}
			if (Blocks[i][j] == -2)
			{
				oldHead_i = i;
				oldHead_j = j;
			}
		}
	}
	int newHead_i = oldHead_i; //  �趨�����洢����ͷ��λ��
	int newHead_j = oldHead_j;

	//  �����û��������趨����ͷ��λ��
	if (moveDirection2 == '5') // �����ƶ�
		newHead_i = oldHead_i - 1;
	else if (moveDirection2 == '2') // �����ƶ�
		newHead_i = oldHead_i + 1;
	else if (moveDirection2 == '1') // �����ƶ�
		newHead_j = oldHead_j - 1;
	else if (moveDirection2 == '3') // �����ƶ�
		newHead_j = oldHead_j + 1;

	//  �����ͷ�����߽磬������ͷ����������Ϸʧ��
	if (newHead_i >= HEIGHT || newHead_i < 0 || newHead_j >= WIDTH || newHead_j < 0
		|| Blocks[newHead_i][newHead_j] < 0)
	{
		isFailure = 2;
		return; // ��������
	}
	else if (Blocks[newHead_i][newHead_j] > 0) {
		isFailure = 4;
		return;
	}

	Blocks[newHead_i][newHead_j] = -1;  // ����ͷλ����ֵΪ1	
	if (newHead_i == food_i2 && newHead_j == food_j2) //  �������ͷ��������ʳ��
	{
		srand((unsigned)time(NULL));
		scores2 = scores2 + value2;
		food_i2 = rand() % (HEIGHT - 5) + 2; //  ʳ���������λ��
		food_j2 = rand() % (WIDTH - 5) + 2;
		value2 = rand() % (10);
		// ���Ծ���β�����൱���ߵĳ���+1
	}
	else // ����ͷû������ʳ��
		Blocks[oldTail_i][oldTail_j] = 0; // ����β��ɿհף�����ʳ��ʱ�����ߵĳ��Ȳ���
}

static void startup()  //  ��ʼ������
{
	int i;
	//initialize Snake 1
	Blocks[HEIGHT / 4][WIDTH / 4] = 1; // �����м仭��ͷ������Ϊ1
	for (i = 1; i <= 4; i++) //  ��������4��������ֵ����Ϊ2,3,4,5
		Blocks[HEIGHT / 4][WIDTH / 4 - i] = i + 1;
	moveDirection1 = 'd';	 //  ��ʼ�����ƶ�

	//initialize Snake 2
	Blocks[3 * HEIGHT / 4][3 * WIDTH / 4] = -1; // �����м仭��ͷ������Ϊ-1
	for (i = 1; i <= 4; i++) //  ��������4��������ֵ����Ϊ2,3,4,5
		Blocks[3 * HEIGHT / 4][3 * WIDTH / 4 + i] = -(i + 1);
	moveDirection2 = '1';	 //  ��ʼ�����ƶ�

	//initialize the food 1&2
	food_i1 = rand() % (HEIGHT - 5) + 2; //  ��ʼ�����ʳ��λ��
	food_j1 = rand() % (WIDTH - 5) + 2; //  
	value1 = rand() % (10);
	food_i2 = rand() % (HEIGHT - 5) + 2; //  ��ʼ�����ʳ��λ��
	food_j2 = rand() % (WIDTH - 5) + 2; //  
	value2 = rand() % (10);
	initgraph(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE); //  �¿�����
	setlinecolor(RGB(200, 200, 200)); // ����������ɫ
	BeginBatchDraw(); // ��ʼ��������
}

static void show()  // ���ƺ���
{
	cleardevice(); // ����
	int i, j;
	for (i = 0; i < HEIGHT; i++) //  �Զ�ά��������Ԫ�ر���
	{
		for (j = 0; j < WIDTH; j++)
		{
			if (Blocks[i][j] > 0) // Ԫ�ش���0��ʾ���ߣ��������ߵ�������ɫɫ������
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 0.9));
			else if (Blocks[i][j] < 0)
				setfillcolor(HSVtoRGB(-(Blocks[i][j] * 10 - 72), 0.9, 0.9));
			else
				setfillcolor(RGB(150, 150, 150)); // Ԫ��Ϊ0��ʾΪ�գ���ɫΪ��ɫ
			// �ڶ�Ӧλ�ô����Զ�Ӧ��ɫ����С����
			fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE,
				(j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
		}
	}

	setfillcolor(RGB(0, (food_i1 - food_j1) * 6, 0)); //  ʳ��Ϊ��ɫ
	//  ����ʳ��С����1
	fillrectangle(food_j1 * BLOCK_SIZE, food_i1 * BLOCK_SIZE,
		(food_j1 + 1) * BLOCK_SIZE, (food_i1 + 1) * BLOCK_SIZE);

	setfillcolor(RGB(255, (food_i2 - food_j2) * 6, 255)); //  ʳ��Ϊ��ɫ
	//  ����ʳ��С����2
	fillrectangle(food_j2 * BLOCK_SIZE, food_i2 * BLOCK_SIZE,
		(food_j2 + 1) * BLOCK_SIZE, (food_i2 + 1) * BLOCK_SIZE);

	FlushBatchDraw(); // ��������
}


/*Avoid self collision & double direction acceleration system*/
static void updateWithInput()  // �������йصĸ��º���
{
	if (_kbhit() && isFailure == 0)  //  ����а������룬���Ҳ�ʧ��
	{
		char input = _getch(); //  ��ð�������
		if (input == 'a' && 'd' != moveDirection1) // �����asdw 
		{
			moveDirection1 = input;  // �趨�ƶ�����
		}
		else if (input == 's' && 'w' != moveDirection1) // �����asdw 
		{
			moveDirection1 = input;  // �趨�ƶ�����
		}
		else if (input == 'd' && 'a' != moveDirection1) // �����asdw 
		{
			moveDirection1 = input;  // �趨�ƶ�����
		}
		else if (input == 'w' && 's' != moveDirection1) // �����asdw 
		{
			moveDirection1 = input;  // �趨�ƶ�����
		}
		if (input == 'w' || input == 'a' || input == 's' || input == 'd') {
			moveSnake1();
		}
		if (input == '2' && '5' != moveDirection2) // �����asdw 
		{
			moveDirection2 = input;  // �趨�ƶ�����
		}
		else if (input == '3' && '1' != moveDirection2) // �����asdw 
		{
			moveDirection2 = input;  // �趨�ƶ�����
		}
		else if (input == '5' && '2' != moveDirection2) // �����asdw 
		{
			moveDirection2 = input;  // �趨�ƶ�����
		}
		else if (input == '1' && '3' != moveDirection2) // �����asdw 
		{
			moveDirection2 = input;  // �趨�ƶ�����
		}
		if (input == '1' || input == '2' || input == '3' || input == '5') {
			moveSnake2();
		}
	}
}
int main() //  ������
{
	startup();  // ��ʼ����������ִ��һ��	
	// ������
	mciSendString(_T("open music.mp3 alias mymusic"), NULL, 0, NULL);
	// ��������
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);

	//�����ٶȴ���
		// ��ʼ����ͼ����
	initgraph(800,600);
	wchar_t s[5];
	InputBox(s, 10, L"�������Ѷȵȼ� ��0/1/2/3/4/5��");
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

	while (1)   // һֱѭ��
	{
		show();  // ���л���
		static int waitIndex = 1; // ��̬�ֲ���������ʼ��ʱΪ1
		waitIndex++; // ÿһ֡+1
		if (waitIndex == speed) // ����speed֡��ִ��
		{
			moveSnake1(); //  ����С���ƶ�����
			moveSnake2();
			waitIndex = 1; // �ٱ��1
		}
		updateWithInput();    // �������йصĸ���   
		if (isFailure == 1 || isFailure == 2 || isFailure == 3 || isFailure == 4) //  �����Ϸʧ�� isFailure =1||2||3||4
		{
			setbkmode(TRANSPARENT); // ��������͸��    
			settextcolor(RGB(255, 0, 0));// �趨������ɫ
			settextstyle(60, 0, _T("����")); //  �趨���ִ�С����ʽ
			outtextxy(100, 120, _T("��Ϸʧ��")); //  �����������
			outtextxy(100, 180, _T("���ĵ÷�Ϊ��")); //  �����������

			TCHAR s1[5];
			_stprintf_s(s1, _T("%d"), get_scores1());		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
			outtextxy(100, 240, s1); //  ���scores
			TCHAR s2[5];
			_stprintf_s(s2, _T("%d"), get_scores2());		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
			outtextxy(100, 300, s2); //  ���scores
			TCHAR s[5];
			_stprintf_s(s, _T("%d"), get_scores1() + get_scores2());		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
			outtextxy(100, 360, s); //  ���scores

			FlushBatchDraw(); // ��������
			Sleep(3000);
			break;
		}
	}
	return 0;
}
