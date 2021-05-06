#include <stdio.h>
#include <windows.h> // system �Լ�, GetAsyncKeyState() �Լ�
#include <time.h> // time_h �ڷ��� �̿��Ͽ� �ð� ������ �� ��� Game_Start()�Լ����� ����.

#define BACKGROUND_WIDTH 10 // ���� �����ϴ� ����� �ʺ�
#define BACKGROUND_HEIGHT 20 // ���� �����ϴ� ����� ����

#define WALL_THICKNESS 2 // ���� �β��� ����, background_initial_x, y�� control_point_x, y�� �ʱ� ���� ������ �ش�.

#define BACKGROUND_WIDTH_TOTAL (BACKGROUND_WIDTH + (WALL_THICKNESS * 2)) // �¿� WALL_THICKNESS ĭ�� �� ���� ��� �ʺ�  
#define BACKGROUND_HEIGHT_TOTAL (BACKGROUND_HEIGHT + (WALL_THICKNESS * 2)) // ���� WALL_THICKNESS ĭ�� �� ���� ��� ����

/*
	���� ��
*/
#define RED 4 
#define ORANGE 12
#define YELLOW 14
#define GREEN 10
#define CYAN 11
#define BLUE 9
#define PURPLE 13
#define WHITE 15
#define GRAY 8

short background[BACKGROUND_HEIGHT_TOTAL][BACKGROUND_WIDTH_TOTAL]; // ���� �¿� 2ĭ�� �� ���� ��� �迭
short background_initial_x = WALL_THICKNESS; // ��� �� �� ���� �����ϴ� ����� ���� X��
short background_initial_y = WALL_THICKNESS; // ��� �� �� ���� �����ϱ� ����� ���� Y��

int control_point_x; // �� �������� x ��ǥ, Game_Start()�Լ��� ó�� �κп��� �ʱ�ȭ �ȴ�.
int control_point_y; // �� �������� y ��ǥ, Game_Start()�Լ��� ó�� �κп��� �ʱ�ȭ �ȴ�.

short check_zone_point_x, check_zone_point_y; // üũ ���� ��ǥ�ϴ� ���� x��ǥ�� y��ǥ

/* check_zone[4][4]
(0,0), (1,0), (2,0), (3,0)
(0,1), (1,1), (2,1), (3,1)
(0,2), (1,2), (2,2), (3,2)
(0,3), (1,3), (2,3), (3,3)
*/  
//O,I �� ��Ͽ� ���Ͽ� (2,1)��, S, Z, L, J, T�� ��Ͽ� ���Ͽ� (1,1)�� chechk_zone_point

short check_zone[4][4] = { {0, 0, 0, 0},
							  {0, 0, 0, 0},
							  {0, 0, 0, 0},
							  {0, 0, 0, 0} };
short check_zone_OI_size = 4; // O�� I�� üũ �� ũ��

short check_zone_SZLJT_size = 3; // S�� Z�� L�� J�� T�� üũ �� ũ��

/*
	O, I, S, Z, L, J, T �� ���� ó�� ����
*/

short type_O[4][4] = { {0,      0,      0, 0},
					   {0, YELLOW, YELLOW, 0},
					   {0, YELLOW, YELLOW, 0},
					   {0,      0,      0, 0} };

short type_I[4][4] = { {0,       0,    0,    0},
					   {CYAN, CYAN, CYAN, CYAN},
					   {0,       0,    0,    0},
					   {0,       0,    0,    0} };

short type_S[3][3] = { {0,     GREEN, GREEN},
					   {GREEN, GREEN,     0},
					   {0,         0,     0} };

short type_Z[3][3] = { {RED, RED,   0},
					   {0,   RED, RED},
					   {0,     0,   0} };

short type_L[3][3] = { {0,           0, ORANGE},
					   {ORANGE, ORANGE, ORANGE},
					   {0,           0,      0} };

short type_J[3][3] = { {BLUE,    0,    0},
					   {BLUE, BLUE, BLUE},
					   {0,       0,    0} };

short type_T[3][3] = { {0,      PURPLE,      0},
					   {PURPLE, PURPLE, PURPLE},
					   {0,           0,      0} };

int score = 0; // ����

/*
	���ڿ� ���� ������ �Լ�
*/
void FontColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


