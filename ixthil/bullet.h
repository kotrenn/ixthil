#ifndef __BULLET_H
#define __BULLET_H

#define BULLET_W 6
#define BULLET_H 6

#include "actor.h"
#include "bulletshape.h"
#include "camera.h"
#include "color.h"
#include "level.h"
#include "timer.h"

class Actor;
class BulletShape;
class Level;

class Bullet
{
public:
	Bullet(Actor *parent, BulletShape *shape, int pain, int color);
	Bullet(Level *level, BulletShape *shape,
	       int team, int pain, int color);
	virtual ~Bullet();

	virtual void pause() {}
	virtual void unpause() {}

	virtual void update();
	virtual void move(double dt);
	virtual void draw(Surface *dst) const;

	void set_level(Level *level) { m_level = level; }
	int get_team() const { return m_team; }
	int get_color() const { return m_color; }
	vector2d get_center() const;
	virtual void actor_dying(Actor *actor);
	bool dead() const { return m_dead; }
	virtual void die(Actor *source);

	Shape *get_shape() const;
protected:
	Level *m_level;
	Actor *m_parent;
	BulletShape *m_shape;
	int m_team;
	int m_pain;
	int m_color;
	bool m_dead;
};

class HomingBullet : public Bullet
{
public:
	HomingBullet(Actor *parent, BulletShape *shape, int color);

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void actor_dying(Actor *actor);
private:
	Actor *m_target;
	double m_speed;
	Timer m_move_x;
	Timer m_move_y;
};

class LightningBullet : public Bullet
{
public:
	LightningBullet(Actor *parent, BulletShape *shape,
	                int color, int radius);

	void die(Actor *source);
private:
	int m_radius;
};

class Lightning : public Bullet
{
public:
	Lightning(Level *level, Actor *a, Actor *b,
	          int color, int team);

	void update();
	void move(double dt);
	void draw(Surface *dst) const;

	void actor_dying(Actor *actor);
private:
	void update_positions();
	
	Actor *m_actor1;
	Actor *m_actor2;
	Timer m_timer;
	vector2d m_p1;
	vector2d m_p2;
};

#endif
