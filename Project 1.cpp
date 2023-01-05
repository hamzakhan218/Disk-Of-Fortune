/*
 * Name: YOUR NAME
 * Project 1: Disk of Fortune
 * Course: CSI218 (Fall 2020)
 * Date: THE DATE
 * Description: Play game in which 3 players try to solve a puzzle
 *				by guessing letters in a phrase.  Virtual money is
 *				allotted for correct letter guesses.
 */

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include "obscuredentries.cpp"
using namespace std;

// Definitions for functions that help game proceed.
// Show any punctuation (non-letters) in puzzle.
// Returns copy of puzzle with punctuation revealed.
ObscuredEntries<char> showPunct(ObscuredEntries<char> puzzle);

// Show a letter (passed) that user guessed in puzzle.
// Return a count of how many of such letter found.
int findLetter(ObscuredEntries<char>& puzzle, char letter);

// Return whether puzzle is solved (all letters revealed).
bool solved(const ObscuredEntries<char>& puzzle);

int main()
{
	// Values of each item on "disk" (see writeup) that is spun.
	const int NUM_ITEMS = 24;
	const string ITEMS[NUM_ITEMS] = {
		"450",
		"700",
		"300",
		"600",
		"1000",
		"200",
		"600",
		"BANKRUPT",
		"500",
		"200",
		"LOSE TURN",
		"400",
		"200",
		"900",
		"250",
		"300",
		"900",
		"BANKRUPT",
		"500",
		"400",
		"250",
		"LOSE TURN",
		"800",
		"150"
	};

	// 2D vector that holds phrase to solve as puzzle.
	vector<vector<char>> phrase;

	// Allow user to enter words in phrase.
	cout << "Enter phrase to solve (each word 1 line, blank line when done):" << endl;
	string word;

	getline(cin, word);  // read first word

	while (word != "")  // done entering?
	{
		// Add word as vector of characters (use anonymous object
		// with constructor that extracts characters from string
		// using iterators).
		phrase.push_back(vector<char>(word.begin(), word.end()));

		// Read another word.
		getline(cin, word);
	}

	// Seed random number generator (for determining player spin).
	// COMMENT OUT IF WISH TO TEST WITH SAME SPINS EACH TIME.
	srand((unsigned int)time(NULL));

	// Create puzzle with all letters hidden.
	const char HIDDEN = '*';
	ObscuredEntries<char> puzzle(phrase, HIDDEN);
	showPunct(puzzle);
	// Track prize money accumulated by each player.
	const int NUM_PLAYERS = 3;
	int prizeMoney[NUM_PLAYERS] = { 0, 0, 0 };
	int currPlayer = 0;  // start with first player

	// Display puzzle and give each player turns.  Advance to next
	// player if they guess a missing letter, spin a BANKRUPT or
	// spin a LOSE TURN.  Stop when all letters in puzzle are revealed.

	cout << "Disk of Fortune!" << endl;
	bool loop = true;
	while (loop)
	{
		// Wrap around from last to first player (if necessary).
		currPlayer %= NUM_PLAYERS;

		// Display current (one-based) player # with their money.
		cout << "\nPlayer #" << (currPlayer + 1)
			<< ": $" << prizeMoney[currPlayer] << endl;

		// Display current puzzle (some parts may be revealed based on
		// previous turns).
		puzzle.display();

		// Generate a random spin of the disk.
		int spin = rand() % 24;
		string item = ITEMS[spin];
		cout << "Spin: "
			<< (isdigit(item[0]) ? "$" : "")  // "$" if monetary amount
			<< item << endl;

		// Process result of spin.

		if (item == "BANKRUPT")
		{
			cout << "Sorry, you lost your money!" << endl;
			currPlayer++;
		}
		else if (item == "LOSE TURN")
		{
			cout << "Sorry, you lost your turn!" << endl;
			currPlayer++;
		}
		else
		{
			// Allow user to guess a letter.
			cout << "Guess a letter: ";
			char letter;
			cin >> letter;

			// Count how many of that letter found in puzzle.
			int numFound = findLetter(puzzle, letter);

			if (numFound == 0)
			{
				cout << "Sorry, there is no '" << letter << "'" << endl;
				currPlayer++;
			}
			else
			{
				// Give player amount of money spun for each such letter in puzzle.
				int spinMoney = stoi(item);  // string => int (e.g., "450" => 450)
				prizeMoney[currPlayer] += numFound * spinMoney;
				for (int i = 0; i < puzzle.getNumRows(); i++)
				{
					for (int j = 0; j < puzzle.getNumCols(i); j++)
					{
						if (letter == puzzle.getEntry(i, j))
						{
							puzzle.unhide(i, j);
						}
					}
				}
			}
		}

		int flag = 0;
		for (int i = 0; i < puzzle.getNumRows(); i++)
		{
			for (int j = 0; j < puzzle.getNumCols(i); j++)
			{
				if (puzzle.status(i, j) == false)
				{
					flag = 1;
					loop = true;
				}
			}
		}
		if (flag == 0)
		{
			loop = false;
		}
	}

	cout << "\n\nFinished";
	if (prizeMoney[0] > prizeMoney[2] && prizeMoney[0] > prizeMoney[1])
	{
		cout << "\nPlayer #" << 1 << " solved the puzzle\n";
		cout << "Total amount win: " << prizeMoney[0];
	}
	else if (prizeMoney[2] > prizeMoney[1] && prizeMoney[2] > prizeMoney[0])
	{
		cout << "\nPlayer #" << 3 << " solved the puzzle\n";
		cout << "Total amount win: " << prizeMoney[2];
	}
	else
	{
		cout << "\nPlayer #" << 2 << " solved the puzzle\n";
		cout << "Total amount win: " << prizeMoney[1]<<endl;
	}

	system("PAUSE");

	return 0;
}

// Definitions for functions that help game proceed.

// Show any punctuation (non-letters) in puzzle.
// Returns copy of puzzle with punctuation revealed.
ObscuredEntries<char> showPunct(ObscuredEntries<char> puzzle)
{
	// FILL IN THIS FUNCTION
	int row_size = puzzle.getNumRows();
	for (int i = 0; i < row_size; i++)
	{
		int column_size = puzzle.getNumCols(i);
		for (int j = 0; j < column_size; j++)
		{
			if (puzzle.getEntry(i, j) == '\'' || puzzle.getEntry(i, j) == '-')
			{
				puzzle.unhide(i, j);
			}
		}
	}
	return puzzle;
}

// Show a letter (passed) that user guessed in puzzle.
// Return a count of how many of such letter found.
int findLetter(ObscuredEntries<char>& puzzle, char letter)
{
	int COUNT = 0;
	int row_size = puzzle.getNumRows();
	for (int i = 0; i < row_size; i++)
	{
		int column_size = puzzle.getNumCols(i);
		for (int j = 0; j < column_size; j++)
		{
			if (letter == puzzle.getEntry(i, j))
			{
				COUNT++;
			}
		}
	}
	return COUNT;
}

// Return whether puzzle is solved (all letters revealed).
bool solved(const ObscuredEntries<char>& puzzle)
{
	// FILL IN FUNCTION

	int flag = 0;
	int row_size = puzzle.getNumRows();
	for (int i = 0; i < row_size; i++)
	{
		int column_size = puzzle.getNumCols(i);
		for (int j = 0; j < column_size; j++)
		{
			if (puzzle.getEntry(i, j) == '*')
			{
				flag = 1;
				break;
			}
		}
	}

	if (flag == 1) {
		return false;
	}
	else
		return true;

}