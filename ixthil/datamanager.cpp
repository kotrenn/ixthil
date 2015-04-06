#include "datamanager.h"

DataManager *DataManager::m_instance = NULL;

DataManager *DataManager::get_instance()
{
	if (m_instance == NULL)
		m_instance = new DataManager();
	return m_instance;
}

DataManager::DataManager()
	:m_int(),
	 m_str(),
	 m_vec(),
	 m_dbl()
{
	FILE *data;
	if ((data = fopen("data.txt", "r")) == NULL)
	{
		fprintf(stderr, "Could not open data.txt for reading\n");
		exit(1);
	}

	char type[80];
	char name[80];

	while (fscanf(data, "%s %s", type, name) > 0)
	{
		if (strcmp(type, "int") == 0)
		{
			int val;
			fscanf(data, "%d", &val);
			m_int[string(name)] = val;
		}
		else if (strcmp(type, "vec") == 0)
		{
			double x, y;
			fscanf(data, "%lf %lf", &x, &y);
			m_vec[string(name)] = vector2d(x, y);
		}
		else if (strcmp(type, "str") == 0)
		{
			char val[80];
			fscanf(data, "%s", val);
			int i = 1;
			while (val[i])
			{
				val[i - 1] = val[i];
				++i;
			}
			val[i - 2] = '\0';
			m_str[string(name)] = string(val);
		}
		else if (strcmp(type, "dbl") == 0)
		{
			double val;
			fscanf(data, "%lf", &val);
			m_dbl[string(name)] = val;
		}
	}

	fclose(data);

	fflush(stdout);
}

DataManager::~DataManager()
{
}

void DataManager::error(const char *var) const
{
	fprintf(stderr, "Could not find value <%s> in data.txt\n", var);
	exit(1);
}

int DataManager::get_int(const char *var)
{
	string var_str(var);
	if (m_int.find(var_str) == m_int.end())
		error(var);
	return m_int[var_str];
}

void DataManager::get_str(const char *var, char *dst)
{
	string var_str(var);
	if (m_str.find(var_str) == m_str.end())
		error(var);
	strcpy(dst, m_str[var_str].c_str());
}

vector2d DataManager::get_vec(const char *var)
{
	string var_str(var);
	if (m_vec.find(var_str) == m_vec.end())
		error(var);
	return m_vec[var_str];
}

double DataManager::get_dbl(const char *var)
{
	string var_str(var);
	if (m_dbl.find(var_str) == m_dbl.end())
		error(var);
	return m_dbl[var_str];
}
