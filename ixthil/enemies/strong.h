#ifndef __STRONG_H
#define __STRONG_H

#include "../actor.h"

class StrongActor : public Actor
{
public:
	StrongActor();
	StrongActor(Level *level, const string &name);
	~StrongActor();

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
