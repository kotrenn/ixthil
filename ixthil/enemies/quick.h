#ifndef __QUICK_H
#define __QUICK_H

#include "../actor.h"

class QuickActor : public Actor
{
public:
	QuickActor();
	QuickActor(Level *level, const string &name);
	~QuickActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	double get_health() const;

	void update();
	void move(double dt);

	void create_bullet();
private:
	void select_target();
	double m_tx;
	double m_ty;
	Timer m_move_timer;
	Timer m_shoot_timer;
};

#endif
