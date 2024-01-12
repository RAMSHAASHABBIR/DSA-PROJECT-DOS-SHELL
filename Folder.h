#pragma once
#include <list>
#include<vector>
#include<string>
#include<iostream>
#include<ctime>
#include "File.h"
using namespace std;
class Folder
{
public:
	string name;
	vector <Folder*> folder;
	vector <File*> file;
	Folder* parent;
	time_t creation_time;
	string owner;
	string path;
	bool hidden;
	Folder(string n = "", string p = "", Folder* Parent = nullptr, vector <File*> fs = vector <File*>(), vector <Folder*> Fs = vector <Folder*>(),bool hidd = false)
	{
		name = n;
		path = p;
		parent = Parent;
		file = fs;  
		folder = Fs;
		hidden = hidd;
	}
	void insertFolder(Folder* f)
	{
		folder.push_back(f);
	}
	void insertFile(File* f)
	{
		file.push_back(f);
	}
	void removeFile(string name)
	{
		for (int i=0;i<file.size();i++)
		{
			if (file[i]->name==name)
			{
				swap(file[i], file[file.size() - 1]);
				file.pop_back();
			}
		}
	}
	void removeFolder(string name)
	{
		for (int i = 0;i < folder.size();i++)
		{
			if (folder[i]->name == name)
			{
				swap(folder[i], folder[folder.size() - 1]);
				folder.pop_back();
			}
		}
	}
	void EmpDir()
	{
		folder.resize(0);
		file.resize(0);
	}
	File* findFile(string name)
	{
		for (int i = 0;i < file.size();i++)
		{
			if (file[i]->name == name)
			{
				return file[i];
			}
		}
		return nullptr;
	}
	Folder* findFolder(string name)
	{
		for (int i = 0;i < folder.size();i++)
		{
			if (folder[i]->name == name)
			{
				return folder[i];
			}
		}
		return nullptr;
	}
	
	void printFolderFile()
	{
		cout << "Directory of " << path << "\\" << name << endl << endl;
		for (int i=0;i<folder.size();i++)
		{
			cout << "              <DIR>        " << folder[i]->name << endl;
		}
		for (int i=0;i<file.size();i++)
		{
			cout << "                           " << file[i]->name << endl;
		}
	}
	void displayFolder(Folder* folder, int depth) {
		for (int i = 0; i < depth; ++i)
			cout << "\t";
		    cout << "|--" << folder->name << std::endl;

		for (Folder* subfolder : folder->folder)
			displayFolder(subfolder, depth + 1);

		for (File* file : folder->file) {
			for (int i = 0; i <= depth; ++i)
				std::cout << "\t";
			std::cout << "|--" << file->name << "." << file->format << std::endl;
		}
	}
	void version() {
		cout << "DOS Shell - A simple command-line file system simulation." << endl;
		cout << "Created by: RAMSHA SHABBIR" << endl;
		cout << "Version: 1.0" << endl;
		cout << "Release Date: 21-12-2023" << endl;
		
	}


};
