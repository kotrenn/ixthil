#include "darksimple.h"

DarkSimpleActor::DarkSimpleActor()
	:DarkActor("dark_simple")
{
}

DarkSimpleActor::DarkSimpleActor(Level *level, const string &name)
	:DarkActor(level, name)
{
}

DarkSimpleActor::~DarkSimpleActor()
{
}

Actor *DarkSimpleActor::clone(Level *level, const string &name)
{
	DarkSimpleActor *ret = new DarkSimpleActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = randint(0, level->get_w() - w);
	int y = -200;
	ret->m_shape = new Rect(x, y, w, h);
	
	ret->m_v = vector2d(60 * randint(-2, 2), 30 * randint(2, 5));
	ret->m_max_y = randint(50, 150);
	
	ret->m_health = 2;
	ret->m_color = COLOR_PURPLE;
	ret->m_score = 150;

	ret->m_shoot_timer.reset(0);

	return ret;
}

void DarkSimpleActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void DarkSimpleActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void DarkSimpleActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void DarkSimpleActor::move(double dt)
{
	Actor::move(dt);

	Rect dims = m_level->get_dims();
	m_shape->fix_rect_x(dims, &m_v);

	Rect rect = *(Rect *)m_shape;
	if (rect[1] > m_max_y)
		m_v[0] = m_v[1] = 0;
}

void DarkSimpleActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect r1(center[0] - BULLET_W, center[1], BULLET_W, BULLET_H);
	Rect r2(center[0] + BULLET_W, center[1], BULLET_W, BULLET_H);

	vector2d v(0, 150);
	new Bullet(this, new RectBullet(r1, v), 1, m_color);
	new Bullet(this, new RectBullet(r2, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 70.0) / 30);
}
