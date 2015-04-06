#ifndef __ACTOR_H
#define __ACTOR_H

#include "level.h"
#include "surface.h"
#include "timer.h"

#define TEAM_ENEMY 0
#define TEAM_PLAYER 1

class Level;

class Actor
{
public:
	Actor(const string &name);
	Actor(Level *level, const string &name);
	virtual ~Actor();

	virtual Actor *clone(Level *level, const string &name);

	string get_name() const { return m_name; }

	virtual void pause();
	virtual void unpause();

	virtual double get_health() const { return 0.0; }

	virtual void reset_update();
	virtual void update();
	virtual void move(double dt);
	virtual void draw(Surface *dst) const;

	bool has_moved() const { return m_moved; }
	void set_parent(Actor *parent) { m_parent = parent; }
	Actor *get_parent() const { return m_parent; }

	void damage(int pain, int color);
	bool dead() const { return m_dead; }
	virtual void die();

	void set_level(Level *level);
	virtual void actor_dying(Actor *) {}

	Level *get_level() const { return m_level; }
	int get_team() const { return m_team; }
	Shape *get_shape() const { return m_shape; }
	vector2d get_center() const { return m_shape->get_center(); }
	vector2d get_v() const { return m_v; }
	int get_color() const { return m_color; }
	void set_color(int color) { m_color = color; }

	void set_v(const vector2d &v) { m_v = v; }
protected:
	Level *m_level;
	string m_name;
	Shape *m_shape;
	vector2d m_v;
	int m_team;
	int m_health;
	bool m_dead;
	Timer m_invincible;
	int m_color;
	int m_prev_hit_color;
	string m_death_spawn;
	
	int m_score;
	bool m_items;
	int m_item_rate;
	
	Surface *m_surface;

	bool m_moved;
	Actor *m_parent;

	/* animation stuff */
	/*
	int m_frames;
	vector<Surface *> m_surfaces;
	int m_frame;
	Timer m_animate;
	double m_anim_delay;
	*/
};

#endif
