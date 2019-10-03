#include "Application.gen.h"

#include "../Application/Application.h"
int Application_Foo_Glue(lua_State* pState)
{
	void* _pAddress = lua_touserdata(pState, -2);
	int _n = (int)lua_tointeger(pState, -1);

	Application* _pApplication = static_cast<Application*>(_pAddress);
	_pApplication->Foo(_n);

	lua_pop(pState, 2);
	return 0;
}

