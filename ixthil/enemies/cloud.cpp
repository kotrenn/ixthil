#include "cloud.h"

#include "enemies.h"

CloudManager *CloudManager::m_instance = NULL;

CloudManager *CloudManager::get_instance()
{
	if (m_instance == NULL)
		m_instance = new CloudManager();
	return m_instance;
}

CloudManager::CloudManager()
	:m_clouds()
{
}

vector2d CloudManager::get_a(CloudActor *cloud)
{
	vector2d a(0, 0);
	set<CloudActor *>::iterator c;
	for (c = m_clouds.begin(); c != m_clouds.end(); c++)
	{
		if (*c != cloud)
		{
			vector2d disp = (*c)->get_center() - cloud->get_center();
			double dist = disp * disp;
			if (dist < 50 * 50 && dist > EPSILON)
			{
				dist = sqrt(dist);
				a += disp * 0.3 / dist;
			}
		}
	}

	double mag = a.norm();
	if (mag > EPSILON)
		a /= mag;
	
	return a;
}

void CloudManager::add_cloud(CloudActor *cloud)
{
	m_clouds.insert(cloud);
}

void CloudManager::remove_cloud(CloudActor *cloud)
{
	m_clouds.erase(cloud);
}



CloudActor::CloudActor()
	:Actor("cloud")
{
}

CloudActor::CloudActor(Level *level, const string &name)
	:Actor(level, name)
{
}

CloudActor::~CloudActor()
{
}

Actor *CloudActor::clone(Level *level, const string &name)
{
	CloudActor *ret = new CloudActor(level, name);

	CloudManager *cm = CloudManager::get_instance();

	double theta = randint(1, 360) * M_PI / 180.0;
	double r_0 = 0.5 * randint(1, 40);
	vector2d center = Enemies::get_cloud_center();
	center += r_0 * vector2d(cos(theta), sin(theta));
	double r = ret->m_surface->get_w() / 2;
	ret->m_shape = new Circle(center[0], center[1], r);
	
	vector2d v(0, 0);
	while (v[0] == 0 || v[1] == 0)
		v = vector2d(0.2 * randint(-10, 10),
		             0.2 * randint(2, 10));
	ret->m_v = 30 * v;
	ret->m_a = vector2d(0, 0);
	
	ret->m_health = 10;
	ret->m_color = COLOR_PURPLE;
	ret->m_score = 150;

	ret->m_shoot_timer.reset(0);
	
	ret->m_in_level = false;

	cm->add_cloud(ret);
	
	return ret;
}

void CloudActor::pause()
{
	Actor::pause();
	
	m_shoot_timer.pause();
}

void CloudActor::unpause()
{
	Actor::unpause();

	m_shoot_timer.unpause();
}

void CloudActor::die()
{
	Actor::die();

	CloudManager *cm = CloudManager::get_instance();
	cm->remove_cloud(this);
}

void CloudActor::update()
{
	Actor::update();

	CloudManager *cm = CloudManager::get_instance();
	m_a = cm->get_a(this);

	if (m_shoot_timer.done())
		create_bullet();
}

void CloudActor::move(double dt)
{
	Actor::move(dt);

	vector2d center = get_center();
	if (center[1] < 0 && m_v[1] < 0)
		m_v[1] *= -1;

	m_v += 30 * 30 * dt * m_a;

	double mag = m_v * m_v;
	double max_v = 240.0;
	if (mag > max_v * max_v)
	{
		mag = sqrt(mag);
		m_v = max_v * m_v / mag;
	}

	Rect dims = m_level->get_dims();
	dims[3] -= 200;

	if (dims.collides(m_shape))
		m_in_level = true;
	
	m_shape->fix_rect_x(dims, &m_v);
	if (m_in_level)
		m_shape->fix_rect_y(dims, &m_v);
}

void CloudActor::create_bullet()
{
	vector2d center = get_center();
	center -= 0.5 * vector2d(BULLET_W, BULLET_H);
	Rect rect(center[0], center[1], BULLET_W, BULLET_H);

	vector2d v(60 * randint(-1, 1), 30 * randint(5, 10));
	new Bullet(this, new RectBullet(rect, v), 1, m_color);

	m_shoot_timer.reset(geometric(1.0 / 100.0) / 30);
}
