#include "dark.h"

#include "../player.h"

DarkActor::DarkActor(const string &name)
	:Actor(name)
{
}

DarkActor::DarkActor(Level *level, const string &name)
	:Actor(level, name),
	 m_energy(1)
{
}

DarkActor::~DarkActor()
{
}

void DarkActor::die()
{
	Actor::die();

	fprintf(stderr, "DarkActor::die(%d)\n", m_energy);
	fflush(stderr);

	if (!m_level->subspace() && m_energy > 0)
	{
		PlayerActor *player = (PlayerActor *)m_level->get_player();
		
		int a = 2, b = 5;
		if (player->subspace(2))
		{
			a = 3;
			b = 6;
		}

		int n = max(1, randint(a, b) / m_energy);
		if (player->subspace(2))
			n++;

		vector2d center = get_center();
		for (int i = 0; i < n; ++i)
			new EnergyItem(m_level, center);
	}
}
