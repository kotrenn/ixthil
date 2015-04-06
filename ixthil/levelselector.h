#ifndef __LEVEL_SELECTOR_H
#define __LEVEL_SELECTOR_H

#include "background.h"
#include "camera.h"
#include "level.h"
#include "player.h"

class LevelSelector
{
public:
	LevelSelector();
	~LevelSelector();

	void pause();
	void unpause();

	void set_player(PlayerActor *player);
	PlayerActor *get_player() const { return m_player; }

	void update();
	void move(double dt);
	void draw(Surface *dst) const;

	Level *get_level() const { return m_level; }

	void switch_level(const string &next);
private:
	void level_done();
	
	Level *m_level;
	Background *m_background;
	Camera *m_camera;
	PlayerActor *m_player;
};

#endif
