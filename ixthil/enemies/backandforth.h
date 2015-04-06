#ifndef __BACK_AND_FORTH_H
#define __BAND_AND_FORTH_H

#include "dark.h"

class BackAndForthActor : public DarkActor
{
public:
	BackAndForthActor();
	BackAndForthActor(Level *level, const string &name);
	~BackAndForthActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void create_bullet();
private:
	Timer m_shoot_timer;
	double m_shoot_delay;
};

#endif
