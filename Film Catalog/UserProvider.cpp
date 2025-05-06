#include "include/UserProvider.h"

UserProvider* UserProvider::getInstance()
{
	static UserProvider instance;
	return &instance;
}
