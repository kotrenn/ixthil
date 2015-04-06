#ifndef __WEAPON_GRID_H
#define __WEAPON_GRID_H

#include "bullet.h"
#include "player.h"

class PlayerActor;

class WeaponGrid : public InputListener
{
public:
	WeaponGrid(PlayerActor *player, InputDevice *device);
	~WeaponGrid();

	void pause();
	void unpause();

	void button_pressed(Button button);

	void reset();
	void update();
	void draw(Surface *dst,
	          int ix, int iy, int iw, int ih) const;

	int get_level(int color) const;
	int get_color() const { return m_color; }
	void upgrade(int color);
	void add_blank() { m_blanks++; }
	void cheat();
	void random_color();
	
	void create_bullet();
private:
	PlayerActor *m_player;
	Timer m_shoot_timer;
	Timer m_select_timer;
	int m_color;
	int m_upgrades[6];
	int m_blanks;
	Timer m_blank_timer;

	Surface *m_empty;
	Surface *m_up[6];
	Surface *m_ups[6];
};

#endif
