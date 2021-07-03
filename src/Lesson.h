#pragma once

#include "pch.h"
#include "EasiNotePacker.h"
#include <vector>

using namespace std;

class Lesson
{
public:
	class CSS {
	public:
		CString *path, *rela;
		CSS(CString path,CString rela);
		CSS();
		~CSS();
	};
	CString* path;
	CString* user;
	CString* phone;
	CString* name;
	int* checked;
	vector<CSS*>* files;	//["C:\...\MD5.xml","Slides\Slide_0.xml"],[...,...]
	Lesson();
	~Lesson();
};

