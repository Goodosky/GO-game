#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include"conio2.h"
#include "ui.h"

void setupConsole() {
	// Set the window title
	settitle("Oskar, Radziewicz, 193676");

	// hide the blinking cursor
	_setcursortype(_NOCURSOR);

	// Set console bg color
	textbackground(CONSOLE_BG_COLOR);
}


void displayLegend(int x, int y) {
	clrscr(); // Clean console
	textcolor(LEGEND_TXT_COLOR);

	int number_of_legend_elements = sizeof(legendElements) / sizeof(legendElements[0]);

	// Display the legend
	for (int i = 0; i < number_of_legend_elements; i++) {
		gotoxy(LEGEND_START_X, LEGEND_START_Y + i);
		cputs(legendElements[i]);
	}

	// Display current coordinates position
	gotoxy(LEGEND_START_X, LEGEND_START_Y + number_of_legend_elements + 2);
	cputs("Coordinates:");

	gotoxy(LEGEND_START_X, LEGEND_START_Y + number_of_legend_elements + 3);
	char txt[30];
	sprintf(txt, "x: %d, y: %d", x + 1, y + 1);
	cputs(txt);
};


