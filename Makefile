SRC = sudoku.c
GEN = generator.c
OUT = sudoku

clean:
	rm $(OUT)

sudoku:
	gcc -w $(SRC) -o $(OUT) `pkg-config --cflags --libs  gtk+-2.0` `mysql_config --libs`
