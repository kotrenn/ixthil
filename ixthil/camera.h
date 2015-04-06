#ifndef __CAMERA_H
#define __CAMERA_H

#include "common.h"
#include "surface.h"

class Camera
{
public:
	Camera(int w, int h, Surface *surface = NULL);
	~Camera();

	void move(double dt);
	void clear();
	void draw(Surface *dst) const;

	Surface *get_surface() const { return m_surface; }
	double get_x() const { return m_x; }
	double get_y() const { return m_y; }
	int get_w() const { return m_w; }
	int get_h() const { return m_h; }
	const vector2d get_pos() const;
	const vector2d get_size() const;
	const Rect get_dims() const;
	void set_surface(Surface *surface);
	void set_x(double x) { m_x = x; }
	void set_y(double y) { m_x = y; }
	void set_w(int w) { m_w = w; }
	void set_h(int h) { m_h = h; }
	void set_pos(const vector2d &pos);
private:
	Surface *m_surface;
	double m_x;
	double m_y;
	int m_w;
	int m_h;
	double m_t;
};

#endif
