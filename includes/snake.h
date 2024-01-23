extern BITMAP *buffer;
extern BITMAP *snake[8];

extern SAMPLE *chomp;

extern class cBoard Board;
extern class cSnake Snake;
extern int end_game;
extern int score, apples, eaten;

class cBody {
	public:
		cBody();
        cBody(int row, int col, int type);
        void drawBody();
		void removeBody();
        void newType(int newtype);
	private:
		int Brow, Bcol;
        int Bbody;
};

class cSnake {
	public:
		cSnake();
		void drawSnake();
		void updateSnake();
		void moveSnake(int horiz, int vert);
		int checkSnake();
		void resetSnake();
	private:
		list <class cBody> lSnake;
		int length;
		int xHead;
		int yHead;
		int xDirection;
		int yDirection;
		int nextType;
};

