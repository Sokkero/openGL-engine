#include "SingletonManager.h"

std::map<const char*, std::shared_ptr<SingletonBase>> SingletonManager::s_objects;
