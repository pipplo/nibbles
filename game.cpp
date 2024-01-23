#include <iostream.h>
#include <stdio.h>
#include <time.h>
#include <list.h>

#include <allegro.h>
#include <winalleg.h>

#include "data.h"

#include "highscores.h"
#include "board.h"
#include "snake.h"
#include "enums.h"
#include "main.h"

using namespace std;

DATAFILE *data;

MIDI *music;
SAMPLE *chomp;

BITMAP *buffer;
BITMAP *snake[8]; 
BITMAP *wall, *apple;

int end_game = false, quit = false;
volatile int actual_fs, timing_fs = 1;
int variable_fs;

int score, eaten, apples;

class cSnake Snake;
class cBoard Board;

int main() {
	setup();

	do {
		menu();
	} while (!quit);

	allegro_exit();
	return 0;

}
END_OF_MAIN();

void menu() {
	int pos = 0;
	key[KEY_ENTER] = false;
	
	blit( (BITMAP *) data[TITLE_BMP].dat, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	blit( (BITMAP *) data[CURSOR_BMP].dat, screen, 0, 0, 405, 180 + (pos * 55), 20, 60);

	do {
		if (keypressed()) {
			if (key[KEY_DOWN]) {
				pos ++;
				key[KEY_DOWN] = false;
			}
			if (key[KEY_UP]) {
				pos--;
				key[KEY_UP] = false;
			}

			if (pos < 0)
				pos = 0;
			if (pos > 2)
				pos = 2;
			blit( (BITMAP *) data[TITLE_BMP].dat, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
			blit( (BITMAP *) data[CURSOR_BMP].dat, screen, 0, 0, 405, 180 + (pos * 55), 20, 60);
			readkey();
		}

	} while (!key[KEY_ENTER]);

	if (pos == 0)
		play();
	if (pos == 1)
		highscores_Show();
	if (pos == 2)
		quit = true;

		return;
}

// ****** Timer Function ****** //
void timing() {
     actual_fs++;
     timing_fs++;
}
END_OF_FUNCTION(timing);

void play() {

    install_int_ex(timing, BPS_TO_TIMER(15));
	variable_fs = 0;
	timing_fs = 0;
	play_midi(music, true);
	
	Board.resetBoard();
	Snake.resetSnake();
	
	end_game = false;

    do {
		input();
		process();
        output();
        do {
        } while ( actual_fs < 1 );
        actual_fs = 0;

    } while (!end_game);

	stop_midi();
	remove_int(timing);
    highscores_Add(score);
}

void input() {
     if (key[KEY_LEFT])
        Snake.moveSnake(-1, 0);
     else if (key[KEY_RIGHT])
        Snake.moveSnake(1, 0);
     else if (key[KEY_DOWN])
        Snake.moveSnake(0, 1);
     else if (key[KEY_UP])
        Snake.moveSnake(0, -1);
     else if (key[KEY_ESC])
        end_game = TRUE;
}

void process() {
	Snake.updateSnake();
    if ( Board.updateBoard() == NEWLEVEL ) {
        Snake.resetSnake();
        Board.newLevel();
       	timing_fs = 0;
       	variable_fs = 0;
        while ( !key[KEY_ENTER] ) {}
    }
}

void output() {
    char score_Buffer[100];
    float frames = timing_fs + 1;
    float fps = 10;	

    variable_fs++;
    fps = variable_fs / (frames / 10);
	
	acquire_bitmap(buffer);
	acquire_screen();
    Snake.drawSnake();
    Board.drawBoard();
    sprintf(score_Buffer, "FPS: %f", fps);
    textout(buffer, font, score_Buffer, 545, 70, 2);
	blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
	release_screen();
	release_bitmap(buffer);

}

// ******    Setup Function      ****** //
// ****** Initializes Everything ****** //

bool setup() {
     // ****** Setup Random Seed ****** //
     srand(time(NULL));

     allegro_init();

     // ****** Keyboard Functions ****** //
     if( install_keyboard() < 0 ) {
         cout << "\nFatal error installing keyboard handler. There is nothing I can do, sorry :(";
         return false;
     }

     // ****** Timer Functions ****** //
     if( install_timer() < 0 ) {
         cout << "\nFatal error installing timer handler. There is nothing I can do, sorry :(";
         return false;
     }
     LOCK_VARIABLE(actual_fs);
     LOCK_VARIABLE(timing_fs);
     LOCK_FUNCTION(timing);

     // ****** Install Sound ****** //
     if ( install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0) {
        cout << "Error Loading sound on this system."
                "\nI will proceed with no Sound";
        return false;
     }

     // ****** Install Graphics Drivers ****** //
     if ( set_gfx_mode(GFX_DIRECTX, 800, 600, 0, 0) < 0) {
        cout << "Error Loading Graphics into 640 x 480 mode."
                "\nCannot continue";
        return false;
     }
     //set_color_depth(24);
     // ****** Load the data file and set variables from it ****** //
     data = load_datafile("data.dat");

     // ****** System Variabes ****** //
     set_palette((RGB *) data[GAME_PAL].dat);

     // ****** Sprites ****** //
     snake[0] = (BITMAP *) data[HEAD_BMP].dat;
     // Tail..?
     //snake[2] = (BITMAP *) data[HBODY_BMP].dat;
     snake[HBODY] = (BITMAP *) data[HBODY_BMP].dat;
     snake[VBODY] = (BITMAP *) data[VBODY_BMP].dat;
     snake[4] = (BITMAP *) data[UL_BMP].dat;
     snake[5] = (BITMAP *) data[UR_BMP].dat;
     snake[6] = (BITMAP *) data[LL_BMP].dat;
     snake[7] = (BITMAP *) data[LR_BMP].dat;

     wall = (BITMAP *) data[WALL_BMP].dat;
     apple = (BITMAP *) data[BONUS_BMP].dat;

     // ****** Create the bitmap for our Double Buffer ****** //
     buffer = create_bitmap(SCREEN_W, SCREEN_H);
     clear(buffer);

     chomp = load_wav("eat.wav");
     music = load_midi("cindy.mid");

     highscores_Load();

     return true;
}


