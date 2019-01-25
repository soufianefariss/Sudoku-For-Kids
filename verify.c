#include <stdio.h>
#include <stdlib.h>


int bad_board[4][4] = {
	{1, 2, 3, 4},
	{4, 3, 2, 1},
	{1, 1, 1, 1},
	{1, 1, 2, 4}
};

int good_board[4][4] = {
	{3, 2, 1, 4},
	{4, 1, 2, 3},
	{1, 4, 3, 2},
	{2, 3, 4, 1}
};



void print(int A[][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d\t", A[i][j]);		
		}
		printf("\n");
	}
}

int hasDuplicates(int array[4]) {
	for (int i = 0; i < 4 - 1; i++) {
		for (int j = i + 1; j < 4; j++) {
			if (array[i] == array[j]) {
		        return 1; // 1 means a duplicate is found, quitting..
		    }
		}
	}
	return 0; // 0 means no duplicates
}

void transpose(int A[][4], int B[][4]) { 
    int i, j; 
    for (i = 0; i < 4; i++) 
        for (j = 0; j < 4; j++) 
            B[i][j] = A[j][i]; 
} 

int main() {
	printf("\n\nChecking rows...\n");
	print(good_board);
	// check rows...
	for (int row = 0; row < 4; row++) {
		if (hasDuplicates(good_board[row])) {
			printf("Bad Board\n");
			return 0;
		}
	}
	// transpose the board so we switch rows with cols
	
	printf("\n\nChecking columns...\n");
	int transposed[4][4];
	transpose(good_board, transposed);
	print(transposed);
	// now, check columns... 
	for (int col = 0; col < 4; col++) {
		if (hasDuplicates(transposed[col])){
			printf("Bad Board\n");
			return 0;
		}
	}
	printf("Good board!\n");
	return 0;
}
