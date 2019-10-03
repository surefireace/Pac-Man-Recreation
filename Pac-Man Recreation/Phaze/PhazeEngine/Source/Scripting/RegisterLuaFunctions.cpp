#include "RegisterLuaFunctions.h"
#include <lua.hpp>

#include "..\Generated\Application.gen.h"
void RegisterLuaFunctions(lua_State* pState)
{
	// Function table for Application
	lua_newtable(pState); // [+1]

	lua_pushcfunction(pState, Application_Foo_Glue); // [+2]
	lua_setfield(pState, -2, "Foo"); // [+1]

	lua_setglobal(pState, "Application"); // [+0]

}