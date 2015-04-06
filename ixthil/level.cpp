#include "level.h"

#include "actorfactory.h"
#include "datamanager.h"
#include "enemies/enemies.h"
#include "levelreader.h"
#include "player.h"
#include "resourcemanager.h"

/*
Level::Level(int w, int h, Background *background)
	:m_name(""),
	 m_w(w), m_h(h),
	 m_background(background),
	 m_level(0),
	 m_subspace(false),
	 m_boss(NULL),
	 m_next(""),
	 m_entering_subspace(false),
	 m_actors(), m_actors_add(), m_actors_remove(),
	 m_bullets(), m_bullets_add(), m_bullets_remove(),
	 m_items(), m_items_add(), m_items_remove()
{
}
*/

Level::Level(const char *filename, Background *background)
	:m_name(filename),
	 m_w(0), m_h(0),
	 m_background(background),
	 m_level(0),
	 m_subspace(false),
	 m_player(NULL),
	 m_boss(NULL),
	 m_options(),
	 m_next(""),
	 m_entering_subspace(false),
	 m_actors(), m_actors_add(), m_actors_remove(),
	 m_bullets(), m_bullets_add(), m_bullets_remove(),
	 m_items(), m_items_add(), m_items_remove()
{
	DataManager *dm = DataManager::get_instance();
	vector2d level_dims = dm->get_vec("level_dims");
	m_w = level_dims[0];
	m_h = level_dims[1];

	LevelReader().read(this, filename);

	/*
	ActorFactory *af = ActorFactory::get_instance();
	for (int i = 0; i < 500; ++i)
		add_actor(af->load_actor(this, "bounce"));
	*/
}

Level::~Level()
{
	add_lists();

	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	set<Item *>::iterator i;
	
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		remove_actor(*a);
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		remove_bullet(*b);
	for (i = m_items.begin(); i != m_items.end(); i++)
		remove_item(*i);

	m_actors.clear();
	m_bullets.clear();
	m_items.clear();

	clear_lists();
}

void Level::pause()
{
	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		(*a)->pause();
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		(*b)->pause();
}

void Level::unpause()
{
	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		(*a)->unpause();
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		(*b)->unpause();
}

void Level::update()
{
	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	set<Item *>::iterator i;

	for (a = m_actors.begin(); a != m_actors.end(); a++)
		(*a)->reset_update();

	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
	{
		Bullet *bullet = *b;
		int color = bullet->get_color();
		vector2d center = bullet->get_center();
		m_background->add_value(color, center[0], center[1]);
	}
	for (a = m_actors.begin(); a != m_actors.end(); a++)
	{
		Actor *actor = *a;
		int color = actor->get_color();
		vector2d center = actor->get_center();
		if (color >= 0 && actor != m_player)
			m_background->add_value(color, center[0], center[1]);
	}
	
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		(*a)->update();
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		(*b)->update();
	for (i = m_items.begin(); i != m_items.end(); i++)
		(*i)->update();
	
	// check for stuff out of bounds here
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		if (out_of_bounds(*b))
			remove_bullet(*b);
	for (i = m_items.begin(); i != m_items.end(); i++)
		if (out_of_bounds(*i))
			remove_item(*i);

	add_lists();
	clear_lists();
	
	/* insert portal */
	if (done() && get_option("portal") && m_player != NULL)
	{
		fprintf(stderr, "Inserting portal\n");
		fflush(stderr);
		PlayerActor *player = (PlayerActor *)get_player();
		int subspace = player->num_subspace();
		new PortalItem(this, subspace);
		add_lists();
		set_option("portal", false);
	}
}

void Level::add_lists()
{
	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	set<Item *>::iterator i;
	
	for (a = m_actors_add.begin(); a != m_actors_add.end(); a++)
		m_actors.insert(*a);
	for (b = m_bullets_add.begin(); b != m_bullets_add.end(); b++)
		m_bullets.insert(*b);
	for (i = m_items_add.begin(); i != m_items_add.end(); i++)
		m_items.insert(*i);

	m_actors_add.clear();
	m_bullets_add.clear();
	m_items_add.clear();
}

void Level::clear_lists()
{
	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	set<Item *>::iterator i;
	
	for (a = m_actors_remove.begin();
	     a != m_actors_remove.end(); a++)
	{
		set<Actor *>::iterator a2;
		for (a2 = m_actors.begin(); a2 != m_actors.end(); a2++)
			(*a2)->actor_dying(*a);
		for (b = m_bullets.begin(); b != m_bullets.end(); b++)
			(*b)->actor_dying(*a);
	}
	
	for (a = m_actors_remove.begin();
	     a != m_actors_remove.end(); a++)
	{
		m_actors.erase(*a);
		if ((*a)->get_team() != TEAM_PLAYER)
			delete *a;
	}

	for (b = m_bullets_remove.begin();
	     b != m_bullets_remove.end(); b++)
	{
		m_bullets.erase(*b);
		delete *b;
	}

	for (i = m_items_remove.begin();
	     i != m_items_remove.end(); i++)
	{
		m_items.erase(*i);
		delete *i;
	}

	m_actors_remove.clear();
	m_bullets_remove.clear();
	m_items_remove.clear();
}

