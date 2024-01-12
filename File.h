#pragma once
#include <list>
#include<vector>
#include<string>
#include<iostream>
#include<ctime>
#include "Folder.h"
#include <conio.h>
#include <deque>
using namespace std;
class File
{
public:
	string name;
	string user;
	int priority;
	bool readonly;
	string format;
	int size;
	int noOfLines;
	int avrCharPrLine;
	int cursorIndex;
	list<char> Data;
	deque<list<char>> cursorHistory;
	deque<list<char>> redo;
public:
	File(string n = "", string User = "", int prio = 0, bool ro = false, string fmt = "", int s = 0, int lines = 0, int avgChar = 0, list <char> listdata = {})
	{
		name = n;
		user = User;
		priority = prio;
		readonly = ro;
		format = fmt;
		size = s;
		noOfLines = lines;
		avrCharPrLine = avgChar;
	}

	void printnotepad() {
		system("cls");
		int i = 0;
		for (char& character : Data)
		{
			if (i == cursorIndex)
			{
				cout << "|";
			}
			cout << character;
			i++;
		}
		if (cursorIndex == i) {
			cout << "|";
		}
	}
	char getCharAt(list<char>& lst, int index) {
		auto it = lst.begin();
		advance(it, index);
		return *it;
	}
	void insertdata() {
		int userInput;
		while (true) {
			system("cls");
			printnotepad();

			userInput = _getch();
			if (userInput == 224) {
				userInput = _getch();

				if (userInput == 75 && cursorIndex > 0)
				{
					cursorIndex--;
				}
				else if (userInput == 77 && cursorIndex < Data.size())
				{
					cursorIndex++;
				}
			}
			else if (userInput == 8)
			{
				if (cursorIndex > 0) {
					cursorHistory.push_back(Data);
					Data.erase(next(Data.begin(), cursorIndex - 1));
					cursorIndex--;
				}
			}
			else if (userInput >= 32 && userInput <= 126)
			{
				cursorHistory.push_back(Data);
				redo.clear();
				Data.insert(next(Data.begin(), cursorIndex), static_cast<char>(userInput));
				cursorIndex++;
			}
			else if (userInput == 13)
			{
				cursorHistory.push_back(Data);
				redo.clear();
				Data.insert(next(Data.begin(), cursorIndex), '\n');
				cursorIndex++;
			}

			else if (userInput == 26)
			{
				if (!cursorHistory.empty())
				{
					redo.push_back(Data);
					Data = cursorHistory.back();
					cursorHistory.pop_back();
				}
			}
			else if (userInput == 25)
			{
				if (!redo.empty())
				{
					cursorHistory.push_back(Data);
					Data = redo.back();
					redo.pop_back();
				}
			}
			else { 
				break;
			}
		}
	}
	struct Compare {
		bool operator()(const File* file1, const File* file2) const {
			return file1->priority < file2->priority;
		}
	};


};
