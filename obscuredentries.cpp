/*
 * Name: YOUR NAME
 * HiddenEntries Class Member Function Definitions
 * Course: CSI218 (Fall 2020)
 * Date: THE DATE
 * Description: HiddenEntries member function definitions.
 */

#ifndef OBSCUREDENTRIES_CPP
#define OBSCUREDENTRIES_CPP

#include <iostream>
#include <vector>
#include "obscuredentries.h"
using namespace std;

// Definitions for template class member functions.

// Constructor to store entries (vector passed) and set all as
// hidden.  Value displayed for hidden entries is also passed.
template<class T>
ObscuredEntries<T>::ObscuredEntries(
	const vector<vector<T>>& values, const T& hiddenVal)
{
	// Make copy of entries (vectors know how to copy).
	contents = values;
	// Record what value to display for hidden entries.
	hiddenValue = hiddenVal;
	// CREATE AND SET UP 2D ARRAY OF BOOLEANS.
	int size = contents.size();
	shown = new bool* [size];
	for (int i = 0; i < size; i++)
	{
		shown[i] = new bool[100];
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			shown[i][j] = false;
		}
	}
}

// Return number of rows in 2D entries.
template<class T>
int ObscuredEntries<T>::getNumRows() const
{
	return contents.size();  // outer vector size
}

// Return number of columns in 2D entries for particular row.
//
// Precondition: row passed should be within valid # of
// rows (indices start at zero).
template<class T>
int ObscuredEntries<T>::getNumCols(int row) const
{
	return contents[row].size();  // inner vector size
}

// Display 2D entries (each row on separate line, certain
// entries hidden by displaying special value).
template<class T>
void ObscuredEntries<T>::display() const
{
	// Nested loop to display all rows/cols.
	for (int r = 0; r < contents.size(); r++)
	{
		for (int c = 0; c < contents[r].size(); c++)
			if (shown[r][c])
				cout << contents[r][c];  // Display value
			else
				cout << hiddenValue;  // Hide value

		cout << endl;  // each row on own line
	}
}

// Get value at particular location from 2D entries.
//
// Precondition: row and col passed should be within
// valid # of rows and columns (indices start at zero).
template<class T>
T ObscuredEntries<T>::getEntry(int row, int col) const
{
	return contents[row][col];
}

// Reveal entry at particular location within 2D entries.
//
// Precondition: row and col passed should be within
// valid # of rows and columns (indices start at zero).
template<class T>
void ObscuredEntries<T>::unhide(int row, int col)
{
	// Allow showing entry value at specified position.
	shown[row][col] = true;
}
template<class T>
bool ObscuredEntries<T>::status(int row, int col)
{
	return shown[row][col];
}

#endif