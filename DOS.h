#pragma once
#include "tree.h"
#include <cctype>
#include <Windows.h>
#include <string>
#include <iostream>
#include <cctype>
#include <sstream>
#include <list>
#include <queue>
#include <fstream>

using namespace std;
class DOS
{
private:
	string lower(string s)
	{
		for (int i = 0;i < s.length();i++)
		{
			s[i] = tolower(s[i]);
		}
		return s;
	}
	bool prompt = true;
	string savePath = "save.txt";
	void saveStateToFile(Folder* rootFolder, ofstream& saveFile) {
		if (rootFolder) {
			saveFile << rootFolder->name << " " << rootFolder->path << " " << rootFolder->hidden << endl;
			for (Folder* subfolder : rootFolder->folder) {
				saveStateToFile(subfolder, saveFile);
			}
			for (File* file : rootFolder->file) {
				saveFile << file->name << " " << file->user << " " << file->priority << " "
					 << " " << file->format << " " << file->size << " "
					<< file->noOfLines << " " << file->avrCharPrLine << " ";
				for (char character : file->Data) {
					saveFile << character;
				}
				saveFile << endl;
			}
		}
	}
	void saveStateToFile() {
		ofstream saveFile(savePath);
		if (saveFile.is_open()) {
			saveStateToFile(t.root, saveFile);
			saveFile.close();
		}
		else {
			cout << "Unable to save the state to file." << endl;
		}
	}

public:
	queue<File*> PQueue;
	priority_queue<File*, vector<File*>, File::Compare> PriorityQueue;
	queue<File*> tempQueue;

	tree t;
	string user = "ramsha";
	DOS()
	{

	}

