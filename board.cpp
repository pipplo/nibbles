#include <stdio.h>
#include <allegro.h>
#include <winalleg.h>

#include "board.h"
#include "enums.h"

#define EAT_NEW 3

cBoard::cBoard() {
	loadLevels();
}

void cBoard::removeBoard(int x, int y) {
	board[x][y] = 0;
}

void cBoard::drawBoard() {
//    int i,j;
//    if ( apples )
//	for (i = 0; i < 34; i++)
//		for (j = 0; j < 30; j++) {
			//if (board[i][j] == 1)
			//	blit(wall, buffer, 0, 0, i * 16, j * 16, wall->w, wall->h);
//			if (board[i][j] == 2)
//				blit(apple, buffer, 0, 0, i * 16, j * 16, apple->w, apple->h);
//		}

	char score_Buffer[20];
    sprintf(score_Buffer, "Score: %d", score);
    textout(buffer, font, score_Buffer, 545, 10, 2);
    sprintf(score_Buffer, "Level: %d", level + 1);
    textout(buffer, font, score_Buffer, 545, 40, 2);
    sprintf(score_Buffer, "Eaten: %d", eaten);
    textout(buffer, font, score_Buffer, 545, 100, 2);

}

int cBoard::updateBoard() {
    int prob, randx, randy;

    if ( eaten == EAT_NEW ) {
        return NEWLEVEL;
    }
 	  
    
    prob = rand() % 10;
    randx = rand() % 32 + 1;
    randy = rand() % 28 + 1;

    if (prob != 4)
		return 0;

    if ( (board[randx][randy] == 0) && (apples < 1)) {
        board[randx][randy] = 2;
        apples = 1;
        blit(apple, buffer, 0, 0, randx * 16, randy * 16, apple->w, apple->h);
        
    }
    return 0;
}

void cBoard::resetBoard() {
	int i, j;
	level = 1;
	score = 0;
	apples = 0;
	eaten = 0;
	
    clear(buffer);
	for (i = 0; i < 34; i++)
		for (j = 0; j < 30; j++)
			board[i][j] = levels[level][i][j];

	for (i = 0; i < 34; i++)
		for (j = 0; j < 30; j++) {
			if (board[i][j] == 1)
				blit(wall, buffer, 0, 0, i * 16, j * 16, wall->w, wall->h);
		}
}

void cBoard::newLevel() {
	int i, j;
  	char msg_Buffer[25];

	level++;
	score+=100;
	eaten = 0;
	apples = 0;

	clear(buffer);
	sprintf(msg_Buffer, "New Level: Press Enter");
	textout(buffer, font, msg_Buffer, 310, 240, 4);
	blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

	clear(buffer);
    
	for (i = 0; i < 34; i++)
		for (j = 0; j < 30; j++)
			board[i][j] = levels[level][i][j];

	for (i = 0; i < 34; i++)
		for (j = 0; j < 30; j++) {
			if (board[i][j] == 1)
				blit(wall, buffer, 0, 0, i * 16, j * 16, wall->w, wall->h);
		}
}

void cBoard::loadLevels() {
    int x, y, j;
    int direction;
    int startx, starty, start;
	 
    for (j = 1; j < 6; j++) {
		for (x = 0; x < 34; x++)
			for (y = 0; y < 30; y++)
				levels[j][x][y] = 0;

		for (x = 0; x < 34; x++) {
			levels[j][x][0] = 1;
			levels[j][x][29] = 1;
		}
		for (y = 0; y < 30; y++) {
			levels[j][0][y] = 1;
			levels[j][33][y] = 1;
		}
	}	

	startx = 12;
	starty = 8;
	for (x = 8; x < 27; x++)
		if (x != 16 && x != 17) {
			levels[4][x][9] = 1;
			levels[4][x][20] = 1;
		}
	for (y = 9; y < 21; y++)
		if (y != 14 && y != 15) {
			levels[4][8][y] = 1;
			levels[4][26][y] = 1;
		}

	start = 0;
    direction = 1;
    for (x = 0; x < 30; x+= 5) {
		for (y = start; y != start + (8 * direction) ; y+= direction)
			levels[3][x][y] = 1;
		if (start == 29)
			start = 0;
		else
			start = 29;
			direction *= -1;
    }
            
	for (y = 10; y < 20; y++) 
		levels[2][17][y] = 1;
	for (x = 10; x < 24; x++)
		levels[2][x][14] = 1;

	for (x = 10; x < 24; x++)
		levels[1][x][14] = 1;
      
}

void cBoard::eventBoard(int event) {
	switch (event) {
		case BONUS:
			score += 10;
			apples = 0;
			eaten++;
			break;
		case DEATH:
			end_game = true;
			break;
	}

}

void cBoard::setBoard(int x, int y) {
	board[x][y] = 4;
}

int cBoard::returnBoard(int x, int y) {
	return board[x][y];
}
