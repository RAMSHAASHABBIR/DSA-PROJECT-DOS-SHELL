#pragma once
#include "Folder.h"
#include <string>
#include <iostream>
#include <filesystem>
using namespace std;



class tree
{
private:
	string prompt = "\\>";
public:
	Folder* root;
	Folder* CF;
	tree()
	{
		CF = root = new Folder("V");
	}
	void changePrompt() {
		prompt= (prompt == "\\>") ? "\\$" : "\\>";
	}
	void print()
	{
		if (CF->name == "V")
		{
			cout << "V:" << prompt;	
		}
		else
		{
			cout << "V:" << CF->path.substr(2, CF->path.length()) << "\\" << CF->name << prompt;
		}
	}
	bool folderExists(string name)
	{
		for (int i=0;i<CF->folder.size();i++)
		{
			if (CF->folder[i]->name==name)
			{
				return true;
			}
		}
		return false;
	}
	
	void setPath(Folder* f)
	{
		string path = CF->name;
		Folder* parent = new Folder();
		while (parent!=nullptr)
		{
			path = parent->name + "/" + path;
			parent = parent->parent;
		}
		f->path = path;
	}

	void printPath(Folder* f) {
		cout << "Current Path: " << f->path << "\\" <<f->name << endl;
	}
};

