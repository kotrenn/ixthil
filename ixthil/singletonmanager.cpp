#include "singletonmanager.h"

#include "datamanager.h"
#include "resourcemanager.h"
#include "actorfactory.h"

SingletonManager::SingletonManager()
{
	DataManager::get_instance();
	ResourceManager::get_instance();
	ActorFactory::get_instance();
}

SingletonManager::~SingletonManager()
{
	delete DataManager::get_instance();
	delete ResourceManager::get_instance();
	delete ActorFactory::get_instance();
}
