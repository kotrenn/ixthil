#ifndef __BULLET_SHAPE_H
#define __BULLET_SHAPE_H

#include "common.h"
#include "surface.h"

class BulletShape
{
public:
	BulletShape(Shape *shape,
	            const vector2d &v);
	virtual ~BulletShape();

	virtual void move(double dt) = 0;
	virtual void draw(Surface *dst, Uint32 color) const = 0;

	Shape *get_shape() const { return m_shape; }
	vector2d &get_v() { return m_v; }
	const vector2d &get_v() const { return m_v; }
	void set_v(const vector2d &v) { m_v = v; }
protected:
	Shape *m_shape;
	vector2d m_v;
};

class RectBullet : public BulletShape
{
public:
	RectBullet(const Rect &rect,
	           const vector2d &v);
	~RectBullet();

	void move(double dt);
	void draw(Surface *dst, Uint32 color) const;
};

class CircleBullet : public BulletShape
{
public:
	CircleBullet(const Circle &circle,
	             const vector2d &v);
	~CircleBullet();
	
	void move(double dt);
	void draw(Surface *dst, Uint32 color) const;
};

class LineBullet : public BulletShape
{
public:
	LineBullet(const Line &line,
	           const vector2d &v);
	~LineBullet();
	
	void move(double dt);
	void draw(Surface *dst, Uint32 color) const;
};

#endif
