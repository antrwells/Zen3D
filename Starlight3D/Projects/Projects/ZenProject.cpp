#include "pch.h"
#include "ZenProject.h"

ZenProject::ZenProject() {

	mName = "New Project";

}

void ZenProject::SetName(std::string name) {

	mName = name;

}

void ZenProject::SetAuthor(std::string author) {

	mAuthor = author;

}

void ZenProject::SetPath(std::string path) {

	mPath = path;

}

void ZenProject::SetInfo(std::string info) {

	mInfo = info;

}