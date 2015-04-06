#include "quick.h"

QuickActor::QuickActor()
	:Actor("quick")
{
}

QuickActor::QuickActor(Level *level, const string &name)
	:Actor(level, name)
{
}

QuickActor::~QuickActor()
{
}

Actor *QuickActor::clone(Level *level, const string &name)
{
	QuickActor *ret = new QuickActor(level, name);

	int w = ret->m_surface->get_w();
	int h = ret->m_surface->get_h();
	int x = 0.5 * (level->get_w() - w);
	int y = -200;
	ret->m_shape = new Rect(x, y, w, h);
	
	ret->m_v = vector2d(0, 0);
	ret->m_tx = 0;
	ret->m_ty = 0;

	ret->m_health = 40;
	ret->m_color = COLOR_RED;
	ret->m_score = 10000;

	ret->m_move_timer.reset(0);
	ret->m_shoot_timer.reset(0);

	ret->select_target();

	level->set_boss(ret);
	
	return ret;
}

void QuickActor::pause()
{
	Actor::pause();

	m_move_timer.pause();
	m_shoot_timer.pause();
}

void QuickActor::unpause()
{
	Actor::unpause();

	m_move_timer.unpause();
	m_shoot_timer.unpause();
}

void QuickActor::select_target()
{
	m_tx = randint(0.2 * m_level->get_w(), 0.8 * m_level->get_w());
	m_ty = randint(0.2 * m_level->get_h(), 0.6 * m_level->get_h());
}

double QuickActor::get_health() const
{
	return m_health / 40.0;
}

void QuickActor::update()
{
	Actor::update();

	vector2d center = m_shape->get_center();
	vector2d t(m_tx, m_ty);
	vector2d d = t - center;

	if (d * d < 5 * 5)
	{
		m_v = vector2d(0, 0);
		if (m_move_timer.done())
		{
			m_move_timer.reset(randint(30, 90) / 30.0);
			select_target();
		}
	}
	else
	{
		for (int i = 0; i < 2; ++i)
			if (fabs(d[i]) > EPSILON)
				m_v[i] = 3.0 * d[i];
	}
	
	if (m_shoot_timer.done())
		create_bullet();
}

void QuickActor::move(double dt)
{
	Actor::move(dt);
}

void QuickActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	for (int i = 0; i < 3; ++i)
	{
		vector2d v(60 * (i - 1), 150);
		if (i == 1)
			v[1] = 240;
		new Bullet(this, new RectBullet(rect, v), 1, m_color);
	}

	m_shoot_timer.reset(geometric(1.0 / 25.0) / 30);
}
