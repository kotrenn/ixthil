#ifndef __PLAYER_INTERFACE_H
#define __PLAYER_INTERFACE_H

#include "player.h"

class PlayerInterface : public PlayerActor
{
public:
	PlayerInterface(Level *level, InputDevice *device);
	~PlayerInterface();

	void draw_interface(Surface *dst) const;
private:
	Surface *m_backgrounds[7];
};

#endif
