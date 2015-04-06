#include "smart.h"

SmartActor::SmartActor()
	:Actor("smart")
{
}

SmartActor::SmartActor(Level *level, const string &name)
	:Actor(level, name)
{
}

SmartActor::~SmartActor()
{
}

Actor *SmartActor::clone(Level *level, const string &name)
{
	SmartActor *ret = new SmartActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = randint(0, level->get_w());
	int y = -1 * randint(150, 250);
	ret->m_shape = new Rect(x, y, w, h);
	
	ret->m_v = vector2d(30 * randint(-5, 5),
	                    30 * randint(2, 6));
	ret->m_speed = 30 * 0.5 * randint(2, 8);
	ret->m_max_speed = 30 * randint(10, 22);
	ret->m_max_y = randint(20, 100);
	
	ret->m_health = 10;
	ret->m_color = COLOR_YELLOW;
	ret->m_score = 750;

	//ret->m_death_spawn = "dark_smart";

	ret->m_shoot_timer.reset(0);
	
	return ret;
}

void SmartActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void SmartActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void SmartActor::update()
{
	Actor::update();

	if (m_shoot_timer.done())
		create_bullet();
}

void SmartActor::move(double dt)
{
	Actor::move(dt);

	/* chase player */
	Actor *player = m_level->get_player();
	vector2d target = player->get_center();

	if (m_v[0] < -m_max_speed)
		m_v[0] = -m_max_speed;
	if (m_v[0] > m_max_speed)
		m_v[0] = m_max_speed;

	Rect rect = *(Rect *)m_shape;
	if (rect[0] < target[0])
		m_v[0] += 30 * dt * m_speed;
	else if (rect[0] > target[0])
		m_v[0] -= 30 * dt * m_speed;

	/* level bounds */
	Rect dims = m_level->get_dims();
	m_shape->fix_rect_x(dims, &m_v);

	if (rect[1] > m_max_y)
		m_v[1] = 0;
}

void SmartActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(0, 30 * randint(4, 8));
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 50.0) / 30);
}
