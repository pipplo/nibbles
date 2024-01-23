extern BITMAP *buffer;
extern BITMAP *wall, *apple;

extern int end_game;

extern int score, eaten, apples;

class cBoard {
	public:
		cBoard();
		void resetBoard();
		void removeBoard(int x, int y);
		void drawBoard();
		int updateBoard();
		void newLevel();
		void loadLevels();
		void eventBoard(int event);
		void setBoard(int x, int y);
		int returnBoard(int x, int y);
		int returnScore();

	private:
		int levels[5][34][30];
		int board[34][30];
		int level;
};
