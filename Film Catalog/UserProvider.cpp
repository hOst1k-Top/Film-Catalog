#include "include/UserProvider.h"
UserProvider* UserProvider::instance = nullptr;

UserProvider* UserProvider::getInstance()
{
	if (instance == nullptr) instance = new UserProvider();
	return instance;
}
