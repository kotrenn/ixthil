#include "actorfactory.h"
#include "enemies/enemies.h"

ActorFactory *ActorFactory::m_instance = NULL;

ActorFactory *ActorFactory::get_instance()
{
	if (m_instance == NULL)
		m_instance = new ActorFactory();
	return m_instance;
}

ActorFactory::ActorFactory()
	:m_actors()
{
	insert(new SimpleActor());
	insert(new StrongActor());
	insert(new AttractorActor());
	insert(new SmartActor());
	insert(new BounceActor());
	insert(new GroupCreator());
	insert(new CloudActor());

	insert(new DarkSimpleActor());
	insert(new BackAndForthActor());
	insert(new DarkAttractorActor());
	
	insert(new QuickActor());
}

ActorFactory::~ActorFactory()
{
	map<string, Actor *>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
		delete it->second;
}

void ActorFactory::insert(Actor *actor)
{
	m_actors[actor->get_name()] = actor;
}

Actor *ActorFactory::load_actor(Level *level,
                                const string &name)
{
	if (m_actors.find(name) == m_actors.end())
	{
		fprintf(stderr, "Error: no actor named %s\n", name.c_str());
		exit(1);
	}
	return m_actors[name]->clone(level, name);
}
