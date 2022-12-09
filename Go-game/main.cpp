#define _CRT_SECURE_NO_WARNINGS
#include"conio2.h"
#include "ui.h"
#include "board.h"

int main() {
	// Variables
	int key = 0;

	// Init game
	struct go_data go;
	setupConsole();

	// Create board and place the cursor in the center of the board
	newGame(&go);

	do {
		displayLegend(go.board_x, go.board_y);
		displayBoard(go);
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
			newGame(&go);
			break;
		default:
			break;
		}

	} while (key != 'q' && key != 'Q');

	_setcursortype(_NORMALCURSOR); // show the cursor, so it will be visible after the program ends

	return 0;
}