#include "backandforth.h"

BackAndForthActor::BackAndForthActor()
	:DarkActor("back_and_forth")
{
}

BackAndForthActor::BackAndForthActor(Level *level, const string &name)
	:DarkActor(level, name)
{
}

BackAndForthActor::~BackAndForthActor()
{
}

Actor *BackAndForthActor::clone(Level *level, const string &name)
{
	BackAndForthActor *ret = new BackAndForthActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = 0;
	while (x >= -0.5 * level->get_w() &&
	       x <=  1.5 * level->get_w())
		x = randint(-1 * level->get_w(), 2 * level->get_w());
	int y = randint(50, 150);
	ret->m_shape = new Rect(x, y, w, h);
	
	vector2d v = vector2d(30 * randint(2, 6), 0);
	if (x > 0)
		v[0] *= -1;
	ret->m_v = v;
	
	ret->m_health = 6;
	ret->m_color = COLOR_YELLOW;
	ret->m_score = 450;

	ret->m_shoot_timer.reset(0);
	ret->m_shoot_delay = randdbl(0.5, 0.833);
	
	return ret;
}

void BackAndForthActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void BackAndForthActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void BackAndForthActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void BackAndForthActor::move(double dt)
{
	Actor::move(dt);

	Rect dims = m_level->get_dims();
	m_shape->fix_rect_x(dims, &m_v);
}

void BackAndForthActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect r1(center[0] - BULLET_W, center[1], BULLET_W, BULLET_H);
	Rect r2(center[0] + BULLET_W, center[1], BULLET_W, BULLET_H);

	vector2d v(0, 150);
	new Bullet(this, new RectBullet(r1, v), 1, m_color);
	new Bullet(this, new RectBullet(r2, v), 1, m_color);

	m_shoot_timer.reset(m_shoot_delay);
}
