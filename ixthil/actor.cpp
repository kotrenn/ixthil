#include "actor.h"

#include "actorfactory.h"
#include "datamanager.h"
#include "resourcemanager.h"
#include "item.h"
#include "player.h"

Actor::Actor(const string &name)
	:m_level(NULL),
	 m_name(name),
	 m_shape(NULL),
	 m_v(0, 0),
	 m_team(TEAM_ENEMY),
	 m_health(1),
	 m_dead(false),
	 m_invincible(0),
	 m_color(COLOR_BLANK),
	 m_prev_hit_color(COLOR_BLANK),
	 m_death_spawn(""),
	 m_score(0),
	 m_items(false),
	 m_item_rate(1),
	 m_moved(false),
	 m_parent(NULL)
{
}

Actor::Actor(Level *level, const string &name)
	:m_level(level),
	 m_name(name),
	 m_shape(NULL),
	 m_v(0, 0),
	 m_team(TEAM_ENEMY),
	 m_health(1),
	 m_dead(false),
	 m_invincible(0),
	 m_color(COLOR_BLANK),
	 m_prev_hit_color(COLOR_BLANK),
	 m_death_spawn(""),
	 m_score(0),
	 m_items(true),
	 m_item_rate(1),
	 m_moved(false),
	 m_parent(NULL)
{
	ResourceManager *rm = ResourceManager::get_instance();

	char filename[81];
	snprintf(filename, 80, "actors/%s.png", name.c_str());
	m_surface = rm->get_image(filename);
}

Actor::~Actor()
{
	delete m_shape;
}

Actor *Actor::clone(Level *, const string &)
{
	fprintf(stderr, "Actor::clone() default method\n");
	return NULL;
}

void Actor::pause()
{
	m_invincible.pause();
}

void Actor::unpause()
{
	m_invincible.unpause();
}

void Actor::reset_update()
{
	m_moved = false;
}

void Actor::update()
{
	/*
	if (m_animate.done())
	{
		bool herp = true;
		if (m_flying)
			herp = true;
		if (m_target != NULL && !m_flying)
			herp = false;
		if (m_jump > 0)
			herp = false;
		if (fabs(m_v[0]) > EPSILON)
		    herp = false;
		if (!herp)
		{
			m_frame = (m_frame + 1) % m_frames;
			m_animate.reset(m_anim_delay);
		}
	}
	*/
}

void Actor::move(double dt)
{
	if (m_moved)
		return;

	if (m_parent != NULL)
		if (!m_parent->has_moved())
			m_parent->move(dt);
	
	*m_shape += dt * m_v;

	m_moved = true;
}

void Actor::draw(Surface *dst) const
{
	vector2d center = m_shape->get_center();
	int x = center[0];
	int y = center[1];

	int w2 = m_surface->get_w() / 2;
	int h2 = m_surface->get_h() / 2;
	if (x + w2 < 0 ||
	    y + h2 < 0 ||
	    x - w2 > dst->get_w() ||
	    y - h2 > dst->get_h())
		return;

	int t = (int)(5 * m_invincible.get_time());
	if (t % 2 == 0 && !m_invincible.done())
		return;

	dst->blit(m_surface, x - w2, y - h2);
	
	/*
	if (m_frames == 0)
		dst->draw_circle(mapRGB(255, 255, 255), x, y, r, 1);
	else
	{
		int frame = m_frame;
		if (m_direction == EAST)
			frame += m_frames;
		Surface *surface = m_surfaces[frame];
		y += r;
		y -= surface->get_h();
		x -= 0.5 * surface->get_w();
		dst->blit(surface, x, y);
	}
	*/
}

void Actor::damage(int pain, int color)
{
	if (!m_invincible.done())
		return;

	// REMOVE THIS
	if (color == -55)
		return;

	DataManager *dm = DataManager::get_instance();
	//ResourceManager *rm = ResourceManager::get_instance();
	
	m_health -= pain;
	m_prev_hit_color = color;
	if (m_team == TEAM_PLAYER)
		m_invincible.reset(dm->get_dbl("invincible"));
	if (m_health <= 0 && !m_dead)
	{
		die();
		m_dead = true;
	}
}

void Actor::die()
{
	// drop an item upon death
	int k = 2;
	if (m_level->subspace())
		k = 4;

	PlayerActor *player = (PlayerActor *)m_level->get_player();

	int p = randint(1, k * m_item_rate);
	if ((p == 1 || m_level->get_boss() == this) && m_items)
	{
		vector2d center = get_center();
		int n = randint(0, 5);
		int t = randint(1, 60);
		if (player != NULL)
			if (player->subspace(0))
				t = randint(1, 50);
		if (m_level->get_boss() == this)
			t = randint(1, 11);
		if (t <= 6)
			n = ITEM_BLANK;
		else if (t <= 7)
			n = ITEM_HEALTH;
		else if (t <= 8)
			n = ITEM_SHIELD;
		else if (t <= 9)
			n = ITEM_BOMB;
		else if (t <= 10)
			n = ITEM_HEALTH_PLUS;
		else if (t <= 11)
			n = ITEM_SHIELD_PLUS;
		new Item(m_level, center, n);
	}

	// spawn an enemy upon death
	if (color_match(m_color, m_prev_hit_color) &&
	    m_death_spawn.size() > 0)
	{
		ActorFactory *af = ActorFactory::get_instance();
		Actor *actor = af->load_actor(m_level, m_death_spawn);
		m_level->add_actor(actor);
	}
	
	m_level->remove_actor(this);
}

void Actor::set_level(Level *level)
{
	m_level = level;
	m_level->add_actor(this);
}
