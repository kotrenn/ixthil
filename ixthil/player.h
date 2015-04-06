#ifndef __PLAYER_H
#define __PLAYER_H

#include "actor.h"
#include "input.h"
#include "weapongrid.h"

class WeaponGrid;

class PlayerActor : public Actor, InputListener
{
public:
	PlayerActor(Level *level, InputDevice *device);
	virtual ~PlayerActor();

	void pause();
	void unpause();

	void reset();
	void update();
	void move(double dt);
	void draw(Surface *dst) const;
	virtual void draw_interface(Surface *) const {}

	bool energizing() const { return false; }
	bool subspace(int s) const { return s <= m_subspace; }
	int num_subspace() const { return m_subspace; }
	void give_crystal();
	
	void begin_demo();
	void end_demo();
	bool demo() const { return m_demo; }
protected:
	void select_target();
	void collect_item(Item *item);
	void pull_item(Item *item) const;
	
	WeaponGrid *m_weapon_grid;
	Timer m_shield_timer;
	int m_health_max;
	double m_shield;
	double m_shield_max;
	int m_bombs;
	int m_energy;
	int m_required_energy;
	int m_subspace;
	bool m_demo;
	vector2d m_target;
};

#endif
