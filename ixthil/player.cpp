#include "player.h"
#include "datamanager.h"
#include "resourcemanager.h"

PlayerActor::PlayerActor(Level *level, InputDevice *device)
	:Actor(level, "player"),
	 InputListener(device),
	 m_weapon_grid(NULL),
	 m_shield_timer(0),
	 m_health_max(0),
	 m_shield(0),
	 m_shield_max(0),
	 m_bombs(0),
	 m_energy(0),
	 m_required_energy(1),
	 m_subspace(0),
	 m_demo(false),
	 m_target()
{
	m_shape = new Rect(level->get_w() / 2,
	                   level->get_h() - 100,
	                   m_surface->get_w(),
	                   m_surface->get_h());

	m_team = TEAM_PLAYER;
	m_color = COLOR_RED;
	m_weapon_grid = new WeaponGrid(this, device);

	m_items = false;

	reset();
}

PlayerActor::~PlayerActor()
{
	delete m_weapon_grid;
}

void PlayerActor::pause()
{
	Actor::pause();

	m_weapon_grid->pause();
	m_shield_timer.pause();
}

void PlayerActor::unpause()
{
	Actor::unpause();

	m_weapon_grid->unpause();
	m_shield_timer.unpause();
}

void PlayerActor::reset()
{
	DataManager *dm = DataManager::get_instance();

	m_health_max = dm->get_int("player_health");
	m_health_max = 5000;
	m_health = m_health_max;
	m_shield_max = dm->get_dbl("player_shield");
	m_shield = m_shield_max;
	m_bombs = 0;
	m_energy = 0;
	m_required_energy = 1;
	m_subspace = 0;

	m_color = COLOR_RED;
	m_weapon_grid->reset();
	
	if (m_demo)
	{
		m_weapon_grid->cheat();
		m_weapon_grid->random_color();
		m_energy = 1000000;
		select_target();
	}
}

void PlayerActor::select_target()
{
	Rect *dims = (Rect *)m_shape;
	m_target[0] = randint(0.2 * m_level->get_w(),
	                      0.8 * m_level->get_w());
	m_target[1] = randint(0.7 * m_level->get_h(),
	                      m_level->get_h() - dims->get_h());
}

void PlayerActor::update()
{
	Actor::update();

	m_v[0] = 0;
	m_v[1] = 0;

	DataManager *dm = DataManager::get_instance();

	int speed = dm->get_int("player_speed");

	if (m_demo)
	{
		vector2d center = get_center();
		vector2d disp = m_target - center;
		for (int i = 0; i < 2; ++i)
		{
			m_v[i] = speed * signum(disp[i]);
			if (fabs(disp[i]) < 5)
				m_v[i] = 0;
		}
		if (m_v * m_v < 20)
			select_target();
	}
	else
	{
		if (m_input->pressed(BUTTON_UP))
			m_v[1] -= speed;
		if (m_input->pressed(BUTTON_DOWN))
			m_v[1] += speed;
		if (m_input->pressed(BUTTON_LEFT))
			m_v[0] -= speed;
		if (m_input->pressed(BUTTON_RIGHT))
			m_v[0] += speed;
	}

	m_weapon_grid->update();

	// heal
	if (m_shield_timer.done() && m_shield < m_shield_max)
	{
		m_shield += 0.33; /* switch to 0.5 if have yellow crystal */
		m_shield_timer.reset(dm->get_dbl("shield_delay"));
	}

	// collect items
	Circle circle(get_center(), 200);
	list<Item *> items;
	m_level->get_collisions(&items, &circle);
	list<Item *>::iterator it;
	for (it = items.begin(); it != items.end(); it++)
	{
		Item *item = *it;

		if (item->get_shape()->collides(get_shape()))
			collect_item(item);
		else
			pull_item(item);
	}
}

void PlayerActor::move(double dt)
{
	Actor::move(dt);

	m_shape->fix_rect(m_level->get_dims());
}

void PlayerActor::draw(Surface *dst) const
{
	Actor::draw(dst);

	Uint32 white = mapRGB(255, 255, 255);

	if (!m_invincible.done())
	{
		vector2d center = get_center();
		dst->draw_lightning_circle(white, center[0], center[1], 100);
	}
}

void PlayerActor::give_crystal()
{
	m_subspace = min(6, m_subspace + 1);
}

void PlayerActor::collect_item(Item *item)
{
	m_level->remove_item(item);
	int type = item->get_type();

	if (type >= 0)
		m_weapon_grid->upgrade(type);
	else if (type == ITEM_BLANK)
		m_weapon_grid->add_blank();
	else if (type == ITEM_BOMB)
		m_bombs += 1;
	else if (type == ITEM_HEALTH)
		m_health += 1;
	else if (type == ITEM_SHIELD)
		m_shield += 100;
	else if (type == ITEM_HEALTH_PLUS)
	{
		m_health += 1;
		m_health_max += 1;
	}
	else if (type == ITEM_SHIELD_PLUS)
	{
		m_shield += 1;
		m_shield_max += 1;
	}
	else if (type == ITEM_ENERGY)
		m_energy += 1;
	else if (type == ITEM_PORTAL &&
	         m_energy >= m_required_energy)
	{
		//m_level->enter_subspace((PortalItem *)item);
		m_level->enter_subspace();
		m_energy -= m_required_energy;
	}

	if (m_health > m_health_max)
		m_health = m_health_max;
	if (m_shield > m_shield_max)
		m_shield = m_shield_max;
}

void PlayerActor::pull_item(Item *item) const
{
	bool shooting = m_input->pressed(BUTTON_SHOOT);
	if (m_demo)
		shooting = false;
	if (!item->get_pulled() && shooting)
		return;

	if (item->get_type() == ITEM_PORTAL)
		return;
	
	item->set_pulled(true);
	
	double theta = 45;
	if (m_level->get_level() % 2 == 0)
		theta = -45;
	double c = cos(theta * M_PI / 180.0);
	double s = sin(theta * M_PI / 180.0);

	vector2d r = get_center() - item->get_shape()->get_center();
	//vector2d a(c * r[0] - s * r[1], c * r[0] + s * r[1]);
	vector2d a(c * r[0] - s * r[1], s * r[0] + c * r[1]);
	double alpha = 0.25;
	if (r * r < 25 * 25)
		alpha = 0.6;
	
	item->set_v(30 * alpha * a);
}

void PlayerActor::begin_demo()
{
	m_demo = true;
	reset();
}

void PlayerActor::end_demo()
{
	m_demo = false;
	reset();
}
