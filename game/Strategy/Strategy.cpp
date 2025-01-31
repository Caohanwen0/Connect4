#include "Strategy.h"


#include <iostream>

#include "MCTree.h"
#include "Point.h"

using namespace std;
int Phase::M = 12;
int Phase::N = 12;
int Phase::noX = 12;
int Phase::noY = 12;
typedef pair<int, int> pii;
MCTree* mcTree = nullptr;
// int negamax(const Phase &phase, int alpha, int 
, int player, int &move);
/*
		策略函数接口,该函数被对抗平台调用,每次传入当前状态,要求输出你的落子点,该落子点必须是一个符合游戏规则的落子点,不然对抗平台会直接认为你的程序有误

		input:
				为了防止对对抗平台维护的数据造成更改，所有传入的参数均为const属性
				M, N : 棋盘大小 M - 行数 N - 列数 均从0开始计，
   左上角为坐标原点，行用x标记，列用y标记 top : 当前棋盘每一列列顶的实际位置.
   e.g. 第i列为空,则_top[i] == M, 第i列已满,则_top[i] == 0 _board :
   棋盘的一维数组表示,
   为了方便使用，在该函数刚开始处，我们已经将其转化为了二维数组board
								你只需直接使用board即可，左上角为坐标原点，数组从[0][0]开始计(不是[1][1])
								board[x][y]表示第x行、第y列的点(从0开始计)
								board[x][y] == 0/1/2 分别对应(x,y)处
   无落子/有用户的子/有程序的子,不可落子点处的值也为0 lastX, lastY :
   对方上一次落子的位置, 你可能不需要该参数，也可能需要的不仅仅是对方一步的
								落子位置，这时你可以在自己的程序中记录对方连续多步的落子位置，这完全取决于你自己的策略
				noX, noY :
   棋盘上的不可落子点(注:top已经替你处理了不可落子点，也就是说如果某一步
								所落的子的上面恰是不可落子点，那么UI工程中的代码就已经将该列的top值又进行了一次减一操作，
								所以在你的代码中也可以根本不使用noX和noY这两个参数，完全认为top数组就是当前每列的顶部即可,
								当然如果你想使用lastX,lastY参数，有可能就要同时考虑noX和noY了)
				以上参数实际上包含了当前状态(M N _top _board)以及历史信息(lastX
   lastY),你要做的就是在这些信息下给出尽可能明智的落子点 output: 你的落子点Point
*/
#ifdef WINDOWS
extern "C" __declspec(dllexport) Point * getPoint(const int M, const int N, const int* top,
	const int* _board, const int lastX, const int lastY,
	const int noX, const int noY) {
#else
extern "C" Point * getPoint(const int M, const int N, const int* top, const int* _board,
	const int lastX, const int lastY, const int noX, const int noY) {
#endif
	/*
			不要更改这段代码
	*/
	int** board = new int* [M];
	for (int i = 0; i < M; i++) {
		board[i] = new int[N];
		for (int j = 0; j < N; j++) {
			board[i][j] = _board[i * N + j];
		}
	}
	Phase::set(M, N, noX, noY);
	Phase phase(board, top);
	if (!mcTree) mcTree = new MCTree();
	mcTree->setPhase(phase, lastY);
	int move = mcTree->search(TIME_LIMIT);
#ifdef OUTPUT
	// cout << "Opponent's move: " << lastY << endl;
	cout << "My move: " << move << endl;
	// phase.printBoard();
#endif
	clearArray(M, N, board);
	return new Point(top[move] - 1, move);
}

#ifdef WINDOWS
extern "C" __declspec(dllexport) void clearPoint(Point * p) {
#else
extern "C" void clearPoint(Point * p) {
#endif
	delete p;
	return;
}

void clearArray(int M, int N, int** board) {
	for (int i = 0; i < M; i++) {
		delete[] board[i];
	}
	delete[] board;
}
