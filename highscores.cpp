#include <fstream.h>
#include <stdio.h>

#include <allegro.h>
#include <winalleg.h>

#include "highscores.h"

#define NUMBER_OF_SCORES 10

struct HIGH_SCORES highscores;

// ****** Loads High Scores ******
void highscores_Load() {
	ifstream highscoresFile;
    highscoresFile.open("scores.dat", ios::in | ios::binary);
	if (highscoresFile)
		highscoresFile.read( (char *) &highscores, sizeof(struct HIGH_SCORES));
	highscoresFile.close();
}

void highscores_Show() {
     char scoreStr[15];
     int i;

     clear(screen);

     for(i = 0; i < NUMBER_OF_SCORES; i++) {
           textout(screen, font, highscores.names[i], 50, 100 + (text_height(font)) * i, -1);
           itoa(highscores.scores[i], scoreStr, 10);
           textout(screen, font, scoreStr, 500, 100 + (text_height(font)) * i, -1);
     }
     key[KEY_ENTER] = FALSE;
     while(!key[KEY_ENTER]) {
     };

     key[KEY_ENTER] = FALSE;
}

void highscores_Save() {
	ofstream highscoresFile;
    highscoresFile.open("scores.dat", ios::out | ios::trunc | ios::binary);
	
	if(highscoresFile.good()) {
		highscoresFile.write( (char *) &highscores, sizeof(struct HIGH_SCORES));
		highscoresFile.close();
    }
}

void highscores_Add(int score) {
   int i, j, keyp;
   int score_rank = NUMBER_OF_SCORES;
   int index = 0;
   clear(screen);

   // ****** Find position in Scores ******
   for (i = NUMBER_OF_SCORES - 1; i >= 0; i--)
      if(highscores.scores[i] > score) {
      	score_rank = i + 1;
         break;
      }

   // ****** Check to see if Highest ******
   if (highscores.scores[0] < score)
   	score_rank = 0;

	// ****** Move them down if new high score ******
	if (score_rank < NUMBER_OF_SCORES) {

     	for (i = NUMBER_OF_SCORES - 2; i >= score_rank; i--) {
         highscores.scores[i + 1] = highscores.scores[i];
		 for (j = 0; j < 15; j++) {
         highscores.names[i + 1][j] = highscores.names[i][j];
		 }
      }

      highscores.scores[score_rank] = score;

      // ****** Clear the screen and get the new High Score ******
      clear(screen);

      textout_centre(screen, font, "You got a high score!", SCREEN_W / 2, 50, 2);
      textout_centre(screen, font, "Please enter your name:", SCREEN_W / 2, 200, 2);

      clear_keybuf();

      highscores.names[score_rank][0] = '_';
      highscores.names[score_rank][1] = '\0';

      text_mode(1);

      while(1) {
         rectfill(screen, SCREEN_W/2 - 54, 246, SCREEN_W/2 + 54, 254 + text_height(font), 4);
      	 rectfill(screen, SCREEN_W/2 - 50, 250, SCREEN_W/2 + 50, 250 + text_height(font), 1);
         textout_centre(screen, font, highscores.names[score_rank], SCREEN_W / 2, 251, 2);
         keyp = readkey();

         if(keyp >> 8 == KEY_BACKSPACE && index != 0) {
            highscores.names[score_rank][index--] = '\0';
            highscores.names[score_rank][index] = '_';
         }
         else if(keyp >> 8 == KEY_ENTER) {
            highscores.names[score_rank][index] = '\0';
            break;
         }
         else {
            highscores.names[score_rank][index] = keyp & 0xff;
            index++;

            if(index > 13)
            	index = 13;

            highscores.names[score_rank][index] = '_';
            highscores.names[score_rank][index + 1] = '\0';
         }
      }
   }
   // ****** Save The High Scores ******
   highscores_Save();
   text_mode(0);
   key[KEY_ENTER] = false;
}
