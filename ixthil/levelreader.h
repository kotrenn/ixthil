#ifndef __LEVEL_READER_H
#define __LEVEL_READER_H

#include "level.h"

class Level;

class LevelReader
{
public:
	LevelReader();
	~LevelReader();

	void read(Level *level, const char *filename) const;
private:
	vector<string> tokenize(FILE *file) const;
	int parse(Level *level,
	          const vector<string> &tokens,
	          int cur) const;
};

#endif
