#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define	CURRENT_PUZZLE_PATH				"data/puzzles/current_puzzle.txt"
#define	FOUR_BOX_PUZZLES_FILE_PATH		"data/puzzles/4boxes.txt"
#define	FIVE_BOX_PUZZLES_FILE_PATH		"data/puzzles/5boxes.txt"

#define N 4


char init_board[N][N] = {
	{'.', '.', '.', '.'},
	{'.', '.', '.', '.'},
	{'.', '.', '.', '.'},
	{'.', '.', '.', '.'}
};

void populateBoardWith4(char board[N][N]) {
	FILE* file = fopen(FOUR_BOX_PUZZLES_FILE_PATH, "r");
    
    if (!file) {
    	printf("File not opened!\nQuitting...\n");
    	return;
	}
    
    
    srand(time(NULL));
    int random_line = rand() % 256 + 1;
	
	char line[9], copy_line[9];
    int i = 0;
	
    while (fgets(line, sizeof(line), file)) {
        i++;
        
        if(i == random_line)
        {
            strcpy(copy_line, line);
        }
    }
    fclose(file);
 	
 	init_board[0][0] = copy_line[0];
 	init_board[0][1] = copy_line[2];
 	init_board[0][2] = copy_line[4];
 	init_board[0][3] = copy_line[6];
}

void populateBoardWith5(char board[N][N]) {
	FILE* file = fopen(FIVE_BOX_PUZZLES_FILE_PATH, "r");
    
    if (!file) {
    	printf("File not opened!\nQuitting...\n");
    	return;
	}
    
    
    srand(time(NULL));
    int random_line = rand() % 1020 + 1; // 1020 combo dans le fichier
	
	char line[12], copy_line[12];
    int i = 0;
	
    while (fgets(line, sizeof(line), file)) {
        i++;
        
        if(i == random_line)
        {
            strcpy(copy_line, line);
        }
    }
    fclose(file);
 	init_board[0][0] = copy_line[0];
 	init_board[0][1] = copy_line[2];
 	init_board[0][2] = copy_line[4];
 	init_board[0][3] = copy_line[6];
 	init_board[1][0] = copy_line[8];
 	
}

/* 
 * This function rotates the board 90°. 
 * Input:
 *	 1  2  3
 *	 4  5  6
 *	 7  8  9
 *
 * Output:
 *   3  6  9 
 *   2  5  8 
 *   1  4  7
 */
void rotateBoard90(char mat[N][N]) {
    for (int x = 0; x < N / 2; x++) {
        for (int y = x; y < N-x-1; y++) {
            int temp = mat[x][y];
            mat[x][y] = mat[y][N-1-x]; 
            mat[y][N-1-x] = mat[N-1-x][N-1-y];
            mat[N-1-x][N-1-y] = mat[N-1-y][x];
            mat[N-1-y][x] = temp; 
        } 
    } 
}

/* 
 * This function rotates the board 180° clockwise. 
 * Input:
 *	 1  2  3  4
 *   5  6  7  8
 *   9  10 11 12
 *   13 14 15 16
 *
 * Output:
 *   16 15 14 13 
 *   12 11 10 9 
 *   8  7  6  5 
 *   4  3  2  1
 */
void rotateBoard180(char arr[N][N]) {
	int temp;
    for (int i = 0; i < N; i++) {
        for (int j = 0, k = N - 1; j < k; j++, k--) {
            temp = arr[j][i];
            arr[j][i] = arr[k][i];
            arr[k][i] = temp;
		}
	}
}

// transpose of a board
void transpose(char arr[N][N]) {
	int temp;
    for (int i = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            temp = arr[i][j];
            arr[i][j] = arr[j][i];
            arr[j][i] = temp;
    	}
	}
} 

void printBoard(char mat[N][N]) { 
    for (int i = 0; i < N; i++) { 
        for (int j = 0; j < N; j++) 
            printf("  %c ", mat[i][j]); 
            printf("\n"); 
    }
    printf("\n"); 
}

void rotateBoard180AntiClockwise(
							     char board[N][N],
							     void (*ptrTranspose)(char[N][N]),
							     void (*ptrRotateBoard180)(char[N][N])
							     ) 
{
	(*ptrTranspose)(board);
	(*ptrRotateBoard180)(board);
	(*ptrTranspose)(board);
	(*ptrRotateBoard180)(board);
}

void reverseCols(char board[N][N], char col1, char col2) {
	int temp;
	for (int row = 0; row < N; row++) {
		temp = board[row][col1];
		board[row][col1] = board[row][col2];
		board[row][col2] = temp;
	}
}

void reverseRows(char board[N][N], char row1, char row2) {
	int temp;
	for (int col = 0; col < N; col++) {
		temp = board[row1][col];
		board[row1][col] = board[row2][col];
		board[row2][col] = temp;
	}
}