/*
	ȭ���� �����ϴ� �Լ�
*/
void Display()
{
	system("cls");

	for (int y = 0; y < BACKGROUND_HEIGHT_TOTAL; y++)
	{
		for (int x = 0; x < BACKGROUND_WIDTH_TOTAL; x++)
		{
			if (background[y][x] == 0) // ��� �迭���� 0�� �� ������ �ǹ��Ѵ�.
			{
				printf("  ");
			}
			else if (background[y][x] == GRAY)
			{
				FontColor(GRAY);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == RED)
			{
				FontColor(RED);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == YELLOW)
			{
				FontColor(YELLOW);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == ORANGE)
			{
				FontColor(ORANGE);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == GREEN)
			{
				FontColor(GREEN);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == BLUE)
			{
				FontColor(BLUE);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == CYAN)
			{
				FontColor(CYAN);
				printf("��");
				FontColor(WHITE);
			}
			else if (background[y][x] == PURPLE)
			{
				FontColor(PURPLE);
				printf("��");
				FontColor(WHITE);
			}
		}
		printf("\n");
	}
	printf("          ����: %d\n", score);
}

void Game_Start();
void Block_Renewal();
void Block_Delete();
void Overlap_Test(short type);
int Drop_Test();
void Delete_Lines();
void Game_Over_Test();
void Game_Over();


/*
	���� �Լ�
*/
int main()
{
	// �ܼ� Ŀ�� ����� �ϴ� �ڵ�... ������ ��..
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 2;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);

	Game_Start();

	return 0;
}
int block_type; // Game_Start() �Լ� �ܿ��� Block_Renewal(), Block_Delete(), Ovelap_Test() �Լ������� ���� �����̱� ������ Game_Start() ������ ����

void Game_Start()
{
	score = 0;

	/*
		ó�� ���� �� ��� �迭�� ���� ����� ������ �ֱ�
	*/
	for (int y = 0; y < BACKGROUND_HEIGHT_TOTAL; y++)
	{
		for (int x = 0; x < BACKGROUND_WIDTH_TOTAL; x++)
		{
			if ((x >= background_initial_x && x <= background_initial_x + BACKGROUND_WIDTH - 1)
				&& (y >= background_initial_y && y <= background_initial_y + BACKGROUND_HEIGHT - 1))
			{
				background[y][x] = 0;
			}
			else
			{
				background[y][x] = GRAY;
			}
		}
	}

	Display();

	short is_first = 1; // ���� �����ǰ� ó�� �������� 1, ó�� �������� ���� �ƴϸ� 0

	while (1)
	{
		time_t start_time = time(NULL);


		if (is_first == 1)
		{
			/*
				������� ���� ó�� ������ ��, control_point_x, y�� ���������� �ʱ�ȭ �Ѵ�.
				���� ������ �� �ִ� ����� ���� ���� �� ��ǥ�� (0,0)���� ������ �������� ��ǥ�� �׻� (5,1)�� �ǵ��� �Ѵ�.
			*/
			control_point_x = WALL_THICKNESS + 5;
			control_point_y = WALL_THICKNESS + 1;

			/*
				���� ó�� �����Ǿ��� �� �� ���� �������� �����Ͽ� check_zone �� �����ϱ�
			*/
			srand(time(NULL));

			block_type = rand() % 7;
			//block_type = 0; // �� ������ ���� ���Ͽ� �׽�Ʈ�� ��..

			if (block_type == 0) // O�� ����� ���
			{
				check_zone_point_x = 2, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_OI_size; y++)
				{
					for (int x = 0; x < check_zone_OI_size; x++)
					{
						check_zone[y][x] = type_O[y][x];
					}
				}
			}
			else if (block_type == 1) // I�� ����� ���
			{
				check_zone_point_x = 2, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_OI_size; y++)
				{
					for (int x = 0; x < check_zone_OI_size; x++)
					{
						check_zone[y][x] = type_I[y][x];
					}
				}
			}
			else if (block_type == 2) // S�� ����� ���
			{
				check_zone_point_x = 1, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_SZLJT_size; y++)
				{
					for (int x = 0; x < check_zone_SZLJT_size; x++)
					{
						check_zone[y][x] = type_S[y][x];
					}
				}
			}
			else if (block_type == 3) // Z�� ����� ���
			{
				check_zone_point_x = 1, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_SZLJT_size; y++)
				{
					for (int x = 0; x < check_zone_SZLJT_size; x++)
					{
						check_zone[y][x] = type_Z[y][x];
					}
				}
			}
			else if (block_type == 4) // L�� ����� ���
			{
				check_zone_point_x = 1, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_SZLJT_size; y++)
				{
					for (int x = 0; x < check_zone_SZLJT_size; x++)
					{
						check_zone[y][x] = type_L[y][x];
					}
				}
			}
			else if (block_type == 5) // J�� ����� ���
			{
				check_zone_point_x = 1, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_SZLJT_size; y++)
				{
					for (int x = 0; x < check_zone_SZLJT_size; x++)
					{
						check_zone[y][x] = type_J[y][x];
					}
				}
			}
			else if (block_type == 6) // T�� ����� ���
			{
				check_zone_point_x = 1, check_zone_point_y = 1;

				for (int y = 0; y < check_zone_SZLJT_size; y++)
				{
					for (int x = 0; x < check_zone_SZLJT_size; x++)
					{
						check_zone[y][x] = type_T[y][x];
					}
				}
			}

			Game_Over_Test();

		}

		while (1)
		{
			time_t finish_time = time(NULL);
			time_t interval = finish_time - start_time;

			/*
				��, �Ʒ�, ����, ������ ����Ű, �����̽�Ű�� ������ �� ����.
			*/
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				Overlap_Test(0);
				Display();
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				Overlap_Test(1);
				Display();
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				Overlap_Test(2);
				Display();
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				Overlap_Test(3);
				Display();
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				while (1)
				{
					if(Drop_Test() == 1)
						break;
				}
				Delete_Lines();
				Display();
				is_first = 1;
				break;
			}

			if (interval == 1)
			{
				if (Drop_Test() == 1)
				{
					Delete_Lines();
					Display();
					is_first = 1;
					break;
				}
				else 
				{
					Display();
					is_first = 0;
					break;
				}
			}
		}
	}
}

