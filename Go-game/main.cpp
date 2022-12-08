#define _CRT_SECURE_NO_WARNINGS
#include"conio2.h"
#include "ui.h"
#include "board.h"
#include<string.h>
#include <cstdlib>



int main() {
	// Variables
	int key = 0;

	// Init game
	struct go_data go;
	setupConsole();

	// Place the cursor in the center of the board and create board
	newGame(&go);
	centerCursor(&go);
	go.board = (char*)malloc(go.board_size * go.board_size * sizeof(char));



	do {
		displayLegend(go.board_x, go.board_y);
		displayBoard(go.board);
		displayCursor(go);

		key = getch();
		switch (key) {
		case 0:
			moveCursor(&go);
			break;
		case 'i':
			putStone(&go);
			break;
		case 'n':
			//newGame();

			centerCursor(&go);
			// Zero out a go.board array
			memset(go.board, 0, sizeof(go.board));

			break;
		default:
			break;
		}

	} while (key != 'q' && key != 'Q');

	_setcursortype(_NORMALCURSOR); // show the cursor, so it will be visible after the program ends

	return 0;
}
