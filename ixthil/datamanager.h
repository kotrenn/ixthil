#ifndef __DATA_MANAGER_H
#define __DATA_MANAGER_H

#include "common.h"

class DataManager
{
public:
	static DataManager *get_instance();
	~DataManager();

	int get_int(const char *var);
	void get_str(const char *var, char *dst);
	vector2d get_vec(const char *var);
	double get_dbl(const char *var);
private:
	DataManager();
	static DataManager *m_instance;

	void error(const char *var) const;

	map<string, int> m_int;
	map<string, string> m_str;
	map<string, vector2d> m_vec;
	map<string, double> m_dbl;
};

#endif
