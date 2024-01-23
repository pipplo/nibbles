// The high score data struct
struct HIGH_SCORES {
	int scores[10];
	char names[10][15];
};

void highscores_Add(int score);
void highscores_Load();
void highscores_Save();
void highscores_Show();

