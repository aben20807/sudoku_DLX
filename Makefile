all: Sudoku.o solve_DLX.cpp
	g++ -o solve_DLX Sudoku.o solve_DLX.cpp
	
Sudoku.o:
	g++ -c Sudoku.cpp -o Sudoku.o
	
