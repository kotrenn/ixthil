#include "levelselector.h"

#include "datamanager.h"
#include "resourcemanager.h"
#include "system.h"

LevelSelector::LevelSelector()
	:m_level(NULL),
	 m_background(NULL),
	 m_camera(NULL),
	 m_player(NULL)
{
	DataManager *dm = DataManager::get_instance();

	System *system = System::get_instance();

	int system_w;
	int system_h;
	system->get_size(&system_w, &system_h);
	m_background = new Background(Rect(0, 0, system_w, system_h));

	char level_name[81];
	dm->get_str("start", level_name);
	m_level = new Level(level_name, m_background);

	vector2d size = dm->get_vec("level_dims");
	m_camera = new Camera((int)size[0], (int)size[1]);
}

LevelSelector::~LevelSelector()
{
	delete m_level;
	delete m_background;
	delete m_camera;
}

void LevelSelector::pause()
{
	m_level->pause();
	m_background->pause();
}

void LevelSelector::unpause()
{
	m_level->unpause();
	m_background->unpause();
}

void LevelSelector::set_player(PlayerActor *player)
{
	m_player = player;
	m_level->add_actor(m_player);
}

void LevelSelector::update()
{
	m_level->update();
}

void LevelSelector::move(double dt)
{
	m_level->move(dt);
	m_background->update(dt);
	m_camera->move(dt);

	/* Check if level is done; if so move to next */
	if (m_level->done())
		level_done();
}

void LevelSelector::draw(Surface *dst) const
{
	m_camera->clear();
	m_background->draw(m_camera->get_surface());
	m_level->draw(m_camera->get_surface());
	m_camera->draw(dst);
}

void LevelSelector::level_done()
{
	fprintf(stderr, "level_done()\n");

	string next = m_level->get_next();

	if (m_level->entering_subspace())
	{
		int next_subspace = 1;
		char buf[80];
		sprintf(buf, "subspace%d", next_subspace);
		next = buf;
	}

	switch_level(next);
}

void LevelSelector::switch_level(const string &next)
{
	fprintf(stderr, "switch_level(%s)\n", next.c_str());

	DataManager *dm = DataManager::get_instance();

	/* Give the player his crystal reward */
	if (m_level->subspace())
		m_player->give_crystal();

	/* Build the level and copy bullets over */
	Level *new_level = new Level(next.c_str(), m_background);
	m_level->move_bullets(new_level);
	delete m_level;
	m_level = new_level;

	/* Build new camera */
	delete m_camera;
	vector2d size = dm->get_vec("level_dims");
	m_camera = new Camera((int)size[0], (int)size[1]);

	/* Move player to new level */
	m_player->set_level(m_level);
	m_level->add_actor(m_player);

	/* Expand subspace portal */
	if (m_level->subspace())
	{
		vector2d center = m_player->get_center();
		int subspace = m_player->num_subspace();
		m_background->enter_subspace(center, subspace);
	}
	else
		m_background->exit_subspace();

	/* Play title sound?
	   Probably a leftover from Varon Sector
	*/
	char start[81];
	dm->get_str("start", start);
	if (next == start)
	{
		ResourceManager *rm = ResourceManager::get_instance();
		rm->play_sound("begin.wav");
	}
}
