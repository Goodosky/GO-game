#include"conio2.h"
#include "board.h"
#include<string.h>
#include <stdlib.h>

void newGame(struct go_data* go) {
	if (go->is_new_board_size) {
		// Free memory of old array
		//for (int i = 0; i < go->board_size ; i++)
		//	free(go->board[i]);


		// Allocate memory for board array
		go->board = (int**)malloc(go->board_size * sizeof(int*));

		for (int i = 0; i < go->board_size; i++) {
			go->board[i] = (int*)malloc(go->board_size * sizeof(int));
			memset(go->board[i], NO_STONES, go->board_size * sizeof(int));
		}

		go->is_new_board_size = false;
	}


	// Zero out a go->board array
	for (int i = 0; i < go->board_size; i++) {
		memset(go->board[i], NO_STONES, go->board_size * sizeof(int));
	}

	centerCursor(go);
}


void displayBoard(struct go_data go) {
	drawBorder(go.board_size);

	for (int x = 0; x < go.board_size; x++) {
		for (int y = 0; y < go.board_size; y++) {
			gotoxy(BOARD_OFFSET_X + x, BOARD_OFFSET_Y + y);

			// Draw empty field...
			if (go.board[x][y] == NO_STONES) {
				textcolor(EMPTY_FIELD_COLOR);
				putch(EMPTY_FIELD);
				continue;
			}

			// ...or stone
			textcolor(go.board[x][y] == P1 ? BLACK : WHITE);
			putch(STONE);
		}
	}
}

void drawBorder(int board_size) {
	textcolor(BORDER_COLOR);

	gotoxy(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y - 1);
	putch(BORDER_TOP_LEFT_CORNER);

	for (int i = 0; i < board_size; i++) {
		putch(BORDER_TOP);
	}

	putch(BORDER_TOP_RIGHT_CORNER);

	gotoxy(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + board_size);
	putch(BORDER_BOTTOM_LEFT_CORNER);

	for (int i = 0; i < board_size; i++) {
		putch(BORDER_BOTTOM);
	}
	putch(BORDER_BOTTOM_RIGHT_CORNER);

	for (int i = 0; i < board_size; i++) {
		gotoxy(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + i);
		putch(BORDER_LEFT);

		gotoxy(BOARD_OFFSET_X + board_size, BOARD_OFFSET_Y + i);
		putch(BORDER_RIGHT);
	}
}



void displayCursor(struct go_data go) {
	textcolor(go.curr_player == P1 ? BLACK : WHITE);
	gotoxy(go.cursor_x(), go.cursor_y());
	putch('*');
}

void moveCursor(struct go_data* go) {
	int key = getch(); // get code of a special key

	switch (key) {
	case ARROW_UP:
		isInBoard(*go, 0, -1) && go->board_y--;
		break;
	case ARROW_LEFT:
		isInBoard(*go, -1, 0) && go->board_x--;
		break;
	case ARROW_RIGHT:
		isInBoard(*go, 1, 0) && go->board_x++;
		break;
	case ARROW_DOWN:
		isInBoard(*go, 0, 1) && go->board_y++;
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
	if (!isLegalMove(*go)) return;

	// Save info about stone in the go->board array
	go->board[go->board_x][go->board_y] = go->curr_player;

	// Change current player
	go->curr_player = go->curr_player == P2 ? P1 : P2;
};


int coutLiberties(struct go_data go) {
	int liberties = 0;

	hasLiberty(go, 0, -1) && liberties++; // top
	hasLiberty(go, 1, 0) && liberties++; // right
	hasLiberty(go, 0, 1) && liberties++; // down
	hasLiberty(go, -1, 0) && liberties++; // left

	gotoxy(2, 15);
	cputs("hasLiberty:");
	gotoxy(2, 16);
	putch(hasLiberty(go, 0, -1) ? 't' : 'f');
	putch(hasLiberty(go, 1, 0) ? 't' : 'f');
	putch(hasLiberty(go, 0, 1) ? 't' : 'f');
	putch(hasLiberty(go, -1, 0) ? 't' : 'f');
	getch();

	return liberties;
}


bool hasLiberty(struct go_data go, int x_shift, int y_shift) {
	int enemy = go.curr_player == 1 ? P2 : P1;
	int x = go.board_x + x_shift;
	int y = go.board_y + y_shift;

	return isInBoard(go, x_shift, y_shift) && go.board[x][y] != enemy;
}

bool isInBoard(struct go_data go, int x_shift, int y_shift) {
	const bool top = go.board_y + y_shift >= 0;
	const bool bottom = go.board_y + y_shift < go.board_size;
	const bool left = go.board_x + x_shift >= 0;
	const bool right = go.board_x + x_shift < go.board_size;


	return top && left && right && bottom;
}

bool isLegalMove(struct go_data go) {
	const bool field_not_occupied = go.board[go.board_x][go.board_y] == 0;
	const bool not_suicide = coutLiberties(go) > 0;


	return field_not_occupied && not_suicide;
}