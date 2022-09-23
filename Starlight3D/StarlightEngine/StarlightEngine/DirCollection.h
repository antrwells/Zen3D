#pragma once
#include <string>
#include <vector>
#include "tinydir.h"

struct DirEntry {

	std::string name;
	std::string full;
	std::string ext;
	bool folder = false;

};

class DirCollection
{

public:

	DirCollection(std::string path) {


		std::vector<DirEntry> ent;

		ent.resize(0);

		std::wstring ws = std::wstring(path.begin(), path.end());

		tinydir_dir dir;
		tinydir_open(&dir,ws.c_str());

		while (dir.has_next) {

			tinydir_file file;
			tinydir_readfile(&dir, &file);

			std::wstring ws = std::wstring(file.path);

			std::wstring ns = std::wstring(file.name);

			std::wstring es = std::wstring(file.extension);

			std::string fp = std::string(ws.begin(), ws.end());
			std::string np = std::string(ns.begin(), ns.end());
			std::string ep = std::string(es.begin(), es.end());

			/*
			printf("File:");
			printf(fp.c_str());
			//printf(np.c_str());
			printf("\n");
			printf("Ext:");
			printf(ep.c_str());
			printf("\n");
			*/

			DirEntry entry;

			entry.full = fp;
			entry.name = np;
			entry.ext = ep;

			if (file.is_dir) {

				entry.folder = true;

			}
			else {

				entry.folder = false;
			}

			ent.push_back(entry);


			tinydir_next(&dir);

		}

		enteries = ent;


	}

	std::vector<DirEntry> enteries;

};

