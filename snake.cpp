#include <list>
#include <iterator>

#include <allegro.h>
#include <winalleg.h>

#include "board.h"
#include "snake.h"
#include "enums.h"

using namespace std;

// ****** Snake Class Member Functions ******

cSnake::cSnake() {
	length = 5;
	xHead =	16;
	yHead = 8;
	xDirection = -1;
	yDirection = 0;
	nextType = HBODY;
	lSnake.push_front( cBody(xHead, yHead, HEAD) );
}

void cSnake::drawSnake() {

	list <class cBody>::iterator p;
	p = lSnake.begin();
	(*p).drawBody();
	++p;
	(*p).drawBody();
    //for (p = lSnake.begin(); p != lSnake.end(); p++)
    //     (*p).drawBody();
		
}

void cSnake::updateSnake() {
	list <class cBody>::iterator p;

	// ****** Set Current Head to Next Type ******
	p = lSnake.begin();
	(*p).newType(nextType);

   	// ****** Default Next type ******   
    if (yDirection != 0)
        nextType = VBODY;
	else
		nextType = HBODY;
		
     // ****** Move the Snake and Put New Head ******
     xHead += xDirection;
     yHead += yDirection;
     lSnake.push_front( cBody(xHead, yHead, nextType) );
     

	 // ****** Find End and Remove it if Snake is more Than Length ******
	 p = lSnake.end();
     p--;

     if (lSnake.size() >= length) {
		(*p).removeBody();
        lSnake.pop_back();
     }

	 if (checkSnake() == BONUS) {
	    // play chomp sound
	    play_sample(chomp, 128, 128, 1000, 0);
		eaten++;
		apples--;
		score += 10;
		length+=5;
	 }
	 else if (checkSnake() == DEATH) {
		 end_game = true;
	 }

	 Board.setBoard(xHead, yHead);

}

void cSnake::moveSnake(int horiz, int vert) {
     switch (yDirection) {
            case -1:
                 if (horiz == -1)
                    nextType = UR;
                 if (horiz == 1)
                    nextType = UL;
                 break;
            case 1:
                 if (horiz == -1)
                    nextType = LR;
                 if (horiz == 1)
                    nextType = LL;
                 break;
     }
     switch (xDirection) {
            case -1:
                 if (vert == -1)
                    nextType = LL;
                 if (vert == 1)
                    nextType = UL;
                 break;
            case 1:
                 if (vert == -1)
                    nextType = LR;
                 if (vert == 1)
                    nextType = UR;
                 break;
     }

     xDirection = horiz;
     yDirection = vert;
}


int cSnake::checkSnake() {

	if (Board.returnBoard(xHead, yHead) == 2)
		return BONUS;

	if (Board.returnBoard(xHead, yHead) == 4 || Board.returnBoard(xHead, yHead) == 1)
		return DEATH;

	return LIVE;

}

void cSnake::resetSnake() {
	length = 5;
	xHead =	16;
	yHead = 8;
	xDirection = -1;
	yDirection = 0;
	nextType = HBODY;
	lSnake.clear();
	lSnake.push_front( cBody(xHead, yHead, HEAD) );
}
// ****** Body Class Member Functions ******
cBody::cBody() {
	Brow = 16;
	Bcol = 8;
	Bbody = HEAD;
}

cBody::cBody(int row, int col, int type) {
        Brow = row;
        Bcol = col;
        Bbody = type;
}

void cBody::drawBody() {
	masked_blit(snake[Bbody], buffer, 0, 0, Brow * 16, Bcol * 16, 16, 16);
}

void cBody::removeBody() {
	acquire_bitmap(buffer);
	acquire_screen();	
	rectfill(buffer, Brow * 16, Bcol * 16, Brow *16 + 16, Bcol * 16 + 16, 0);
	release_screen();
	release_bitmap(buffer);
	Board.removeBoard(Brow, Bcol);
}

void cBody::newType(int newtype) {
    Bbody = newtype;
}
