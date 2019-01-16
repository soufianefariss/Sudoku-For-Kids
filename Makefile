SRC = sudoku.c
OUT = sudoku

clean:
	rm $(OUT)

sudoku:
	gcc $(SRC) -o $(OUT) `pkg-config gtk+-2.0 --cflags --libs`
