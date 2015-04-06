#ifndef __CLOUD_H
#define __CLOUD_H

#include "../actor.h"

class CloudActor;

class CloudManager
{
public:
	static CloudManager *get_instance();
	~CloudManager();

	vector2d get_a(CloudActor *actor);
	void add_cloud(CloudActor *cloud);
	void remove_cloud(CloudActor *cloud);
private:
	CloudManager();
	static CloudManager *m_instance;

	set<CloudActor *> m_clouds;
};

class CloudActor : public Actor
{
public:
	CloudActor();
	CloudActor(Level *level, const string &name);
	~CloudActor();

	Actor *clone(Level *level, const string &name);

	void pause();
	void unpause();

	void die();

	void update();
	void move(double dt);

	void create_bullet();
private:
	bool m_in_level;
	Timer m_shoot_timer;
	vector2d m_a;
};

#endif
