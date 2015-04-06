#ifndef __SIMPLE_H
#define __SIMPLE_H

#include "../actor.h"

class SimpleActor : public Actor
{
public:
	SimpleActor();
	SimpleActor(Level *level, const string &name);
	~SimpleActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void create_bullet();
private:
	int m_max_y;
	Timer m_shoot_timer;
};

#endif
