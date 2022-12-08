#include"conio2.h"
#include "board.h"

void newGame(struct go_data go) {

}


void displayBoard(struct go_data go) {
	drawBorder(int border_size);

	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			gotoxy(BOARD_OFFSET_X + x, BOARD_OFFSET_Y + y);

			// Draw empty field...
			if (board[x][y] == 0) {
				textcolor(EMPTY_FIELD_COLOR);
				putch(EMPTY_FIELD);
				continue;
			}

			// ...or stone
			textcolor(board[x][y] == P1 ? BLACK : WHITE);
			putch(STONE);

		}

		// Return to cursor color
		textcolor(CURSOR_COLOR);
	}
}

void drawBorder(int border_size) {
	textcolor(BORDER_COLOR);

	gotoxy(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y - 1);
	putch(BORDER_TOP_LEFT_CORNER);

	for (int i = 0; i < BOARD_SIZE; i++) {
		putch(BORDER_TOP);
	}

	putch(BORDER_TOP_RIGHT_CORNER);

	gotoxy(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + BOARD_SIZE);
	putch(BORDER_BOTTOM_LEFT_CORNER);

	for (int i = 0; i < BOARD_SIZE; i++) {
		putch(BORDER_BOTTOM);
	}
	putch(BORDER_BOTTOM_RIGHT_CORNER);

	for (int i = 0; i < BOARD_SIZE; i++) {
		gotoxy(BOARD_OFFSET_X - 1, BOARD_OFFSET_Y + i);
		putch(BORDER_LEFT);

		gotoxy(BOARD_OFFSET_X + BOARD_SIZE, BOARD_OFFSET_Y + i);
		putch(BORDER_RIGHT);
	}

	textcolor(CURSOR_COLOR);
}



void displayCursor(struct go_data go) {
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
	go->board_x = go->board_y = BOARD_SIZE / 2;
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


bool isInBoard(struct go_data go, int x_shift, int y_shift) {
	bool top = go.board_y + y_shift >= 0;
	bool bottom = go.board_y + y_shift < BOARD_SIZE;
	bool left = go.board_x + x_shift >= 0;
	bool right = go.board_x + x_shift < BOARD_SIZE;

	return top && left && right && bottom;
}

bool isLegalMove(struct go_data go) {
	return true;
}