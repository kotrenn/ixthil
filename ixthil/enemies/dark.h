#ifndef __DARK_H
#define __DARK_H

#include "../actor.h"

class DarkActor : public Actor
{
public:
	DarkActor(const string &name);
	DarkActor(Level *level, const string &name);
	virtual ~DarkActor();

	void die();
protected:
	int m_energy;
};

#endif
