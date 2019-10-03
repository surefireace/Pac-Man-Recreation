// Made by Donovan Colen
#include "World.h"

World* g_pGame = World::GetInstance(); // sigleton instance

int main(int argc, char *argv[])
{
    g_pGame->Init();

    delete g_pGame;
    return 0;
}