#ifndef __ATTRACTOR_H
#define __ATTRACTOR_H

#include "../actor.h"

class AttractorActor : public Actor
{
public:
	AttractorActor();
	AttractorActor(Level *level, const string &name);
	~AttractorActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void create_bullet();
private:
	int m_axis;
	int m_a;
	int m_max_v;
	Timer m_shoot_timer;
};

#endif
