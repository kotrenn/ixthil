#include "group.h"

#include "../resourcemanager.h"

GroupCreator::GroupCreator()
	:Actor("group_creator")
{
}

GroupCreator::GroupCreator(Level *level, const string &name)
	:Actor(level, name)
{
}

GroupCreator::~GroupCreator()
{
}

Actor *GroupCreator::clone(Level *level, const string &name)
{
	GroupCreator *ret = new GroupCreator(level, name);

	ret->m_shape = new Rect(-5000000, -5000000, 0, 0);
	ret->m_v = vector2d(0, 0);
	
	ret->m_health = 1;
	ret->m_color = COLOR_BLANK;
	ret->m_score = 0;

	int num_ships = 3;
	int *size = new int;
	*size = num_ships;

	double dt = 30 * randint(3, 8);
	if (randint(0, 1) == 0)
		dt *= -1;

	ResourceManager *rm = ResourceManager::get_instance();
	Surface *image = rm->get_image("actors/group.png");

	int lim = level->get_w() - 2 * image->get_w();
	vector2d center(randint(0, lim), -150);
	vector2d v(0, 0);
	while (v[0] == 0 || v[1] == 0)
		v = vector2d(randint(-5, 5),
		             randint(3, 5));
	v *= 30;
	double r = image->get_w();
	for (int i = 0; i < num_ships; ++i)
	{
		double t = i * (360 / num_ships);
		GroupActor *g = new GroupActor(level, center, v, r, t, dt, size);
		level->add_actor(g);
	}

	return ret;
}

void GroupCreator::update()
{
	m_level->remove_actor(this);
}



GroupActor::GroupActor()
	:Actor("group")
{
}

GroupActor::GroupActor(Level *level,
                       vector2d center,
                       vector2d v,
                       double r,
                       double t,
                       double dt,
                       int *size)
	:Actor(level, "group"),
	 m_size(size),
	 m_shoot_timer(0),
	 m_center(center),
	 m_r(r),
	 m_t(t),
	 m_dt(dt),
	 m_in_level(false)
{
	vector2d pos = get_pos();
	m_shape = new Circle(pos[0], pos[1], m_surface->get_w() / 2);
	m_v = v;

	m_health = 10;
	m_color = COLOR_GREEN;
	m_score = 450;

	m_shoot_timer.reset(0);
}

GroupActor::~GroupActor()
{
}

void GroupActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void GroupActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void GroupActor::die()
{
	if (m_dead)
		return;

	if (*m_size == 1)
	{
		delete m_size;
		//m_death_spawn = "octa_group";
		m_death_spawn = "dark_attractor";
	}
	else
		*m_size -= 1;
	
	Actor::die();
}

void GroupActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void GroupActor::move(double dt)
{
	Actor::move(dt);

	m_t += dt * m_dt;
	while (m_t < 0)
		m_t += 360;
	while (m_t > 360)
		m_t -= 360;

	m_center += dt * m_v;

	Rect dims = m_level->get_dims();
	dims[3] -= 200;

	Rect rect(m_center[0], m_center[1], 1, 1);
	if (dims.collides(&rect))
		m_in_level = true;
	
	rect.fix_rect_x(dims, &m_v);
	if (m_in_level)
		rect.fix_rect_y(dims, &m_v);

	m_center[0] = rect[0];
	m_center[1] = rect[1];

	vector2d pos = get_pos();
	Rect *shape = (Rect *)m_shape;
	shape->set_x(pos[0]);
	shape->set_y(pos[1]);
}

vector2d GroupActor::get_pos() const
{
	double theta = m_t * M_PI / 180.0;
	vector2d ret = m_center;
	ret += m_r * vector2d(cos(theta), sin(theta));
	return ret;
}

void GroupActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(0, 30 * randint(5, 7));
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 150.0) / 30);
}
