#include "darkattractor.h"

DarkAttractorActor::DarkAttractorActor()
	:DarkActor("dark_attractor")
{
}

DarkAttractorActor::DarkAttractorActor(Level *level, const string &name)
	:DarkActor(level, name)
{
}

DarkAttractorActor::~DarkAttractorActor()
{
}

Actor *DarkAttractorActor::clone(Level *level, const string &name)
{
	DarkAttractorActor *ret = new DarkAttractorActor(level, name);

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
	
	ret->m_health = 10;
	ret->m_color = COLOR_CYAN;
	ret->m_score = 700;

	ret->m_shoot_timer.reset(0);
	
	return ret;
}

void DarkAttractorActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void DarkAttractorActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void DarkAttractorActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void DarkAttractorActor::move(double dt)
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

void DarkAttractorActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	int s = randint(-1, 2);
	vector2d v1 = 30 * vector2d(-2 - s, 3 + s);
	vector2d v2 = 30 * vector2d( 2 + s, 3 + s);
	vector2d v3 = 30 * vector2d(0, 5 + s);
	new Bullet(this, new RectBullet(rect, v1), 1, m_color);
	new Bullet(this, new RectBullet(rect, v2), 1, m_color);
	new Bullet(this, new RectBullet(rect, v3), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 100.0) / 30);
}
