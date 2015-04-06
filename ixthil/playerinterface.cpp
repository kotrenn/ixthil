#include "playerinterface.h"
#include "resourcemanager.h"

PlayerInterface::PlayerInterface(Level *level, InputDevice *device)
	:PlayerActor(level, device)
{
	ResourceManager *rm = ResourceManager::get_instance();
	
	char filename[81];
	for (int i = 0; i < 7; ++i)
	{
		snprintf(filename, 80, "interface/interface%d.png", i);
		m_backgrounds[i] = rm->get_image(filename);
	}
}

PlayerInterface::~PlayerInterface()
{
}

void PlayerInterface::draw_interface(Surface *dst) const
{
	PlayerActor::draw_interface(dst);

	int ix = m_level->get_w();
	int iy = 0;
	int iw = dst->get_w() - ix;
	int ih = dst->get_h();

	/* draw background */
	int level = m_level->get_level();
	int n = (level - 1) / 4;
	if (level == 25)
		n = 5;
	if (level > 25)
		n = 6;
	n = max(0, n);
	dst->blit(m_backgrounds[n], ix, iy);

	/* draw weapon grid */
	m_weapon_grid->draw(dst, ix, iy, iw, ih);
}
