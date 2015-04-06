#ifndef __DARK_ATTRACTOR_H
#define __DARK_ATTRACTOR_H

#include "dark.h"

class DarkAttractorActor : public DarkActor
{
public:
	DarkAttractorActor();
	DarkAttractorActor(Level *level, const string &name);
	~DarkAttractorActor();

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
