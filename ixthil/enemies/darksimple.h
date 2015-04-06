#ifndef __DARK_SIMPLE_H
#define __DARK_SIMPLE_H

#include "dark.h"

class DarkSimpleActor : public DarkActor
{
public:
	DarkSimpleActor();
	DarkSimpleActor(Level *level, const string &name);
	~DarkSimpleActor();

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
