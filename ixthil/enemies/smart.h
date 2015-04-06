#ifndef __SMART_H
#define __SMART_H

#include "../actor.h"

class SmartActor : public Actor
{
public:
	SmartActor();
	SmartActor(Level *level, const string &name);
	~SmartActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void create_bullet();
private:
	Timer m_shoot_timer;
	double m_speed;
	double m_max_speed;
	int m_max_y;
};

#endif
