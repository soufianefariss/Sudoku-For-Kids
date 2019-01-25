#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define	CURRENT_PUZZLE_PATH				"data/puzzles/current_puzzle.txt"
#define	FOUR_BOX_PUZZLES_FILE_PATH		"data/puzzles/4boxes.txt"


/*static void writeToCurrentPuzzle() {*/
/*	FILE *cuurent_sudoku_file = NULL, *four_boxes_file = NULL;*/
/*	four_boxes_file = fopen(CURRENT_PUZZLE_PATH, "r");*/
/*	*/
/*	if(!cuurent_sudoku_file)*/
/*		return;*/
/*	*/
/*	srand(time(NULL));*/
/*	int random_selected_puzzle = rand() % 256 + 1;*/
/*	*/
/*}*/

int main()
{
    FILE* current_puzzle_file = fopen(CURRENT_PUZZLE_PATH, "w");
 	
 	if (!current_puzzle_file) {
    	printf("File not opened!\nQuitting...\n");
    	return;
	}
	
 	fputs(init_puzzle, current_puzzle_file);
	fclose(current_puzzle_file);
}
