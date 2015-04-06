#ifndef __ACTOR_FACTORY_H
#define __ACTOR_FACTORY_H

#include "actor.h"

class ActorFactory
{
public:
	static ActorFactory *get_instance();
	~ActorFactory();

	Actor *load_actor(Level *level,
	                  const string &name);
private:
	ActorFactory();
	static ActorFactory *m_instance;
	
	void insert(Actor *actor);
	
	map<string, Actor *> m_actors;
};

#endif
