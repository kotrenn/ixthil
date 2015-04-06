#ifndef __ITEM_H
#define __ITEM_H

#include "level.h"
#include "surface.h"

#define ITEM_BLANK       -1
#define ITEM_BOMB        -2
#define ITEM_HEALTH      -3
#define ITEM_SHIELD      -4
#define ITEM_HEALTH_PLUS -5
#define ITEM_SHIELD_PLUS -6
#define ITEM_ENERGY      -7
#define ITEM_PORTAL      -8

class Level;

class Item
{
public:
	Item(Level *level, const vector2d &center, int type);
	virtual ~Item();

	virtual void update() {}
	virtual void move(double dt);
	virtual void draw(Surface *dst) const;

	const Shape *get_shape() const { return &m_dims; }
	int get_type() const { return m_type; }
	void set_v(const vector2d &v) { m_v = v; }
	void set_pulled(bool pulled) { m_pulled = pulled; }
	bool get_pulled() const { return m_pulled; }
protected:
	Rect m_dims;
	int m_type;
	Surface *m_surface;
	bool m_pulled;
	double m_speed;
	vector2d m_v;
};

class EnergyItem : public Item
{
public:
	EnergyItem(Level *level, const vector2d &center);

	void update();
	void move(double dt);
	void draw(Surface *dst) const;
private:
	int m_frame;
};

class PortalItem : public Item
{
public:
	PortalItem(Level *level, int color);

	void move(double dt);
	void draw(Surface *dst) const;

	int get_color() const { return m_color; }
private:
	int m_color;
	int m_r;
	double m_t0;
	double m_t1;
};

#endif
