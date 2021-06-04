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
// ȫ�ֱ�������
static int Blocks[HEIGHT][WIDTH] = { 0 }; //  ��ά���飬���ڼ�¼���е���Ϸ����
static char moveDirection;  //  С���ƶ�����
static int food_i, food_j, value; //  ʳ���λ��
static int isFailure = 0; //  �Ƿ���Ϸʧ��
static int speed = 10;
static int scores = 0;

static int get_scores() {
	int snake_length = 0;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 40; j++) {
			if (Blocks[i][j] > 0)
				snake_length++;
		}
	}
	float extra = 100 / speed;
	scores = scores + static_cast<int>((snake_length - 5) * extra);
	return scores;
}

static void set_speed(int speed0) {
	speed = speed0;
}

static void moveSnake() //  �ƶ�С�߼���ش�����
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
	if (moveDirection == 'w') // �����ƶ�
		newHead_i = oldHead_i - 1;
	else if (moveDirection == 's') // �����ƶ�
		newHead_i = oldHead_i + 1;
	else if (moveDirection == 'a') // �����ƶ�
		newHead_j = oldHead_j - 1;
	else if (moveDirection == 'd') // �����ƶ�
		newHead_j = oldHead_j + 1;

	//  �����ͷ�����߽磬������ͷ����������Ϸʧ��
	if (newHead_i >= HEIGHT || newHead_i < 0 || newHead_j >= WIDTH || newHead_j < 0
		|| Blocks[newHead_i][newHead_j]>0)
	{
		isFailure = 1;
		return; // ��������
	}

	Blocks[newHead_i][newHead_j] = 1;  // ����ͷλ����ֵΪ1	
	if (newHead_i == food_i && newHead_j == food_j) //  �������ͷ��������ʳ��
	{
		srand((unsigned)time(NULL));
		scores = scores + value;
		food_i = rand() % (HEIGHT - 5) + 2; //  ʳ���������λ��
		food_j = rand() % (WIDTH - 5) + 2;
		value = rand() % (10);
		// ���Ծ���β�����൱���ߵĳ���+1
	}
	else // ����ͷû������ʳ��
		Blocks[oldTail_i][oldTail_j] = 0; // ����β��ɿհף�����ʳ��ʱ�����ߵĳ��Ȳ���
}

static void startup()  //  ��ʼ������
{
	int i;
	Blocks[HEIGHT / 2][WIDTH / 2] = 1; // �����м仭��ͷ������Ϊ1
	for (i = 1; i <= 4; i++) //  ��������4��������ֵ����Ϊ2,3,4,5
		Blocks[HEIGHT / 2][WIDTH / 2 - i] = i + 1;
	moveDirection = 'd';	 //  ��ʼ�����ƶ�
	food_i = rand() % (HEIGHT - 5) + 2; //  ��ʼ�����ʳ��λ��
	food_j = rand() % (WIDTH - 5) + 2; //  
	value = rand() % (10);
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
				setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
			else
				setfillcolor(RGB(150, 150, 150)); // Ԫ��Ϊ0��ʾΪ�գ���ɫΪ��ɫ
			// �ڶ�Ӧλ�ô����Զ�Ӧ��ɫ����С����
			fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE,
				(j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
		}
	}
	setfillcolor(RGB(0, (food_i - food_j) * 6, 0)); //  ʳ��Ϊ��ɫ
	//  ����ʳ��С����
	fillrectangle(food_j * BLOCK_SIZE, food_i * BLOCK_SIZE,
		(food_j + 1) * BLOCK_SIZE, (food_i + 1) * BLOCK_SIZE);
	FlushBatchDraw(); // ��������
}


/*Avoid self collision & double direction acceleration system*/
static void updateWithInput()  // �������йصĸ��º���
{
	if (_kbhit() && isFailure == 0)  //  ����а������룬���Ҳ�ʧ��
	{
		char input = _getch(); //  ��ð�������
		if (input == 'a' && 'd' != moveDirection) // �����asdw 
		{
			moveDirection = input;  // �趨�ƶ�����
		}
		else if (input == 's' && 'w' != moveDirection) // �����asdw 
		{
			moveDirection = input;  // �趨�ƶ�����
		}
		else if (input == 'd' && 'a' != moveDirection) // �����asdw 
		{
			moveDirection = input;  // �趨�ƶ�����
		}
		else if (input == 'w' && 's' != moveDirection) // �����asdw 
		{
			moveDirection = input;  // �趨�ƶ�����
		}
		moveSnake(); // ����С���ƶ�����
	}
}

int main1() //  ������
{
	startup();  // ��ʼ����������ִ��һ��	
	// ������
	mciSendString(_T("open music.mp3 alias mymusic"), NULL, 0, NULL);
	// ��������
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
	//�����ٶȴ���
		// ��ʼ����ͼ����
	initgraph(800, 600);
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
			moveSnake(); //  ����С���ƶ�����
			waitIndex = 1; // �ٱ��1
		}
		updateWithInput();    // �������йصĸ���
		if (isFailure) //  �����Ϸʧ��
		{
			setbkmode(TRANSPARENT); // ��������͸��    
			settextcolor(RGB(255, 0, 0));// �趨������ɫ
			settextstyle(60, 0, _T("����")); //  �趨���ִ�С����ʽ
			outtextxy(100, 120, _T("��Ϸʧ��")); //  �����������
			outtextxy(100, 180, _T("���ĵ÷�Ϊ��")); //  �����������
			TCHAR s[5];
			_stprintf_s(s, _T("%d"), get_scores());		// �߰汾 VC �Ƽ�ʹ�� _stprintf_s ����
			outtextxy(100, 240, s); //  ���scores
			FlushBatchDraw(); // ��������
			Sleep(3000);
			break;
		}
	}
	return 0;
}