/*
	���� �����ϰ� �ִ� ����� ��濡 ���� �Լ� (check_zone�� �ִ� ���� ��濡 �������� ��´ٰ� ��������)
*/

void Block_Renewal()
{
	for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
	{
		for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
		{
			int real_point_x = x - check_zone_point_x + control_point_x;
			int real_point_y = y - check_zone_point_y + control_point_y;

			if (check_zone[y][x] != 0)
			{
				background[real_point_y][real_point_x] = check_zone[y][x];
			}
		}
	}
}

/*
	���� �����ϰ� �ִ� ����� ��濡�� ���� �Լ� (check_zone���� �� ���� ���شٰ� ��������)
*/
void Block_Delete()
{
	// (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size) �� �� ������ ���� üũ �� ũ�⸦ �����ϴ� ���� ������
	for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
	{
		for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
		{
			int real_point_x = x - check_zone_point_x + control_point_x;
			int real_point_y = y - check_zone_point_y + control_point_y;

			if (check_zone[y][x] != 0)
			{
				background[real_point_y][real_point_x] = 0;
			}
		}
	}
}

void Overlap_Test(short type)
{
	short is_overlap = 0;

	Block_Delete();

	if (type == 0)
	{
		int check_zone_next[4][4];

		/*
			���� check_zone�� ��ϵ��� ���� �ӽ� check_zone�� ȸ������ �����ϱ�
		*/
		for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
		{
			for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
			{
				check_zone_next[x][(block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size) - y - 1] = check_zone[y][x];
			}
		}

		for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
		{
			for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
			{
				int real_point_x = x - check_zone_point_x + control_point_x;
				int real_point_y = y - check_zone_point_y + control_point_y;

				if (background[real_point_y][real_point_x] != 0 && check_zone_next[y][x] != 0)
				{
					is_overlap = 1;
					break;
				}
			}
			if (is_overlap == 1)
			{
				break;
			}
		}

		if (is_overlap == 1)
		{
			/*
				���� ����ϰ� ȸ�� ���� ����� ��ģ�ٸ� �ƹ��ϵ� �Ͼ�� �ʰ� �Ͽ� ȸ������ �ʰ� �Ѵ�.
			*/
		}
		else
		{
			/*
				���� ����ϰ� ȸ�� ���� ����� ��ġ�� �ʴ´ٸ�, ���� check_zone �迭 �� ���� ������ check_zone �迭�� �����Ͽ�
				��濡 ȸ���� ����� ���� �� �ְ� �Ѵ�..
			*/

			for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
			{
				for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
				{
					check_zone[y][x] = check_zone_next[y][x];
				}
			}
		}
	}
	else if (type == 1)
	{
		control_point_y += 1;

		for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
		{
			for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
			{
				int real_point_x = x - check_zone_point_x + control_point_x;
				int real_point_y = y - check_zone_point_y + control_point_y;

				if (background[real_point_y][real_point_x] != 0 && check_zone[y][x] != 0)
				{
					is_overlap = 1;
					break;
				}
			}
			if (is_overlap == 1)
			{
				break;
			}
		}

		if (is_overlap == 1)
		{
			control_point_y -= 1;
		}
	}
	else if (type == 2)
	{
		control_point_x -= 1;

		for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
		{
			for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
			{
				int real_point_x = x - check_zone_point_x + control_point_x;
				int real_point_y = y - check_zone_point_y + control_point_y;

				if (background[real_point_y][real_point_x] != 0 && check_zone[y][x] != 0)
				{
					is_overlap = 1;
					break;
				}
			}
			if (is_overlap == 1)
			{
				break;
			}
		}

		if (is_overlap == 1)
		{
			control_point_x += 1;
		}
	}
	else if (type == 3)
	{
		control_point_x += 1;

		for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
		{
			for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
			{
				int real_point_x = x - check_zone_point_x + control_point_x;
				int real_point_y = y - check_zone_point_y + control_point_y;

				if (background[real_point_y][real_point_x] != 0 && check_zone[y][x] != 0)
				{
					is_overlap = 1;
					break;
				}
			}
			if (is_overlap == 1)
			{
				break;
			}
		}

		if (is_overlap == 1)
		{
			control_point_x -= 1;
		}
	}

	Block_Renewal();
}

