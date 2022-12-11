#include"conio2.h"
#include "board.h"
#include<string.h>
#include <stdlib.h>
#include<stdio.h>

void newGame(struct go_data* go) {
	if (go->is_new_board_size) {
		// Free previous dynamically allocated memory
		//for (int i = 0; i < go->board_size ; i++)
		//	free(go->board[i]);
		//free(go->board);

		// Allocate memory for board (it will be array of pointers) 
		go->board = (int**)malloc(go->board_size * sizeof(int*));

		// Allocate memory for the "second dimension" of the array
		// (pointers from the first dimension point to arrays with values)
		// and fiil up by a EMPTY_FIELD representation
		for (int i = 0; i < go->board_size; i++) {
			go->board[i] = (int*)malloc(go->board_size * sizeof(int));
			memset(go->board[i], EMPTY_FIELD, go->board_size * sizeof(int));
		}

		go->is_new_board_size = false;
	}

	// Zero out a go->board array
	for (int i = 0; i < go->board_size; i++) {
		memset(go->board[i], EMPTY_FIELD, go->board_size * sizeof(int));
	}

	centerCursor(go);
}


void displayBoard(struct go_data* go) {
	// Update console info width and height (and some other infos)
	gettextinfo(&go->consoleInfo);
	go->y_shift = go->x_shift = 0;

	// Set the shift in X
	int available_fields_x = go->consoleWidth() - BOARD_OFFSET_X - 1; // -1 = space for the border
	if (available_fields_x < go->board_size && go->board_x > available_fields_x) {
		go->x_shift = go->board_x - available_fields_x;
	}

	// Set the shift in Y
	int available_fields_y = go->consoleHeight() - BOARD_OFFSET_Y - 1; // -1 = space for the border
	if (available_fields_y < go->board_size && go->board_y > available_fields_y) {
		go->y_shift = go->board_y - available_fields_y;
	}

	// Draw board border
	int border_boottom_y = go->board_size > available_fields_y ? available_fields_y + 1 : go->board_size;  // +1 = space for the border
	if (go->x_shift == 0) {
		drawBorder(go, "top-left-corner", -1, -1);
		drawBorder(go, "bottom-left-corner", -1, border_boottom_y);
	}

	// Display columns and rows 
	for (int x = 0; x < go->board_size; x++) {
		// Skip if the row won't fit in the console
		if (x > available_fields_x) return;

		// Draw border top
		drawBorder(go, "top", x, -1);

		// Draw border bottom
		drawBorder(go, "bottom", x, border_boottom_y);

		// Draw column
		for (int y = 0; y < go->board_size; y++) {
			// Skip if the column won't fit in the console
			if (y > available_fields_y) continue;

			// If first column: draw left border
			if (x == 0 && go->x_shift == 0) drawBorder(go, "left", -1, y);

			// If last column: draw right border
			if (x + go->x_shift == go->board_size - 1) {
				if (y == 0) drawBorder(go, "top-right-corner", x + 1, -1);
				drawBorder(go, "right", x + 1, y);
				if (y + 1 == border_boottom_y) drawBorder(go, "bottom-right-corner", x + 1, y + 1);
			}

			// Draw stone or empty field
			drawField(go, x, y);
		}
	}
}

void drawField(struct go_data* go, int x, int y) {
	gotoxy(x + BOARD_OFFSET_X, y + BOARD_OFFSET_Y);

	if (go->board[x + go->x_shift][y + go->y_shift] == EMPTY_FIELD) {
		// Draw empty field...
		textcolor(EMPTY_FIELD_COLOR);
		putch(EMPTY_FIELD_ASCII);
	}
	else {
		// ...or stone
		textcolor(go->board[x + go->x_shift][y + go->y_shift] == P1 ? BLACK : WHITE);
		putch(STONE_ASCII);
	}
}

void drawBorder(struct go_data* go, const char direction[], int x, int y) {
	textcolor(BORDER_COLOR);
	gotoxy(x + BOARD_OFFSET_X, y + BOARD_OFFSET_Y);

	if (direction == "top-left-corner") putch(BORDER_TOP_LEFT_CORNER);
	else if (direction == "top-right-corner") putch(BORDER_TOP_RIGHT_CORNER);
	else if (direction == "bottom-right-corner") putch(BORDER_BOTTOM_RIGHT_CORNER);
	else if (direction == "bottom-left-corner") putch(BORDER_BOTTOM_LEFT_CORNER);
	else if (direction == "top") putch(BORDER_TOP);
	else if (direction == "right") putch(BORDER_RIGHT);
	else if (direction == "bottom") putch(BORDER_BOTTOM);
	else if (direction == "left") putch(BORDER_LEFT);
}

