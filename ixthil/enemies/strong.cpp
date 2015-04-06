#include "strong.h"

StrongActor::StrongActor()
	:Actor("strong")
{
}

StrongActor::StrongActor(Level *level, const string &name)
	:Actor(level, name)
{
}

StrongActor::~StrongActor()
{
}

Actor *StrongActor::clone(Level *level, const string &name)
{
	StrongActor *ret = new StrongActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = randint(0, level->get_w() - w);
	int y = -200;
	ret->m_shape = new Rect(x, y, w, h);
	
	vector2d v = vector2d(0, 30 * randint(3, 8));
	while (fabs(v[0]) < 2)
		v[0] = 30 * randint(-4, 4);
	ret->m_v = v;
	ret->m_max_y = randint(50, 150);
	
	ret->m_health = 3;
	ret->m_color = COLOR_BLUE;
	ret->m_score = 300;

	ret->m_death_spawn = "back_and_forth";

	ret->m_shoot_timer.reset(0);
	
	return ret;
}

void StrongActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void StrongActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void StrongActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void StrongActor::move(double dt)
{
	Actor::move(dt);

	Rect dims = m_level->get_dims();
	m_shape->fix_rect_x(dims, &m_v);

	Rect rect = *(Rect *)m_shape;
	if (rect[1] > m_max_y)
		m_v[1] = 0;
}

void StrongActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(0, 150);
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 70.0) / 30);
}