int Drop_Test()
{
	short is_overlap = 0;

	Block_Delete();

	control_point_y += 1;

	for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
	{
		for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
		{
			int real_point_x = x - check_zone_point_x + control_point_x;
			int real_point_y = y - check_zone_point_y + control_point_y;

			if (background[real_point_y][real_point_x] != 0 && check_zone[y][x] != 0)
			{
				is_overlap = 1;
				break;
			}
		}
		if (is_overlap == 1)
		{
			break;
		}
	}

	if (is_overlap == 1)
	{
		control_point_y -= 1;
		Block_Renewal();
		return 1;
	}
	else
	{
		Block_Renewal();
		return 0;
	}
}

/*
	ä���� �� ���ִ� �Լ�
*/

void Delete_Lines()
{
	int next_background[BACKGROUND_HEIGHT][BACKGROUND_WIDTH]; // ���� ���

	/*
		next_background�� 0���� ä���ֱ�
	*/
	for (int y = 0; y < BACKGROUND_HEIGHT; y++) // ��
	{
		for (int x = 0; x < BACKGROUND_WIDTH; x++) // ��
		{
			next_background[y][x] = 0;
		}
	}

	/*
		ä���� �� ���ִ� �ݺ���
	*/

	for (int y = 0; y < BACKGROUND_HEIGHT; y++) // ��
	{
		for (int x = 0; x < BACKGROUND_WIDTH; x++) // ��
		{
			if (background[background_initial_y + y][background_initial_x + x] == 0)
			{
				break;
			}

			if (x == BACKGROUND_WIDTH - 1)
			{
				for (int i = 0; i < BACKGROUND_WIDTH; i++)
				{
					background[background_initial_y + y][background_initial_x + i] = 0;
				}
				score += 100;
			}
		}
	}

	/*
		�����ִ� ���� ������ �״� �ݺ���
	*/

	int stack_count = 0;

	for (int y = BACKGROUND_HEIGHT - 1; y >= 0; y--) // ��
	{
		int empty_count = 0;

		for (int x = 0; x < BACKGROUND_WIDTH; x++) // ��
		{
			if (background[background_initial_y + y][background_initial_x + x] == 0)
			{
				empty_count += 1;
				/*
					����ִ� ���� stack_count�� ������ �ʰ��Ѵ�.
				*/
				if (empty_count == BACKGROUND_WIDTH)
				{
					break;
				}
			}
			else
			{
				/*
					��� ���� ���� ���� stack_count �� 1 �����ϰ� �Ͽ� ��� ������ ����Ѵ�.
				*/

				stack_count += 1;

				for (int i = 0; i < BACKGROUND_WIDTH; i++)
				{
					next_background[BACKGROUND_HEIGHT - stack_count][i] = background[background_initial_y + y][background_initial_x + i];
				}
				break;
			}
		}
	}

	for (int y = 0; y < BACKGROUND_HEIGHT; y++) // ��
	{
		for (int x = 0; x < BACKGROUND_WIDTH; x++) // ��
		{
			background[background_initial_y + y][background_initial_x + x] = next_background[y][x];
		}
	}
}

void Game_Over_Test()
{
	short is_overlap = 0;

	for (int y = 0; y < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); y++)
	{
		for (int x = 0; x < (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size); x++)
		{
			int real_point_x = x - check_zone_point_x + control_point_x;
			int real_point_y = y - check_zone_point_y + control_point_y;

			if (background[real_point_y][real_point_x] != 0 && check_zone[y][x] != 0)
			{
				if (background[real_point_y][real_point_x] == GRAY)
				{
					Block_Renewal();
					Display();
					Game_Over();
				}

				is_overlap = 1;
				break;
			}
		}
		if (is_overlap == 1)
		{
			break;
		}
	}

	if (is_overlap == 1)
	{
		control_point_y -= 1;
		Game_Over_Test();
	}
	Block_Renewal();
	Display();
}

void Game_Over()
{

	printf("          GameOver\n");
	printf("      �ٽ� ����: ENTER");

	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			Game_Start();
		}
	}
}