void Level::move(double dt)
{
	set<Actor *>::iterator a;
	set<Bullet *>::iterator b;
	set<Item *>::iterator i;
	
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		(*a)->move(dt);
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		(*b)->move(dt);
	for (i = m_items.begin(); i != m_items.end(); i++)
		(*i)->move(dt);
}

void Level::draw(Surface *dst) const
{
	set<Actor *>::const_iterator a;
	set<Bullet *>::const_iterator b;
	set<Item *>::const_iterator i;

	for (i = m_items.begin(); i != m_items.end(); i++)
		(*i)->draw(dst);
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		(*b)->draw(dst);
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		(*a)->draw(dst);

	if (m_boss != NULL)
	{
		if (!m_boss->dead())
		{
			double p = m_boss->get_health();
			double w = 0.8 * m_w;
			double h = 20;
			double x = 0.1 * m_w;
			double y = 0.05 * m_h;
			Uint32 color = mapRGB(255, 0, 0);
			dst->draw_rect(color, Rect(x, y, w, h), 1);
			dst->draw_rect(color, Rect(x, y, p * w, h));
		}
	}
}

void Level::set_option(const string &option, bool value)
{
	m_options[option] = value;
}

bool Level::get_option(const string &option) const
{
	if (m_options.find(option) == m_options.end())
		return false;
	else
		return (*m_options.find(option)).second;
}

bool Level::done()
{
	/* check if we have more than just the player */
	if (m_actors_add.size() > 0)
		return false;
	if (m_actors.size() > 1)
		return false;

	/* check if any items to grab such as subspace portal */
	if (m_items_add.size() > 0)
		return false;
	if (m_items.size() > 0)
		return false;

	/* check if endless */
	if (get_option("endless"))
	{
		Enemies::random_enemies(this);
		return false;
	}

	return true;
}

void Level::add_actor(Actor *actor)
{
	if (actor->get_team() == TEAM_PLAYER)
		m_player = actor;
	
	m_actors_add.insert(actor);
}

void Level::add_bullet(Bullet *bullet)
{
	m_bullets_add.insert(bullet);
}

void Level::add_item(Item *item)
{
	m_items_add.insert(item);
}

void Level::remove_actor(Actor *actor)
{
	if (m_player == actor)
		m_player = NULL;
	if (m_boss == actor)
		m_boss = NULL;
	
	m_actors_remove.insert(actor);
}

void Level::remove_bullet(Bullet *bullet)
{
	m_bullets_remove.insert(bullet);
}

void Level::remove_item(Item *item)
{
	m_items_remove.insert(item);
}

void Level::move_bullets(Level *level)
{
	set<Bullet *>::iterator b;
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
	{
		level->add_bullet(*b);
		(*b)->set_level(level);
	}
	m_bullets.clear();
}

void Level::clear_bullets()
{
	m_bullets.clear();
}

bool Level::out_of_bounds(const Bullet *bullet) const
{
	Rect bounds(-0.1 * m_w, -0.1 * m_h,
	            1.2 * m_w, 1.2 * m_h);
	Shape *shape = bullet->get_shape();
	return !bounds.collides(shape);
}

bool Level::out_of_bounds(const Item *item) const
{
	Rect bounds(0, 0, m_w, m_h);
	const Shape *shape = item->get_shape();
	return !bounds.collides(shape);
}

void Level::get_collisions(list<Actor *> *actors,
                           const Shape *shape,
                           int team) const
{
	set<Actor *>::const_iterator a;
	for (a = m_actors.begin(); a != m_actors.end(); a++)
		if ((*a)->get_team() != team)
			if ((*a)->get_shape()->collides(shape))
				actors->push_back(*a);
}

void Level::get_collisions(list<Bullet *> *bullets,
                           const Shape *shape,
                           int team) const
{
	set<Bullet *>::const_iterator b;
	for (b = m_bullets.begin(); b != m_bullets.end(); b++)
		if ((*b)->get_team() != team)
			if ((*b)->get_shape()->collides(shape))
				bullets->push_back(*b);
}

void Level::get_collisions(list<Item *> *items,
                           const Shape *shape) const
{
	set<Item *>::const_iterator i;
	for (i = m_items.begin(); i != m_items.end(); i++)
		if ((*i)->get_shape()->collides(shape))
			items->push_back(*i);
}

Actor *Level::random_target(int team) const
{
	list<Actor *> actors;
	
	set<Actor *>::const_iterator as;
	for (as = m_actors.begin(); as != m_actors.end(); as++)
		if ((*as)->get_team() != team)
			actors.push_back(*as);

	if (actors.size() == 0)
		return NULL;

	int n = randint(0, actors.size() - 1);

	list<Actor *>::iterator a = actors.begin();
	for (int i = 0; i < n; ++i)
		a++;

	return *a;
}
