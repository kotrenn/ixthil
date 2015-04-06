#include "bullet.h"

Bullet::Bullet(Actor *parent, BulletShape *shape,
               int pain, int color)
	:m_level(parent->get_level()),
	 m_parent(parent),
	 m_shape(shape),
	 m_team(parent->get_team()),
	 m_pain(pain),
	 m_color(color),
	 m_dead(false)
{
	m_level->add_bullet(this);
}

Bullet::Bullet(Level *level, BulletShape *shape,
               int team, int pain, int color)
	:m_level(level),
	 m_parent(NULL),
	 m_shape(shape),
	 m_team(team),
	 m_pain(pain),
	 m_color(color),
	 m_dead(false)
{
	m_level->add_bullet(this);
}

Bullet::~Bullet()
{
	delete m_shape;
}

void Bullet::update()
{
	list<Actor *> actors;
	m_level->get_collisions(&actors, get_shape(), m_team);
	list<Actor *>::iterator a;
	for (a = actors.begin(); a != actors.end(); a++)
	{
		(*a)->damage(m_pain, m_color);
		if (!m_dead)
			die(*a);
	}
	if (actors.size() > 0)
		m_level->remove_bullet(this);
}

void Bullet::move(double dt)
{
	m_shape->move(dt);
}

void Bullet::draw(Surface *dst) const
{
	m_shape->draw(dst, mapColor(m_color));
}

vector2d Bullet::get_center() const
{
	return m_shape->get_shape()->get_center();
}

void Bullet::actor_dying(Actor *actor)
{
	if (m_parent == actor)
		m_parent = NULL;
}

void Bullet::die(Actor *)
{
	m_dead = true;
}

Shape *Bullet::get_shape() const
{
	return m_shape->get_shape();
}



HomingBullet::HomingBullet(Actor *parent, BulletShape *shape,
                           int color)
	:Bullet(parent, shape, 1, color),
	 m_target(NULL),
	 m_speed(300),
	 m_move_x(0),
	 m_move_y(0)
{
}

void HomingBullet::pause()
{
	m_move_x.pause();
	m_move_y.pause();
}

void HomingBullet::unpause()
{
	m_move_x.unpause();
	m_move_y.unpause();
}

void HomingBullet::update()
{
	Bullet::update();
	
	if (m_parent == NULL)
		return;
	
	if (m_move_x.done())
	{
		m_shape->get_v()[0] += randint(-1, 1);
		m_move_x.reset(geometric(1.0 / 60) / 30);
	}
	if (m_move_y.done())
	{
		m_shape->get_v()[1] += randint(-1, 1);
		m_move_y.reset(geometric(1.0 / 60) / 30);
	}

	if (m_target != NULL)
		if (m_target->dead())
			m_target = NULL;
	
	if (m_target == NULL)
		m_target = m_level->random_target(m_team);
}

void HomingBullet::move(double dt)
{
	if (m_target != NULL)
	{
		vector2d center = m_target->get_center();
		vector2d d = center - get_center();
		vector2d v = m_shape->get_v();
		for (int i = 0; i < 2; ++i)
		{
			if (fabs(d[i]) > EPSILON)
				v[i] += 1800 * dt * fabs(d[i]) / d[i];
			if (v[i] >  m_speed) v[i] =  m_speed;
			if (v[i] < -m_speed) v[i] = -m_speed;
		}
		m_shape->set_v(v);
	}
	
	Bullet::move(dt);
}

void HomingBullet::actor_dying(Actor *actor)
{
	if (m_target == actor)
		m_target = NULL;
}



LightningBullet::LightningBullet(Actor *parent, BulletShape *shape,
                                 int color, int radius)
	:Bullet(parent, shape, 1, color),
	 m_radius(radius)
{
}

void LightningBullet::die(Actor *source)
{
	Bullet::die(source);

	Circle circle(get_center(), m_radius);
	list<Actor *> actors;
	m_level->get_collisions(&actors, &circle, m_team);

	list<Actor *>::iterator a;
	for (a = actors.begin(); a != actors.end(); a++)
	{
		new Lightning(m_level, source, *a, m_color, m_team);
		(*a)->damage(1, m_color);
	}
}



Lightning::Lightning(Level *level, Actor *a, Actor *b,
                     int color, int team)
	:Bullet(level,
	        new CircleBullet(Circle(0, 0, 0),
	                         vector2d(0, 0)),
	        team, 1, color),
	 m_actor1(a),
	 m_actor2(b),
	 m_timer(0.333),
	 m_p1(),
	 m_p2()
{
	update_positions();
}

void Lightning::update()
{
	if (m_timer.done() && !m_dead)
		m_level->remove_bullet(this);
}

void Lightning::move(double dt)
{
	Bullet::move(dt);

	update_positions();
}

void Lightning::update_positions()
{
	if (m_actor1 == NULL || m_actor2 == NULL)
		return;
	
	m_p1 = m_actor1->get_center();
	m_p2 = m_actor2->get_center();
}

void Lightning::draw(Surface *dst) const
{
	Uint32 color = mapColor(m_color);
	dst->draw_lightning(color,
	                    m_p1[0], m_p1[1],
	                    m_p2[0], m_p2[1]);
}

void Lightning::actor_dying(Actor *actor)
{
	Bullet::actor_dying(actor);

	if (m_actor1 == actor)
		m_actor1 = NULL;
	if (m_actor2 == actor)
		m_actor2 = NULL;

	if (m_actor1 == NULL || m_actor2 == NULL)
		m_timer.reset(0);
}