void setNewBoardSize(struct go_data* go) {
	go->board_size = 22;
	go->is_new_board_size = true;
}


void displayCursor(struct go_data* go) {
	textcolor(go->curr_player == P1 ? BLACK : WHITE);
	gotoxy(go->cursor_x() - go->x_shift, go->cursor_y() - go->y_shift);
	putch('*');
}

void moveCursor(struct go_data* go) {
	int key = getch(); // get code of a special key

	switch (key) {
	case ARROW_UP:
		isInBoard(go, 0, -1) && go->board_y--;
		break;
	case ARROW_LEFT:
		isInBoard(go, -1, 0) && go->board_x--;
		break;
	case ARROW_RIGHT:
		isInBoard(go, 1, 0) && go->board_x++;
		break;
	case ARROW_DOWN:
		isInBoard(go, 0, 1) && go->board_y++;
		break;
	default:
		break;
	}
}

void centerCursor(struct go_data* go) {
	go->board_x = go->board_y = go->board_size / 2;
	gotoxy(go->cursor_x(), go->cursor_y());
}


void putStone(struct go_data* go) {
	// Check if a move is legal
	if (!isLegalMove(go)) return;

	// Save info about stone in the go->board array
	go->board[go->board_x][go->board_y] = go->curr_player;

	// kills stones surrounded by this move (so without liberties)
	if (coutLiberties(go, 0, -1) == 0) killStone(go, 0, -1); // top
	if (coutLiberties(go, 1, 0) == 0) killStone(go, 1, 0); // right
	if (coutLiberties(go, 0, 1) == 0) killStone(go, 0, 1); // down
	if (coutLiberties(go, -1, 0) == 0) killStone(go, -1, 0); // left

	// Change current player
	go->curr_player = go->curr_player == P1 ? P2 : P1;
};

void killStone(struct go_data* go, int x_shift, int y_shift) {
	int* stone_to_kill = &(go->board[go->board_x + x_shift][go->board_y + y_shift]);

	// Check if this stone is a enemy
	if (*stone_to_kill != go->enemy()) return;

	// Kill stone
	go->board[go->board_x + x_shift][go->board_y + y_shift] = EMPTY_FIELD;

	// Add points for a killer
	go->points[go->curr_player == P1 ? 0 : 1] += 1;
};


int coutLiberties(struct go_data* go, int x_shift = 0, int y_shift = 0) {
	int liberties = 0;
	int enemy = getEnemyByXY(go, go->board_x + x_shift, go->board_y + y_shift);

	hasLiberty(go, 0 + x_shift, -1 + y_shift, enemy) && liberties++; // top
	hasLiberty(go, 1 + x_shift, 0 + y_shift, enemy) && liberties++; // right
	hasLiberty(go, 0 + x_shift, 1 + y_shift, enemy) && liberties++; // down
	hasLiberty(go, -1 + x_shift, 0 + y_shift, enemy) && liberties++; // left

	return liberties;
}

int getEnemyByXY(struct go_data* go, int x, int y) {
	// field (x,y) have to be in the board
	if (!isInBoard(go, x - go->board_x, y - go->board_y)) return 0;

	// Return enemy
	return go->board[x][y] == P1 ? P2 : P1;
};

bool hasLiberty(struct go_data* go, int x_shift, int y_shift, int enemy = false) {
	if (!enemy) int enemy = go->enemy();
	int x = go->board_x + x_shift;
	int y = go->board_y + y_shift;

	return isInBoard(go, x_shift, y_shift) && go->board[x][y] != enemy;
}

bool isInBoard(struct go_data* go, int x_shift, int y_shift) {
	const bool top = go->board_y + y_shift >= 0;
	const bool bottom = go->board_y + y_shift < go->board_size;
	const bool left = go->board_x + x_shift >= 0;
	const bool right = go->board_x + x_shift < go->board_size;

	return top && left && right && bottom;
}

bool isLegalMove(struct go_data* go) {
	const bool field_not_occupied = go->board[go->board_x][go->board_y] == 0;
	const bool not_suicide = coutLiberties(go) > 0;

	return field_not_occupied && not_suicide;
}