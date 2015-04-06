#ifndef __GROUP_H
#define __GROUP_H

#include "../actor.h"

class GroupCreator : public Actor
{
public:
	GroupCreator();
	GroupCreator(Level *level, const string &name);
	~GroupCreator();

	Actor *clone(Level *level, const string &name);

	void update();
private:
};

class GroupActor : public Actor
{
public:
	GroupActor();
	//GroupActor(Level *level, const string &name);
	GroupActor(Level *level, vector2d center,
	           vector2d v, double r, double t,
	           double dt, int *size);
	~GroupActor();

	//Actor *clone(Level *level, const string &name);
	void die();

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void create_bullet();
private:
	vector2d get_pos() const;
	
	int *m_size;
	Timer m_shoot_timer;
	vector2d m_center;
	double m_r;
	double m_t;
	double m_dt;
	bool m_in_level;
};

#endif
