#include <stdio.h>
#include <time.h>

#define	CURRENT_PUZZLE_PATH				"data/puzzles/current_sudoku.txt"
#define	FOUR_BOX_PUZZLES_FILE_PATH	"data/puzzles/4boxes.txt"


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
    FILE* file = fopen(FOUR_BOX_PUZZLES_FILE_PATH, "r");
    
    if (!file) {
    	printf("File not opened!\n Quitting...\n");
    	return 0;
	}
    
    char line[256];
    int i = 0;
/*    srand(time(NULL));*/
/*    int line = rand() % 256 + 1;*/
    while (fgets(line, sizeof(line), file)) {
        i++;
        if(i == 1) {
            printf("i %s\n", line);   
        }
    }
 
    fclose(file);
 
    return 0;
}
