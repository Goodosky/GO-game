#pragma once

#ifndef UI_H
#define UI_H

// Colors (check conio2.h for available colors)
#define CONSOLE_BG_COLOR CYAN
#define LEGEND_TXT_COLOR BLACK

#define LEGEND_START_X 2
#define LEGEND_START_Y 1

#define MAX_LEGEND_ELEMENT_LENGTH 100

const char legendElements[][MAX_LEGEND_ELEMENT_LENGTH] = {
	"Oskar Radziewicz 193676",
	"",
	"Features:",
	"arrows = moving",
	"i	= put stone",
	"q	= exit",
};

void setupConsole(); // Customize console (bg, title etc..)
void displayLegend(int x, int y); // Display the legend

#endif