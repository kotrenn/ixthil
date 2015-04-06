#include "weapongrid.h"
#include "datamanager.h"
#include "resourcemanager.h"

WeaponGrid::WeaponGrid(PlayerActor *player, InputDevice *device)
	:InputListener(device),
	 m_player(player),
	 m_shoot_timer(0),
	 m_select_timer(0),
	 m_color(0),
	 m_upgrades(),
	 m_blanks(0),
	 m_blank_timer(0)
{
	ResourceManager *rm = ResourceManager::get_instance();
	
	m_color = m_player->get_color();

	m_empty = rm->get_image("interface/up_empty.png");
	for (int i = 0; i < 6; ++i)
	{
		char str_up[81];
		char str_ups[81];
		snprintf(str_up,  80, "interface/up%d.png",  i);
		snprintf(str_ups, 80, "interface/ups%d.png", i);
		m_up[i]  = rm->get_image(str_up);
		m_ups[i] = rm->get_image(str_ups);
	}

	reset();
}

WeaponGrid::~WeaponGrid()
{
}

void WeaponGrid::pause()
{
	m_shoot_timer.pause();
	m_select_timer.pause();
	m_blank_timer.pause();
}

void WeaponGrid::unpause()
{
	m_shoot_timer.unpause();
	m_select_timer.unpause();
	m_blank_timer.unpause();
}

void WeaponGrid::button_pressed(Button button)
{
	if (button == BUTTON_BLANK)
	{
		if (m_upgrades[m_color] < 8)
			m_upgrades[m_color]++;
	}
	else if (button == BUTTON_ENERGY)
	{
	}
}

void WeaponGrid::reset()
{
	m_select_timer.reset(0);
	if (m_player->demo())
		m_select_timer.reset(geometric(1.0 / 600.0) / 30);
	
	m_color = m_player->get_color();

	for (int i = 0; i < 6; ++i)
		m_upgrades[i] = 0;
	m_upgrades[0] = 2;

	for (int i = 1; i < 6; ++i)
		m_upgrades[i] = i;
}

void WeaponGrid::update()
{
	DataManager *dm = DataManager::get_instance();
	
	// shoot
	if (m_input->pressed(BUTTON_SHOOT) || m_player->demo())
		if (m_shoot_timer.done())
			create_bullet();

	// select
	bool selection_made = false;
	if (m_input->pressed(BUTTON_PREV) ||
	    m_input->pressed(BUTTON_NEXT))
		selection_made = true;
	if (m_player->demo())
		selection_made = true;
	if (selection_made)
	{
		if (m_select_timer.done())
		{
			double delay = dm->get_dbl("player_select_delay");
			int dir = m_input->pressed(BUTTON_PREV)? -1 : 1;
			if (m_player->demo())
			{
				dir = randint(0, 1) * 2 - 1;
				delay = geometric(1.0 / 600.0) / 30;
			}
			m_color = (m_color + dir + 6) % 6;
			m_player->set_color(m_color);
			m_select_timer.reset(delay);
		}
	}
}

void WeaponGrid::draw(Surface *dst, int ix, int iy, int iw, int ih) const
{
	double uw = 4 * (4 + m_empty->get_w());
	double uh = 6 * (4 + m_empty->get_h());
	double ux = ix + 0.5 * (iw - uw) + 2;
	double uy = iy + (ih - uh) - 0.5 * (iw - uw);
	Rect bounds(0, 0, 0, 0);

	for (int i = 0; i < 6; ++i)
	{
		Surface *img = m_up[i];
		bounds[2] = img->get_w();
		bounds[3] = img->get_h();
		bounds[1] = uy + i * (bounds[3] + 4);
		for (int j = 0; j < 4; ++j)
		{
			bounds[0] = j * (bounds[2] + 4);
			bounds[0] += ux;
			if (j < m_upgrades[i] / 2)
				dst->blit(img, bounds[0], bounds[1]);
			else
				dst->blit(m_empty, bounds[0], bounds[1]);
		}

		/* draw the half level */
		if (m_upgrades[i] % 2 == 1)
		{
			img = m_ups[i];
			if (m_upgrades[i] > 2)
			{
				int u = m_upgrades[i] / 2;
				bounds[0] = (u - 1) * (bounds[2] + 4);
				bounds[0] += 4 + bounds[2];
			}
			else
				bounds[0] = 0;
			bounds[0] += 0.5 * bounds[2] - 0.5 * img->get_w();
			bounds[1] += 0.5 * bounds[3] - 0.5 * img->get_h();
			bounds[0] += ux;
			dst->blit(img, bounds[0], bounds[1]);
		}
	}

	/* draw the highlight around current weapon */
	Uint32 white = mapRGB(255, 255, 255);
	bounds[0] = ux - 2;
	bounds[1] = uy + m_color * (m_empty->get_h() + 4) - 2;
	bounds[2] = 16 + 4 * m_empty->get_w();
	bounds[3] = 4 + m_empty->get_h();
	dst->draw_rect(white, bounds, 2);
}

