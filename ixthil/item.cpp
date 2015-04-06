#include "item.h"
#include "resourcemanager.h"

Item::Item(Level *level, const vector2d &center,
           int type)
	:m_dims(),
	 m_type(type),
	 m_surface(NULL),
	 m_pulled(false),
	 m_speed(120),
	 m_v(0, m_speed)
{
	ResourceManager *rm = ResourceManager::get_instance();
	
	char filename[81];
	if (type >= 0)
		snprintf(filename, 80, "item%d.png", type);
	else if (type == ITEM_BLANK)
		snprintf(filename, 80, "blank.png");
	else if (type == ITEM_BOMB)
		snprintf(filename, 80, "bomb.png");
	else if (type == ITEM_HEALTH)
		snprintf(filename, 80, "life.png");
	else if (type == ITEM_SHIELD)
		snprintf(filename, 80, "shield.png");
	else if (type == ITEM_HEALTH_PLUS)
		snprintf(filename, 80, "health_up.png");
	else if (type == ITEM_SHIELD_PLUS)
		snprintf(filename, 80, "shield_up.png");
	else
		snprintf(filename, 80, "item0.png");
	
	char fullname[81];
	snprintf(fullname, 80, "items/%s", filename);

	m_surface = rm->get_image(fullname);
	m_dims = Rect(center[0], center[1],
	              m_surface->get_w(),
	              m_surface->get_h());
	level->add_item(this);
}

Item::~Item()
{
}

void Item::move(double dt)
{
	m_dims += dt * m_v;
}

void Item::draw(Surface *dst) const
{
	dst->blit(m_surface, m_dims[0], m_dims[1]);
}



EnergyItem::EnergyItem(Level *level, const vector2d &center)
	:Item(level, center, ITEM_ENERGY),
	 m_frame(0)
{
	m_dims[2] = m_dims[3] = 8;
	m_v = vector2d(7.5 * randint(-20, 20),
	               7.5 * randint(-20, 20));
	m_dims += m_v / 30.0;
	m_frame = randint(0, 5);
}

void EnergyItem::update()
{
	m_frame = (m_frame + 1) % 6;
}

void EnergyItem::move(double dt)
{
	if (m_v[0] > 0)
		m_v[0] -= 200 * dt;
	if (m_v[0] < 0)
		m_v[0] += 200 * dt;
	if (m_v[1] < m_speed)
		m_v[1] += 300 * dt;
	
	Item::move(dt);
}

void EnergyItem::draw(Surface *dst) const
{
	Uint32 color = mapColor(m_frame);
	vector2d center = m_dims.get_center();
	dst->draw_circle(color, center[0], center[1], m_dims[2] / 2);
}



PortalItem::PortalItem(Level *level, int color)
	:Item(level, vector2d(0, 0), ITEM_PORTAL),
	 m_color(color),
	 m_r(25),
	 m_t0(0),
	 m_t1(0)
{
	double w = m_r * 2;
	double h = m_r * 2;
	double x = 0.5 * (level->get_w() - w);
	double y = -1 * m_r;
	m_dims = Rect(x, y, w, h);
	m_v = vector2d(0, 120);
}

void PortalItem::move(double dt)
{
	Item::move(dt);

	m_t0 += 300 * dt;
	m_t1 -= 600 * dt;

	while (m_t0 > 360)
		m_t0 -= 360;
	while (m_t1 < 0)
		m_t1 += 360;
}

void PortalItem::draw(Surface *dst) const
{
	Uint32 color = mapColor(m_color);

	for (int i = 0; i < 6; ++i)
	{
		double theta = (m_t0 + i * 60) * M_PI / 180.0;
		vector2d t(m_dims[0], m_dims[1]);
		t += 0.5 * vector2d(m_dims[2], m_dims[3]);
		t += m_r * 2.0 * vector2d(cos(theta), sin(theta));
		for (int j = 0; j < 3; ++j)
		{
			double theta1 = (m_t1 + 120 * j) * M_PI / 180.0;
			double theta2 = (m_t1 + 120 * (j + 1)) * M_PI / 180.0;
			double px1 = (m_r / 2.0) * cos(theta1);
			double py1 = (m_r / 2.0) * sin(theta1);
			double px2 = (m_r / 2.0) * cos(theta2);
			double py2 = (m_r / 2.0) * sin(theta2);
			dst->draw_line(color,
			               t[0] + px1, t[1] + py1,
			               t[0] + px2, t[1] + py2);
		}
	}
}
