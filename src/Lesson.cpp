#include "pch.h"
#include "Lesson.h"

Lesson::CSS::CSS(CString path, CString rela)
{
	this->path = &path;
	this->rela = &rela;
}

Lesson::CSS::CSS() {
	this->path = NULL;
	this->rela = NULL;
}

Lesson::CSS::~CSS()
{
	delete path;
	delete rela;
}

Lesson::Lesson()
{
	this->checked = new int(0);
	this->files = NULL;
	this->name = NULL;
	this->path = NULL;
	this->phone = NULL;
	this->user = NULL;
}

Lesson::~Lesson()
{
	delete name;
	delete path;
	delete phone;
	delete user;
	if (files) {
		int s = files->size();
		for (int i = 0; i < s; i++) {
			delete files->at(i);
		}
		files->clear();
		vector<CSS*>().swap(*files);
		delete files;
		files = NULL;
	}
}