int WeaponGrid::get_level(int color) const
{
	int ret = m_upgrades[color] / 2;
	if (m_player->subspace(COLOR_BLUE))
		ret++;
	if (m_player->energizing())
		ret++;
	return ret;
}

void WeaponGrid::upgrade(int color)
{
	m_upgrades[color] = min(8, m_upgrades[color] + 1);
}

void WeaponGrid::cheat()
{
	for (int i = 0; i < 6; ++i)
		m_upgrades[i] = 8;
}

void WeaponGrid::random_color()
{
	m_color = randint(0, 5);
}

void WeaponGrid::create_bullet()
{
	vector2d center = m_player->get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	int level = get_level(m_color);

	if (level == 0)
		return;

	if (m_color == COLOR_RED)
	{
		vector2d v(0, -300);
		if (level < 2)
		{
			new Bullet(m_player, new RectBullet(rect, v), 1, m_color);
			m_shoot_timer.reset(0.2);
		}
		else
		{
			int limit = 2 * (level - 1);
			for (int i = 0; i < limit; ++i)
			{
				Rect r1(rect);
				Rect r2(rect);
				r1[1] -= i * rect[3];
				r2[1] -= i * rect[3];
				r1[0] -= 0.5 * rect[2];
				r2[0] += 0.5 * rect[2];
				new Bullet(m_player, new RectBullet(r1, v), 1, m_color);
				new Bullet(m_player, new RectBullet(r2, v), 1, m_color);
			}
			m_shoot_timer.reset(0.4);
		}
	}
	else if (m_color == COLOR_YELLOW)
	{
		int radius = 50 + level * 25;
		vector2d v(0, -300);
		new LightningBullet(m_player, new RectBullet(rect, v),
		                    m_color, radius);
		new Bullet(m_player, new RectBullet(rect, v), level, m_color);
		m_shoot_timer.reset((6 + level) / 30.0);
	}
	else if (m_color == COLOR_GREEN)
	{
		int n = max(1, 2 * (level - 1));
		vector2d v(0, -300);
		for (int i = 0; i < n; ++i)
		{
			new HomingBullet(m_player, new RectBullet(rect, v), m_color);
		}
		m_shoot_timer.reset(0.4);
	}
	else if (m_color == COLOR_CYAN)
	{
		for (int i = 0; i < level; ++i)
		{
			double vx = 3.0 * randint(-20, 20);
			vector2d v(vx, -300 + 0.5 * fabs(vx));
			new Bullet(m_player, new RectBullet(rect, v), 1, m_color);
		}
		m_shoot_timer.reset(4.0 / 30.0);
	}
	else if (m_color == COLOR_BLUE)
	{
		for (int i = 1; i < level + 1; ++i)
		{
			Rect r1(rect);
			Rect r2(rect);
			r1[1] -= i * rect[3];
			r2[1] -= i * rect[3];
			r1[0] -= 0.5 * rect[2];
			r2[0] += 0.5 * rect[2];
			vector2d v1(-15 * i, -180);
			vector2d v2( 15 * i, -180);
			new Bullet(m_player, new RectBullet(r1, v1), 1, m_color);
			new Bullet(m_player, new RectBullet(r2, v2), 1, m_color);
		}
		vector2d v(0, -180);
		Rect r3(rect);
		r3[1] -= rect[3];
		new Bullet(m_player, new RectBullet(r3, v), 1, m_color);
		m_shoot_timer.reset(0.4);
	}
	else if (m_color == COLOR_PURPLE)
	{
		m_shoot_timer.reset((6 / level + 1) / 30.0);
		vector2d v(0, -450);
		Rect r1(rect);
		r1[1] -= rect[3];
		new Bullet(m_player, new RectBullet(rect, v), 1, m_color);
		new Bullet(m_player, new RectBullet(r1, v), 1, m_color);
		for (int i = 5; i <= level; ++i)
		{
			Rect r2(rect);
			r2[1] -= (i - 3) * rect[3];
			new Bullet(m_player, new RectBullet(r2, v), 1, m_color);
		}
	}

	/*
	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("player.wav");
	*/
}
