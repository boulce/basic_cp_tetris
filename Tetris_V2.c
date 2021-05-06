#include <stdio.h>
#include <windows.h> // system 함수, GetAsyncKeyState() 함수
#include <time.h> // time_h 자료형 이용하여 시간 측정할 때 사용 Game_Start()함수에서 사용됨.

#define BACKGROUND_WIDTH 10 // 블럭이 존재하는 배경의 너비
#define BACKGROUND_HEIGHT 20 // 블럭이 존재하는 배경의 높이

#define WALL_THICKNESS 2 // 벽의 두께를 조정, background_initial_x, y와 control_point_x, y의 초기 값에 영향을 준다.

#define BACKGROUND_WIDTH_TOTAL (BACKGROUND_WIDTH + (WALL_THICKNESS * 2)) // 좌우 WALL_THICKNESS 칸씩 벽 포함 배경 너비  
#define BACKGROUND_HEIGHT_TOTAL (BACKGROUND_HEIGHT + (WALL_THICKNESS * 2)) // 상하 WALL_THICKNESS 칸씩 벽 포함 배경 높이

/*
	색깔 값
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

short background[BACKGROUND_HEIGHT_TOTAL][BACKGROUND_WIDTH_TOTAL]; // 상하 좌우 2칸씩 벽 포함 배경 배열
short background_initial_x = WALL_THICKNESS; // 배경 블럭 중 블럭이 존재하는 배경의 시작 X값
short background_initial_y = WALL_THICKNESS; // 배경 블럭 중 블럭이 존재하기 배경의 시작 Y값

int control_point_x; // 블럭 조작점의 x 좌표, Game_Start()함수의 처음 부분에서 초기화 된다.
int control_point_y; // 블럭 조작점의 y 좌표, Game_Start()함수의 처음 부분에서 초기화 된다.

short check_zone_point_x, check_zone_point_y; // 체크 존을 대표하는 점의 x좌표와 y좌표

/* check_zone[4][4]
(0,0), (1,0), (2,0), (3,0)
(0,1), (1,1), (2,1), (3,1)
(0,2), (1,2), (2,2), (3,2)
(0,3), (1,3), (2,3), (3,3)
*/  
//O,I 형 블록에 대하여 (2,1)이, S, Z, L, J, T형 블록에 대하여 (1,1)이 chechk_zone_point

short check_zone[4][4] = { {0, 0, 0, 0},
							  {0, 0, 0, 0},
							  {0, 0, 0, 0},
							  {0, 0, 0, 0} };
short check_zone_OI_size = 4; // O형 I형 체크 존 크기

short check_zone_SZLJT_size = 3; // S형 Z형 L형 J형 T형 체크 존 크기

/*
	O, I, S, Z, L, J, T 각 블럭의 처음 상태
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

int score = 0; // 점수

/*
	글자에 색깔 입히는 함수
*/
void FontColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


/*
	화면을 갱신하는 함수
*/
void Display()
{
	system("cls");

	for (int y = 0; y < BACKGROUND_HEIGHT_TOTAL; y++)
	{
		for (int x = 0; x < BACKGROUND_WIDTH_TOTAL; x++)
		{
			if (background[y][x] == 0) // 배경 배열에서 0은 빈 공간을 의미한다.
			{
				printf("  ");
			}
			else if (background[y][x] == GRAY)
			{
				FontColor(GRAY);
				printf("■");
				FontColor(WHITE);
			}
			else if (background[y][x] == RED)
			{
				FontColor(RED);
				printf("▣");
				FontColor(WHITE);
			}
			else if (background[y][x] == YELLOW)
			{
				FontColor(YELLOW);
				printf("▣");
				FontColor(WHITE);
			}
			else if (background[y][x] == ORANGE)
			{
				FontColor(ORANGE);
				printf("▣");
				FontColor(WHITE);
			}
			else if (background[y][x] == GREEN)
			{
				FontColor(GREEN);
				printf("▣");
				FontColor(WHITE);
			}
			else if (background[y][x] == BLUE)
			{
				FontColor(BLUE);
				printf("▣");
				FontColor(WHITE);
			}
			else if (background[y][x] == CYAN)
			{
				FontColor(CYAN);
				printf("▣");
				FontColor(WHITE);
			}
			else if (background[y][x] == PURPLE)
			{
				FontColor(PURPLE);
				printf("▣");
				FontColor(WHITE);
			}
		}
		printf("\n");
	}
	printf("          점수: %d\n", score);
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
	메인 함수
*/
int main()
{
	// 콘솔 커서 숨기게 하는 코드... 문법은 모름..
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 2;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);

	Game_Start();

	return 0;
}
int block_type; // Game_Start() 함수 외에도 Block_Renewal(), Block_Delete(), Ovelap_Test() 함수에서도 쓰일 변수이기 때문에 Game_Start() 밖으로 뺐음

