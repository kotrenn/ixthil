#ifndef __BOUNCE_H
#define __BOUNCE_H

#include "../actor.h"

class BounceActor : public Actor
{
public:
	BounceActor();
	BounceActor(Level *level, const string &name);
	~BounceActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	void update();
	void move(double dt);

	void create_bullet();
private:
	bool m_in_level;
	Timer m_shoot_timer;
};

#endif