void changeBoxesRandomly(char board[N][N], int row1, int col1, int row2, int col2) {
	int temp = board[row1][col1];
	board[row1][col1] = board[row2][col2];
	board[row2][col2] = temp;
}

int count(char c, char square[N]) {
	int count = 0;
	for(int i = 0; i < N; i++) {
		if (square[i] == c)
			count++;
	}
	return count;
} 

int checkSquares(char board[N][N], int (*ptrCount)(char, char[N])) {

	// an array of squares
	char squares[N][N] = {
							{board[0][0], board[0][1], board[1][0], board[1][1]},
							{board[0][2], board[0][3], board[1][2], board[1][3]},
							{board[2][0], board[2][1], board[3][0], board[3][1]},
							{board[2][2], board[2][3], board[3][2], board[3][3]}	
						};

	for (int k = 0; k < N; k++) {
		for (char c = '1'; c < '5'; c++) {
			if ((*ptrCount)(c, squares[k]) != 0 && (*ptrCount)(c, squares[k]) != 1) {
				return 0;
			}			
		}
	}
	return 1; // 1 means no duplicates (Good board!)
	
}

int checkRows(char board[N][N], int (*ptrCount)(char, char[N])) {
	for (int k = 0; k < N; k++) {
		for (char c = '1'; c < '5'; c++) {
			if ((*ptrCount)(c, board[k]) != 0 && (*ptrCount)(c, board[k]) != 1) {
				return 0;
			}			
		}
	}
	return 1;
}

int checkCols(char board[N][N], int (*ptrCount)(char, char[N])) {
	char cols[N][N] = {
							{board[0][0],	 board[1][0],   board[2][0], 	board[3][0]},
							{board[0][1],	 board[1][1],  	board[2][1], 	board[3][1]},
							{board[0][2], 	 board[1][2], 	board[2][2], 	board[3][2]},
							{board[0][3],	 board[1][3], 	board[2][3], 	board[3][3]}
	};
	for (int k = 0; k < N; k++) {
		for (char c = '1'; c < '5'; c++) {
			if ((*ptrCount)(c, cols[k]) != 0 && (*ptrCount)(c, cols[k]) != 1) {
				return 0;
			}			
		}
	}
	return 1;
}

char *toString(char init_board[N][N]) {
	char *toStringBoard, k = 0;
	toStringBoard = (char *) malloc(N * N * sizeof(char));
	strcpy(toStringBoard, "");
	for (int i = 0; i < N; i++) {
		for (int  j = 0; j < N; j++) {
			toStringBoard[k++] = init_board[i][j];
		}
	}
	toStringBoard[k] = '\0';
	return toStringBoard;
}

int main() {
	srand(time(NULL));
	int choose = rand() % 2;

	if (choose == 0)
		populateBoardWith4(init_board);
	else
		populateBoardWith5(init_board);
	
	printf("Initial baord. No changes yet:\n");
	printBoard(init_board);
	
	
	srand(time(NULL));
	
	int counter, a, b, c, d;
	int row1, col1, row2, col2;
	TRY:counter = (rand() % 1000) + 1;
	do {
		int i = rand() % 6;
		switch (i) {
			case 0:
				rotateBoard90(init_board);
				break;
			case 1:
				rotateBoard180(init_board);
				break;
			case 2:
				rotateBoard180AntiClockwise(init_board, transpose, rotateBoard180);
				break;
			case 3:
				RND1:a = rand() % 4; 
				b = rand() % 4;
				if (a == b)
					goto RND1;
				reverseCols(init_board, a, b);
				break;
			case 4:
				RND2:c = rand() % 4;
				d = rand() % 4;
				
				if (c == d)
					goto RND2;
				reverseRows(init_board, c, d);
				break;
			case 5:
				RND3:row1 = rand() % 4;
				col1 = rand() % 4;
				row2 = rand() % 4;
				col2 = rand() % 4;
				if (row1 == row2 && col1 == col2)
					goto RND3;
				changeBoxesRandomly(init_board, row1, col1, row2, col2);
				break;
				
		}
		counter--;
	} while (counter > 0);
	//printf("%d %d %d\n", checkSquares(init_board, count) , checkRows(init_board, count) , checkCols(init_board, count));
	if (!(checkSquares(init_board, count) && checkRows(init_board, count) && checkCols(init_board, count)))
		goto TRY;
	
	printBoard(init_board);
	
	FILE* current_puzzle_file = fopen(CURRENT_PUZZLE_PATH, "w");
 	
 	if (!current_puzzle_file) {
    	printf("File not opened!\nQuitting...\n");
    	return 0;
	}
	

	
 	fputs(toString(init_board), current_puzzle_file);
	fclose(current_puzzle_file);
	
	return 0;

}
