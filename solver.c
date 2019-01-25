#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW 0
#define COL 1
#define GRID 2
#define TRUE 1
#define FALSE 0


char sudoku_correct[9][9] = {
							{8,3,5,4,1,6,9,2,7},
                            {2,9,6,8,5,7,4,3,1},
                            {4,1,7,2,9,3,6,5,8},
                            {5,6,9,1,3,4,7,8,2},
                            {1,2,3,6,7,8,5,4,9},
                            {7,4,8,5,2,9,1,6,3},
                            {6,5,2,7,8,1,3,9,4},
                            {9,8,1,3,4,5,2,7,6},
                            {3,7,4,9,6,2,8,1,5}
};

char sudoku_game[4][4] = {{1, 4, 2, 3},
						  {2, 3, 4, 1},
						  {4, 1, 3, 2},
						  {3, 2, 1, 4}
};

char sudoku_incorrect[9][9] = {
							{8,3,5,4,1,6,9,2,7},
							{2,9,6,8,5,7,4,3,1},
							{4,1,7,2,9,3,6,5,8},
							{5,6,9,1,3,4,7,8,2},
							{1,2,3,6,7,8,5,4,9},
							{7,4,8,5,2,9,1,6,3},
							{6,5,2,7,8,1,3,9,4},
							{9,8,1,3,4,5,2,7,6},
							{3,7,4,9,6,2,8,1,1}
};



short posRec[4][2];



int isValid(char a[][4]) {
   int i, j, val, gIndex;

   for(i=0; i < 4; i++){
      posRec[i][ROW] = 0;
      posRec[i][COL] = 0;
      posRec[i][GRID] = 0;
   }
	printf("\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("%i \t", posRec[i][j]);
		}
		printf("\n");
	}
	
   for(i=0; i<4; i++) {
      for(j=0; j<4; j++) {
         val = a[i][j]-1; //convert to 0-8 array index
         printf("--> 1 begin\n");
         if((posRec[val][ROW]>>i) & 0x1)
            return FALSE;
            
		printf("--> 1 passed\n\n");
		posRec[val][ROW] |= (0x1 << i);

		printf("--> 2 begin\n");
         if((posRec[val][COL] >> j) & 0x1)
            return FALSE;
         printf("--> 2 passed\n\n");
         posRec[val][COL] |= (0x1 << j);
		printf("--> 3 begin\n");
         gIndex = (j / 2) + ((i / 2) * 2); //(j/3) + ((i/3) * 3);
         if((posRec[val][GRID] >> gIndex) & 0x1)
            return FALSE;
         printf("--> 3 passed\n\n");
         posRec[val][GRID] |= (0x1 << gIndex);

		}
	}
	
	printf("\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("%i \t", posRec[i][j]);
		}
		printf("\n");
	}

   for(i=0; i<4; i++){

      if(posRec[i][COL] != 0x1ff ||
         posRec[i][ROW] != 0x1ff ||
         posRec[i][GRID] != 0x1ff)
         return FALSE;
   }
   printf("\n");
   for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++) {
			printf("%i \t", posRec[i][j]);
		}
		printf("\n");
	}

   return TRUE;

}

int main(){
	printf("correct sudoku check = %s \n", isValid(sudoku_game)?"CORRECT":"INCORRECT");
   //printf("incorrect sudoku check = %s \n", isValid(sudoku_incorrect)?"CORRECT":"INCORRECT");
   return 0;
}
