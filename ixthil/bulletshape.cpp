#include "bulletshape.h"

BulletShape::BulletShape(Shape *shape,
                         const vector2d &v)
	:m_shape(shape),
	 m_v(v)
{
}

BulletShape::~BulletShape()
{
	delete m_shape;
}



RectBullet::RectBullet(const Rect &rect,
                       const vector2d &v)
	:BulletShape(new Rect(rect), v)
{
}

RectBullet::~RectBullet()
{
}

void RectBullet::move(double dt)
{
	Rect *rect = (Rect *)m_shape;
	rect->set_x(rect->get_x() + dt * m_v[0]);
	rect->set_y(rect->get_y() + dt * m_v[1]);
}

void RectBullet::draw(Surface *dst, Uint32 color) const
{
	dst->draw_rect(color, *(Rect *)m_shape, 2);
}



CircleBullet::CircleBullet(const Circle &circle,
                           const vector2d &v)
	:BulletShape(new Circle(circle), v)
{
}

CircleBullet::~CircleBullet()
{
}

void CircleBullet::move(double dt)
{
	Circle *circle = (Circle *)m_shape;
	circle->set_x(circle->get_x() + dt * m_v[0]);
	circle->set_y(circle->get_y() + dt * m_v[1]);
}

void CircleBullet::draw(Surface *dst, Uint32 color) const
{
	Circle *circle = (Circle *)m_shape;
	int x = (int)circle->get_x();
	int y = (int)circle->get_y();
	int r = (int)circle->get_r();
	dst->draw_circle(color, x, y, r, 1);
}



LineBullet::LineBullet(const Line &line,
                       const vector2d &v)
	:BulletShape(new Line(line), v)
{
}

LineBullet::~LineBullet()
{
}

void LineBullet::move(double dt)
{
	Line *line = (Line *)m_shape;
	line->set_x1(line->get_x1() + dt * m_v[0]);
	line->set_y1(line->get_y1() + dt * m_v[1]);
	line->set_x2(line->get_x2() + dt * m_v[0]);
	line->set_y2(line->get_y2() + dt * m_v[1]);
}

void LineBullet::draw(Surface *dst, Uint32 color) const
{
	Line *line = (Line *)m_shape;
	int x1 = (int)line->get_x1();
	int y1 = (int)line->get_y1();
	int x2 = (int)line->get_x2();
	int y2 = (int)line->get_y2();
	dst->draw_line(color, x1, y1, x2, y2);
}
