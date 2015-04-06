#include "enemies.h"

#include "../actorfactory.h"

bool Enemies::m_initialized = false;
int Enemies::m_history[Enemies::m_history_size];
vector2d Enemies::m_cloud_center(0, 0);

void Enemies::random_enemies(Level *level)
{
	if (!m_initialized)
		for (int i = 0; i < m_history_size; ++i)
			m_history[i] = -1;

	vector<string> actor_names;
	actor_names.push_back("attractor");
	actor_names.push_back("simple");
	actor_names.push_back("strong");
	actor_names.push_back("bounce");
	
	int type;
	bool done = false;
	printf("history:");
	for (int i = 0; i < m_history_size; ++i)
		printf(" %d", m_history[i]);
	printf("\n");
	fflush(stdout);
	while (!done)
	{
		done = true;
		type = randint(1, actor_names.size()) - 1;
		for (int i = 0; i < m_history_size; ++i)
			if (type == m_history[i])
				done = false;
	}
	string actor_name = actor_names[type];

	ActorFactory *af = ActorFactory::get_instance();
	if (randint(1, 5) > 1 || !level->get_option("title") || true)
	{
		int n = randint(5, 10);
		for (int i = 0; i < n; ++i)
		{
			int r = 1;
			if (actor_name == "cloud")
				r = 2;
			for (int j = 0; j < r; ++j)
			{
				Actor *actor = af->load_actor(level, actor_name);
				level->add_actor(actor);
			}
		}

		for (int i = 0; i < m_history_size - 1; ++i)
			m_history[i + 1] = m_history[i];

		printf("type = %d\n", type);
		m_history[0] = type;
	}
	else
	{
		vector<string> boss_names;
		boss_names.push_back("quick");
		boss_names.push_back("teleporter");
		boss_names.push_back("ballos");
		boss_names.push_back("big");
		type = randint(1, boss_names.size()) - 1;
		string boss_name = boss_names[type];
		
		Actor *actor = af->load_actor(level, boss_name);
		level->add_actor(actor);
	}
}

void Enemies::set_cloud_center(const vector2d &center)
{
	Enemies::m_cloud_center = center;
}
