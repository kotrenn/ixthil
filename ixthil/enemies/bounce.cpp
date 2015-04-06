#include "bounce.h"

BounceActor::BounceActor()
	:Actor("bounce")
{
}

BounceActor::BounceActor(Level *level, const string &name)
	:Actor(level, name)
{
}

BounceActor::~BounceActor()
{
}

Actor *BounceActor::clone(Level *level, const string &name)
{
	BounceActor *ret = new BounceActor(level, name);

	int x = randint(0, level->get_w());
	int y = -1 * randint(200, 400);
	int r = ret->m_surface->get_w() / 2;
	ret->m_shape = new Circle(x, y, r);
	
	vector2d v(0, 0);
	while (v[0] == 0 || v[1] == 0)
		v = vector2d(randint(-4, 4), randint(0, 4));
	ret->m_v = 75 * v;
	
	ret->m_health = 20;
	ret->m_color = COLOR_CYAN;
	ret->m_score = 1000;

	ret->m_shoot_timer.reset(0);
	
	ret->m_in_level = false;
	
	return ret;
}

void BounceActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void BounceActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void BounceActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void BounceActor::move(double dt)
{
	Actor::move(dt);

	Rect dims = m_level->get_dims();
	dims[3] -= 200;

	if (dims.collides(m_shape))
		m_in_level = true;
	
	m_shape->fix_rect_x(dims, &m_v);
	if (m_in_level)
		m_shape->fix_rect_y(dims, &m_v);
}

void BounceActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(0, 30 * randint(8, 12));
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 60.0) / 30);
}
