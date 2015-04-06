#include "levelreader.h"

#include "actorfactory.h"
#include "color.h"
#include "enemies/enemies.h"
#include "resourcemanager.h"

LevelReader::LevelReader()
{
}

LevelReader::~LevelReader()
{
}

void LevelReader::read(Level *level, const char *filename) const
{
	fprintf(stderr, "Reading level \"%s\"\n", filename);
	
	char fullname[81];
	snprintf(fullname, 80, "levels/%s.txt", filename);

	FILE *file;
	if ((file = fopen(fullname, "r")) == NULL)
	{
		fprintf(stderr, "Could not open level %s\n", filename);
		return;
	}

	vector<string> tokens = tokenize(file);
	
	fclose(file);

	int cur = 0;
	while (cur < (int)tokens.size())
		cur += parse(level, tokens, cur);
}

vector<string> LevelReader::tokenize(FILE *file) const
{
	vector<string> ret;

	char buf[81];
	while (fscanf(file, "%80s", buf) > 0)
		ret.push_back(buf);

	return ret;
}

int LevelReader::parse(Level *level,
                       const vector<string> &tokens,
                       int cur) const
{
	ActorFactory *af = ActorFactory::get_instance();
	ResourceManager *rm = ResourceManager::get_instance();

	vector2d cloud_center(0.5 * level->get_w(), -50);
	Enemies::set_cloud_center(cloud_center);
	
	int ret = 1;
	
	if (tokens[cur] == "repeat")
	{
		int n = atoi(tokens[cur + 1].c_str());
		int skip = 0;
		for (int i = 0; i < n; ++i)
			skip = parse(level, tokens, cur + 2);
		ret = 2 + skip;
	}
	else if (tokens[cur] == "enable")
	{
		level->set_option(tokens[cur + 1], true);
		ret = 2;
	}
	else if (tokens[cur] == "level")
	{
		int cur_level = atoi(tokens[cur + 1].c_str());
		level->set_level(cur_level);
		ret = 2;
	}
	else if (tokens[cur] == "actor")
	{
		Actor *actor = af->load_actor(level, tokens[cur + 1].c_str());
		level->add_actor(actor);
		ret = 2;
	}
	else if (tokens[cur] == "random")
	{
		Enemies::random_enemies(level);
		ret = 1;
	}
	else if (tokens[cur] == "music")
	{
		rm->play_music(tokens[cur + 1].c_str());
		ret = 2;
	}
	else if (tokens[cur] == "next")
	{
		level->set_next(tokens[cur + 1]);
		ret = 2;
	}
	else if (tokens[cur] == "subspace")
	{
		level->set_subspace(true);
		ret = 1;
	}
	else if (tokens[cur] == "cloud_center")
	{
		double x = atof(tokens[cur + 1].c_str());
		int y = atoi(tokens[cur + 2].c_str());
		fprintf(stderr, "Setting cloud center: %.1lf %d\n", x, y);
		cloud_center = vector2d(x * level->get_w(), y);
		Enemies::set_cloud_center(cloud_center);
		ret = 3;
	}
	else
		fprintf(stderr, "Level reader:  unknown command \"%s\"\n",
		        tokens[cur].c_str());
	
	return ret;
}	