	~DOS() {
		saveStateToFile();
	}
	void printHeader
	()
	{
		cout << "              " << endl;
		cout << "       RAMSHA  SHABBIR  " << endl;
		cout << "         2022-CS-205  " << endl;
		cout << "DSA - COMPUTER SCIENCE - 2022" << endl;
	}
	void run()
	{
		printHeader();
		bool exit = false;
		while (!exit)
		{
			t.print();
			exit = input();
		}
	}
	bool input()
	{
		string command;
		getline(cin, command);
		string opr = command.substr(0, command.find(' '));
		opr = lower(opr);
		if (opr == "mkdir") {
			string newFolderName = command.substr(6, command.length());
			if (t.CF->findFolder(newFolderName) != nullptr) {
				cout << "A folder with the name '" << newFolderName << "' already exists in the current directory." << endl;
			}
			else {
				t.CF->insertFolder(new Folder(newFolderName, t.CF->path + "\\" + t.CF->name, t.CF));
				saveStateToFile();
			}
		}
		else if (opr == "create") {
			string newFileName = command.substr(7, command.length());
			if (t.CF->findFile(newFileName) != nullptr) {
				cout << "A file with the name '" << newFileName << "' already exists in the current directory." << endl;
			}
			else {
				string priority;
				string readonly;
				string format;
				if (fileAttributes(priority, format)) {
					int prio = stoi(priority);
					bool read = stringToBool(readonly);
					t.CF->insertFile(new File(newFileName, user, prio, read, format));
					saveStateToFile();
				}
			}
		}
		else if (opr == "attrib")
		{
			string file = command.substr(7, command.length());
			File* f = t.CF->findFile(file);
			cout << "name:" << f->name << endl;
			cout << "priority:" << f->priority << endl;
			if (f->readonly == 1)
				cout << "readonly:" << "yes" << endl;
			else
				cout << "readonly:" << "no" << endl;
			cout << "format:" << f->format << endl;
		}
		else if (opr == "cd")
		{
			string folder = command.substr(3, command.length());
			if (t.folderExists(folder))
			{
				t.CF = t.CF->findFolder(folder);
			}
			else
			{
				cout << "NO such directory exists" << endl;
			}
			saveStateToFile();

		}
		else if (opr == "cd..")
		{
			if (t.CF->parent != nullptr)
			{
				t.CF = t.CF->parent;
			}
		}
		else if (opr == "cd\\")
		{
			t.CF = t.root;
		}
		else if (opr == "dir" || opr == "cd.")
		{
			t.CF->printFolderFile();
		}
		else if (opr == "format")
		{
			t.CF->EmpDir();
			saveStateToFile();
		}
		else if (opr == "del")
		{
			t.CF->removeFile(command.substr(4, command.length()));
			saveStateToFile();
		}
		else if (opr == "rmdir")
		{
			Folder* f = t.CF;
			t.CF = t.CF->parent;
			t.CF->removeFolder(f->name);
			saveStateToFile();
		}
		else if (opr == "rename")
		{
			command = command.substr(opr.length() + 1, command.length());
			string currentName = command.substr(0, command.find(" "));
			string newName = command.substr(currentName.length() + 1, command.length());
			File* f = t.CF->findFile(currentName);
			if (f)
			{
				f->name = newName;
				cout << "FILE is renamed succesfully" << endl;
			}
			else
				cout << "FILE not found " << endl;
			saveStateToFile();
		}
		else if (opr == "cls")
		{
			system("cls");
			printHeader();

		}
		else if (opr == "exit")
		{
			return true;
		}
		else if (opr == "tree")
		{
			Folder* f = t.CF;
			t.CF->displayFolder(f, 0);
		}
		else if (opr == "find")
		{
			string folder = command.substr(5, command.length());
			File* f = t.CF->findFile(folder);
			if (f)
				cout << "file exists" << endl;
			else
				cout << "no such file" << endl;
		}
		else if (opr == "mov")
		{
			command = command.substr(4, command.length());
			int ind = command.find(' ');
			string sourcepath = command.substr(0, ind);
			string destinationpath = command.substr(ind + 1, command.length());
			stringstream d1 = stringstream(destinationpath);
			Folder* currentFolder1 = t.CF;
			string f;
			while (getline(d1, f, '\\')) {
				Folder* next = currentFolder1->findFolder(f);
				if (next == nullptr) {
					return false;
				}
				currentFolder1 = next;
			}
			stringstream s1 = stringstream(sourcepath);
			File* file_to_move = nullptr;
			Folder* currentFolder = t.CF;
			string g;
			while (getline(s1, g, '\\')) {
				Folder* next2 = currentFolder->findFolder(g);
				if (next2 == nullptr) {
					File* next = currentFolder->findFile(g);
					if (next == nullptr)
					{
						return false;
					}
					else {
						currentFolder->removeFile(next->name);
						currentFolder1->insertFile(next);
					}
				}
				currentFolder = next2;
			}
			saveStateToFile();

			return false;
		}
		else if (opr == "copy")
		{
			command = command.substr(opr.length() + 1, command.length());
			int ind = command.find(' ');
			string sourcepath = command.substr(0, ind);
			string destinationpath = command.substr(ind + 1, command.length());
			stringstream d1 = stringstream(destinationpath);
			Folder* currentFolder1 = t.CF;
			string f;
			while (getline(d1, f, '\\')) {
				Folder* next = currentFolder1->findFolder(f);
				if (next == nullptr) {
					return false;
				}
				currentFolder1 = next;
			}
			stringstream s1 = stringstream(sourcepath);
			File* file_to_move = nullptr;
			Folder* currentFolder = t.CF;
			string g;
			while (getline(s1, g, '\\')) {
				Folder* next2 = currentFolder->findFolder(g);
				if (next2 == nullptr) {
					File* next = currentFolder->findFile(g);
					if (next == nullptr)
					{
						return false;
					}
					else {

						currentFolder1->insertFile(next);
					}
				}
				currentFolder = next2;
			}
			saveStateToFile();

			return false;

		}
		else if (opr == "convert")
		{
			string fileToConvert = command.substr(8, command.length());
			File* f = t.CF->findFile(fileToConvert);
			if (f)
			{
				string newFormat;
				cout << "Enter the new format/extension: ";
				cin >> newFormat;
				if (newFormat == "txt" || newFormat == "text")
				{
					f->format = newFormat;
					cout << "File format changed to " << newFormat << endl;
				}
				else
					cout << "Invalid format. File format remains unchanged." << endl;
			}
			else
				cout << "File not found." << endl;
			saveStateToFile();

		}
		else if (opr == "pwd")
		{
			t.printPath(t.CF);

		}
		else if (opr == "edit")
		{
			string file = command.substr(5, command.length());
			if (t.CF->findFile(file) != nullptr)
			{
				File* f = t.CF->findFile(file);
				f->printnotepad();
				f->insertdata();
			}
			else
			{
				cout << "NO such file exists" << endl;
			}
			saveStateToFile();
		}
		else if (opr == "ver")
		{
			t.CF->version();
		}
		else if (opr == "print") {
			string file = command.substr(6, command.length());
			if (t.CF->findFile(file) != nullptr) {
				File* f = t.CF->findFile(file);

				bool found = false;
				queue<File*> tempQueueCopy = PQueue;
				while (!tempQueueCopy.empty()) {
					if (f->name == tempQueueCopy.front()->name) {
						found = true;
						break;
					}
					tempQueueCopy.pop();
				}
				if (!found) {
					PQueue.push(f);
					cout << "File added to the print queue." << endl;
				}
				else {
					cout << "File is already in the print queue." << endl;
				}
			}
			else {
				cout << "No such file exists." << endl;
			}
		}
		else if (opr == "queue")
		{
			queue<File*> tempQueue = PQueue;
			while (!tempQueue.empty()) {
				cout << "NAME:" << tempQueue.front()->name << " " << "FORMAT:" << tempQueue.front()->format << " " << "PRIORITY:" << tempQueue.front()->priority;
				tempQueue.pop();
				cout << endl;
			}
		}
		else if (opr == "pprint") {
			string file = command.substr(7, command.length());
			if (t.CF->findFile(file) != nullptr) {
				File* f = t.CF->findFile(file);
				if (!isFileInPriorityQueue(f)) {
					PriorityQueue.push(f);
				}
				else {
					cout << "File is already in the priority queue." << endl;
				}
			}
			else {
				cout << "NO such file exists" << endl;
			}
		}
		else if (opr == "pqueue") {
			priority_queue<File*, vector<File*>, File::Compare> tempQueueCopy = PriorityQueue;
			while (!tempQueueCopy.empty()) {
				cout << "NAME: " << tempQueueCopy.top()->name << " FORMAT: " << tempQueueCopy.top()->format << " PRIORITY: " << tempQueueCopy.top()->priority << endl;
				tempQueueCopy.pop();
			}
		}
		else if (opr == "prompt") {
			t.changePrompt();
		}
		else if (opr == "help")
		{
			cout << "ATTRIB : Displays file (name provided as input) attributes." << endl;
			cout << "CD : Displays the name of or changes the current directory." << endl;
			cout << "CD. or DIR : Prints your current working directory " << endl;
			cout << "CD.. or dir : Change directory to previous directory" << endl;
			cout << "CD\ : Changes directory to root directory " << endl;
			cout << "CLS : Clear the screen" << endl;
			cout << "CONVERT : Asks two types and converts extension of all files of one type to another type." << endl;
			cout << "COPY : Copies one file in the current directory to another location" << endl;
			cout << "CREATE : Creates a file with the name provided." << endl;
			cout << "DEL : Delete a file whose name is provided in input." << endl;
			cout << "FIND : Searches for a file in your current virtual directory whose name is provided as input." << endl;
			cout << "FORMAT : Formats the current virtual directory i.e. empties the current directory and all subdirectories." << endl;
			cout << "EDIT : Opens a file to edit." << endl;
			cout << "MKDIR : Creates a virtual directory." << endl;
			cout << "MOVE : Moves one file (whose name is provided as input) from one directory to another." << endl;
			cout << "PROMPT : Changes the Windows command prompt (for example from V:\&gt; to V$)." << endl;
			cout << "PWD : Prints working directory" << endl;
			cout << "RENAME : Renames a file whose current and new name is provided as input." << endl;
			cout << "PPRINT : Add a file to priority queue." << endl;
			cout << "PQUEUE : Print the files in priority queue." << endl;
			cout << "PRINT : Add a file in a queue." << endl;
			cout << "QUEUE : Print the files in queue." << endl;
			cout << "PWD : Prints working directory." << endl;
			cout << "PROMPT : Changes the window command prompt" << endl;
			cout << "RMDIR : Removes a directory along with its contents." << endl;
			cout << "TREE : Displays the complete directory structure." << endl;
			cout << "SAVE : Saves the currently open file to disk." << endl;
			cout << "VER : Displays the version of your program." << endl;
			cout << "EXIT : Quits the program" << endl;
		}
		if (opr != "cls")
		{
		}
		return false;
	}
	bool fileAttributes(string& priority,  string& format)
	{
		cout << "ENTER PRIORITY OF FILE (1-5): ";
		getline(cin, priority);

		cout << "ENTER ITS EXTENSION (txt,text): ";
		getline(cin, format);
		if (is_integer(priority) && (format == "txt" || format == "text")) {
			int prio = stoi(priority);
		
			if (prio >= 1 && prio <= 5)  {
				cout << "Valid inputs" << endl;
				return true;
			}
			else {

				cout << "Invalid input values" << endl;
				return false;
			}
		}
		else {
			cout << "Invalid input format" << endl;
			return false;
		}
	}
	bool is_integer(string& s)
	{
		stringstream ss(s);
		int x;
		return (ss >> x) && (ss.eof());
	}
	bool stringToBool(string& str)
	{
		return (str == "true" || str == "1");
	}

	bool isFileInPriorityQueue(File* file) {
		priority_queue<File*, vector<File*>, File::Compare> tempQueueCopy = PriorityQueue;
		while (!tempQueueCopy.empty()) {
			if (tempQueueCopy.top() == file) {
				return true;
			}
			tempQueueCopy.pop();
		}
		return false;
	}


};
