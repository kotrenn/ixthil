#include "camera.h"

Camera::Camera(int w, int h, Surface *surface)
	:m_surface(NULL),
	 m_x(0), m_y(0),
	 m_w(w), m_h(h)
{
	if (surface == NULL)
		m_surface = new Surface(m_w, m_h);
	else
		m_surface = new Surface(surface);
}

Camera::~Camera()
{
	delete m_surface;
}

void Camera::move(double)
{
}

void Camera::clear()
{
	m_surface->fill(0);
}

void Camera::draw(Surface *dst) const
{
	dst->blit(m_surface, m_x, m_y);
}

const vector2d Camera::get_pos() const
{
	vector2d ret(m_x, m_y);
	return ret;
}

const vector2d Camera::get_size() const
{
	vector2d ret(m_w, m_h);
	return ret;
}

const Rect Camera::get_dims() const
{
	Rect ret(m_x, m_y, m_w, m_h);
	return ret;
}

void Camera::set_surface(Surface *surface)
{
	if (m_surface == surface)
		return;
	delete m_surface;
	m_surface = new Surface(surface);
}

void Camera::set_pos(const vector2d &pos)
{
	m_x = pos[0];
	m_y = pos[1];
}
