#ifndef __MANAGER_H
#define __MANAGER_H

#include "actor.h"
#include "background.h"
#include "camera.h"
#include "input.h"
#include "levelselector.h"
#include "menu.h"
#include "player.h"

class Manager : public KeyListener, MouseListener
{
public:
	Manager();
	~Manager();

	void key_pressed(SDLKey key, SDLMod mod);
	void key_released(SDLKey key, SDLMod mod);
	void mouse_event(SDL_MouseButtonEvent event);

	void pause();
	void unpause();

	void update();
	void draw(Surface *dst);
private:
	LevelSelector *m_level_selector;
	PlayerActor *m_player;
	MenuController *m_menu_controller;
	
	bool m_paused;
	Uint32 m_prev_move;
	Uint32 m_pause_ticks;
	Surface *m_pause;
	Surface *m_game_over;

	InputDevice *m_key_device;
	InputDevice *m_joy_device;
	InputDevice *m_input_device;
};

#endif
