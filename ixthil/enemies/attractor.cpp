#include "attractor.h"

AttractorActor::AttractorActor()
	:Actor("attractor")
{
}

AttractorActor::AttractorActor(Level *level, const string &name)
	:Actor(level, name)
{
}

AttractorActor::~AttractorActor()
{
}

Actor *AttractorActor::clone(Level *level, const string &name)
{
	AttractorActor *ret = new AttractorActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = randint(0, level->get_w() - w);
	int y = -50 - randint(100, 200);
	ret->m_shape = new Rect(x, y, w, h);

	vector2d v(0, 0);
	while (v[0] == 0)
		v[0] = 30 * randint(-5, 5);
	ret->m_v = v;
	ret->m_axis = 50 + randint(-10, 10);
	ret->m_a = 900 * randint(1, 2);
	ret->m_max_v = 300;
	
	ret->m_health = 5;
	ret->m_color = COLOR_RED;
	ret->m_score = 500;

	ret->m_death_spawn = "dark_attractor";

	ret->m_shoot_timer.reset(0);
	
	return ret;
}

void AttractorActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void AttractorActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void AttractorActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void AttractorActor::move(double dt)
{
	Actor::move(dt);

	Rect dims = m_level->get_dims();
	m_shape->fix_rect_x(dims, &m_v);

	Rect rect = *(Rect *)m_shape;
	if (rect[1] < m_axis)
		m_v[1] += m_a * dt;
	else if (rect[1] > m_axis)
		m_v[1] -= m_a * dt;

	if (m_v[1] < -m_max_v)
		m_v[1] = -m_max_v;
	if (m_v[1] > m_max_v)
		m_v[1] = m_max_v;
}

void AttractorActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(0, 150);
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 50.0) / 30);
}
