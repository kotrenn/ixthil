#include "simple.h"

SimpleActor::SimpleActor()
	:Actor("simple")
{
}

SimpleActor::SimpleActor(Level *level, const string &name)
	:Actor(level, name)
{
}

SimpleActor::~SimpleActor()
{
}

Actor *SimpleActor::clone(Level *level, const string &name)
{
	SimpleActor *ret = new SimpleActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = randint(0, level->get_w() - w);
	int y = -200;
	ret->m_shape = new Rect(x, y, w, h);
	
	ret->m_v = vector2d(60 * randint(-2, 2), 30 * randint(2, 5));
	ret->m_max_y = randint(50, 150);
	
	ret->m_health = 1;
	ret->m_color = COLOR_GREEN;
	ret->m_score = 100;

	ret->m_death_spawn = "dark_simple";

	ret->m_shoot_timer.reset(0);
	
	return ret;
}

void SimpleActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void SimpleActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void SimpleActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void SimpleActor::move(double dt)
{
	Actor::move(dt);

	Rect dims = m_level->get_dims();
	m_shape->fix_rect_x(dims, &m_v);

	Rect rect = *(Rect *)m_shape;
	if (rect[1] > m_max_y)
		m_v[0] = m_v[1] = 0;
}

void SimpleActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(0, 150);
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 100.0) / 30);
}
