#include <iostream> 
#include <stdlib.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <time.h>  
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <utility>
#include <map>
#include <algorithm>

#define RESET   "\033[0m"
// #define BLACK   "\033[30m"      /* Black */
// #define RED     "\033[31m"      /* Red */
// #define GREEN   "\033[32m"      /* Green */
// #define YELLOW  "\033[33m"      /* Yellow */
// #define BLUE    "\033[34m"      /* Blue */
// #define MAGENTA "\033[35m"      /* Magenta */
// #define CYAN    "\033[36m"      /* Cyan */
// #define WHITE   "\033[37m"      /* White */
// #define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
// #define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
// #define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
// #define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
// #define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
// #define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
// #define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
// #define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define BOARDSIZE 10

enum class Color {RED, GREEN, YELLOW, MAGENTA, CYAN, BLUE, WHITE, BOLDRED, BOLDGREEN, BOLDYELLOW, BOLDBLUE, BOLDMAGENTA, BOLDCYAN, BOLDWHITE};
std::map <Color, int> colorMap;

void createBoard();
void myAlgorithm();
void fillColorMap();
void showRemoved(std::vector< std::pair<int,int> > removed);
std::vector<int> checkVertical(const int x, const int y);
std::vector<int> checkHorizontal(const int x, const int y);

std::vector<std::vector <int> > Board(BOARDSIZE, std::vector<int>(BOARDSIZE, -1));
std::vector<std::vector <bool> > BoardVisited(BOARDSIZE, std::vector<bool>(BOARDSIZE, false));
std::vector<std::vector <bool> > BoardVerticalVisited(BOARDSIZE, std::vector<bool>(BOARDSIZE, false));

int main()
{
	fillColorMap();
	srand (time(NULL));
	createBoard();
	myAlgorithm();
	return 0;
}

//mają tabele 0 1
//			  2 3
// pisząc tabela[0][1] to element 1!, najpierw y, potem x!
/*
1. foreach element in table which is not visited:
a) check in row (right/left):
	- mark gem as visited
	- check gems before this one (till other gem)
	- check gems next one (till other gem)
	- sum how many gems (right + left) are same
	- if sum is greater or equal 3 we have match -> save to remove
b) check in column similar as too row
*/
void myAlgorithm()
{
	std::vector<int> currentlyHorizontalMarked;
	std::vector<int> currentlyVerticalMarked;
	std::vector< std::pair<int,int> > toRemove; //indexes, which should be removed

	for (int i = 0; i < BOARDSIZE; i++)
		for (int j = 0; j < BOARDSIZE; j++)
		{
			if (!BoardVisited[i][j])
			{ 
				currentlyHorizontalMarked = checkHorizontal(i,j);
				if (currentlyHorizontalMarked.size() >= 3)
				{//we have horizontal match. We should check vertical for every horizontal matching gem and set to remove
					for (int index : currentlyHorizontalMarked)
						toRemove.push_back(std::make_pair(i, index));
				}
			}
			//check columns
			if (!BoardVerticalVisited[i][j])
			{
				currentlyVerticalMarked = checkVertical(i, j);
				if (currentlyVerticalMarked.size() >= 3)
				{
					for (int vInd : currentlyVerticalMarked)
						toRemove.push_back(std::make_pair(vInd, j));
				}
			}
		}
	showRemoved(toRemove);
}

//returns x indexes of matching gems. Checks columns
std::vector<int> checkVertical(const int x, const int y)
{
	int bottom = x+1, top = x;
	int counter = 0;
	std::vector<int> currentlyMarked;
	//we are looking before
	while (top >= 0 && top < BOARDSIZE && Board[top][y] == Board[x][y])
	{
		BoardVisited[top][y] = true;
		BoardVerticalVisited[top][y] = true;
		currentlyMarked.push_back(top);
		top--;
		counter++;
	}
	while (bottom >= 0 && bottom < BOARDSIZE && Board[bottom][y] == Board[x][y])
	{
		BoardVisited[bottom][y] = true;
		BoardVerticalVisited[bottom][y] = true;
		currentlyMarked.push_back(bottom);
		bottom++;
		counter++;
	}
	if (currentlyMarked.size() < 3)
		currentlyMarked.clear();

	return currentlyMarked;
}

std::vector<int> checkHorizontal(const int x, const int y)
{
	int left = y, right = y+1;
	int counter = 0;
	std::vector<int> currentlyMarked;
	//we are looking before
	while (left >= 0 && left < BOARDSIZE && Board[x][left] == Board[x][y])
	{
		BoardVisited[x][left] = true;
		currentlyMarked.push_back(left);	
		left--;
		counter++;
	}
	//we are looking after
	while (right >= 0 && right < BOARDSIZE && Board[x][right] == Board[x][y])
	{
		BoardVisited[x][right] = true;
		currentlyMarked.push_back(right);		
		right++;
		counter++;
	}

	if (currentlyMarked.size() < 3)
		currentlyMarked.clear();
	return currentlyMarked;
	
}

void fillColorMap()
{
	colorMap.insert(std::pair<Color, int>(Color::RED,31));
	colorMap.insert(std::pair<Color, int>(Color::GREEN,32));		
	colorMap.insert(std::pair<Color, int>(Color::YELLOW,33));		
	colorMap.insert(std::pair<Color, int>(Color::MAGENTA,35));		
	colorMap.insert(std::pair<Color, int>(Color::CYAN,36));		
	colorMap.insert(std::pair<Color, int>(Color::BLUE,34));					
}

void createBoard()
{
	for (int x = 0; x < BOARDSIZE; x++)
	{
		for (int y = 0; y < BOARDSIZE; y++)
			Board[x][y] = rand() % 5 + 1;
	}
	std::cout<<"Board created\n";
}

void showRemoved(std::vector< std::pair<int,int> > removed)
{
	std::vector<std::vector <int> > removedBoard(BOARDSIZE, std::vector<int>(BOARDSIZE, false));
	std::cout << "removed elements: "<<std::endl;
	for (std::pair<int, int> indPair: removed)
	{
		removedBoard[indPair.first][indPair.second] = true;
	}

	for (int x = 0; x < BOARDSIZE; x++)
	{
		for (int y = 0; y < BOARDSIZE; y++)
		{
			std::cout << "\033[" << Board[x][y]+30 << "m";
			if (removedBoard[x][y])
			{
				std::cout << "\033[1m\033[37m";
			}
			std::cout << std::to_string(Board[x][y]) << " ";
			std::cout << RESET ;
		}
		std::cout << "\n";
	}
}