void Game_Start()
{
	score = 0;

	/*
		처음 시작 때 배경 배열에 벽과 빈공간 데이터 넣기
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

	short is_first = 1; // 블럭이 생성되고 처음 떨어지면 1, 처음 떨어지는 것이 아니면 0

	while (1)
	{
		time_t start_time = time(NULL);


		if (is_first == 1)
		{
			/*
				만들어진 블럭이 처음 떨어질 때, control_point_x, y를 시작점으로 초기화 한다.
				블럭이 존재할 수 있는 배경의 가장 왼쪽 위 좌표를 (0,0)으로 잡으면 시작점의 좌표는 항상 (5,1)이 되도록 한다.
			*/
			control_point_x = WALL_THICKNESS + 5;
			control_point_y = WALL_THICKNESS + 1;

			/*
				블럭이 처음 생성되었을 때 블럭 종류 랜덤으로 선택하여 check_zone 에 대입하기
			*/
			srand(time(NULL));

			block_type = rand() % 7;
			//block_type = 0; // 각 종류의 블럭에 대하여 테스트할 떄..

			if (block_type == 0) // O형 블록인 경우
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
			else if (block_type == 1) // I형 블록인 경우
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
			else if (block_type == 2) // S형 블록인 경우
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
			else if (block_type == 3) // Z형 블록인 경우
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
			else if (block_type == 4) // L형 블록인 경우
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
			else if (block_type == 5) // J형 블록인 경우
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
			else if (block_type == 6) // T형 블록인 경우
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
				위, 아래, 왼쪽, 오른쪽 방향키, 스페이스키를 눌렀을 때 반응.
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
	현재 조작하고 있는 블록을 배경에 찍어내는 함수 (check_zone에 있는 블럭을 배경에 도장으로 찍는다고 생각하자)
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
	현재 조작하고 있는 블록을 배경에서 뗴어내는 함수 (check_zone으로 찍어낸 블럭을 없앤다고 생각하자)
*/
void Block_Delete()
{
	// (block_type <= 1 ? check_zone_OI_size : check_zone_SZLJT_size) 는 블럭 종류에 따라 체크 존 크기를 결정하는 조건 연산자
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
			현재 check_zone의 블록들을 다음 임시 check_zone에 회전시켜 대입하기
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
				만약 배경블록과 회전 후의 블록이 겹친다면 아무일도 일어나지 않게 하여 회전하지 않게 한다.
			*/
		}
		else
		{
			/*
				만약 배경블록과 회전 후의 블록이 겹치지 않는다면, 현재 check_zone 배열 에 다음 상태의 check_zone 배열을 대입하여
				배경에 회전한 블록이 찍힐 수 있게 한다..
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
	채워진 줄 없애는 함수
*/

void Delete_Lines()
{
	int next_background[BACKGROUND_HEIGHT][BACKGROUND_WIDTH]; // 다음 배경

	/*
		next_background를 0으로 채워넣기
	*/
	for (int y = 0; y < BACKGROUND_HEIGHT; y++) // 행
	{
		for (int x = 0; x < BACKGROUND_WIDTH; x++) // 열
		{
			next_background[y][x] = 0;
		}
	}

	/*
		채워진 줄 없애는 반복문
	*/

	for (int y = 0; y < BACKGROUND_HEIGHT; y++) // 행
	{
		for (int x = 0; x < BACKGROUND_WIDTH; x++) // 열
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
		남아있는 줄을 밑으로 쌓는 반복문
	*/

	int stack_count = 0;

	for (int y = BACKGROUND_HEIGHT - 1; y >= 0; y--) // 행
	{
		int empty_count = 0;

		for (int x = 0; x < BACKGROUND_WIDTH; x++) // 열
		{
			if (background[background_initial_y + y][background_initial_x + x] == 0)
			{
				empty_count += 1;
				/*
					비어있는 줄은 stack_count가 변하지 않게한다.
				*/
				if (empty_count == BACKGROUND_WIDTH)
				{
					break;
				}
			}
			else
			{
				/*
					비어 있지 않은 줄은 stack_count 가 1 증가하게 하여 어디에 쌓일지 계산한다.
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

	for (int y = 0; y < BACKGROUND_HEIGHT; y++) // 행
	{
		for (int x = 0; x < BACKGROUND_WIDTH; x++) // 열
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
	printf("      다시 시작: ENTER");

	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			Game_Start();
		}
	}
}