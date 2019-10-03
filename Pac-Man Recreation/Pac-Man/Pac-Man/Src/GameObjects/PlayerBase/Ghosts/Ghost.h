// Made by Donovan Colen
#pragma once
#include "../PlayerBase.h"
#include "../Player.h"

static constexpr Vector2D s_kGhostHouse { 13, 16 };

class Ghost : public PlayerBase
{
public:
    enum class GhostModes   // the diffent modes the ghost can be in. kNone is a invalid mode
    {
        kChase,
        kScatter,
        kFrighten,
        kNone
    };

protected:
    bool m_canBeEatten;                 // to keep track of frightened mode
    bool m_wasEatten;                   // to keep track if eatten
    bool m_isInHouse;                   // to keep track of ghosts that start in the house
    bool m_canUseHouseDoor;             // to keep track if ghost can move on house door tile

    Vector2D m_targetTile;              // target tile for chase mode
    Vector2D m_scatterTarget;           // target tile for scatter mode
    Vector2D m_lastTile;                // the last tile the ghost was in for pathfinding 
    Player* m_pPlayer;                  // pointer to the player for pathfinding
    GhostModes m_curMode;               // current mode the ghost is in
    TextureEvent m_afraidTexture;       // event that holds the frightened texure
    TextureEvent m_eattenTexture;       // event that holds the eaten texture

public:
    Ghost(Player* player);

    // virtual funcs
    virtual void Update(float deltaTime_sec) override;
    virtual void ChaseMode(Vector2D oppositeDir, Vector2D curLoc);  // calculates the appropriate chase target and pathfinds to it
    virtual void Reset(int x, int y) override;  // resets the ghost to starting state 

    // void funcs
    void SetScatterTarget(Vector2D target) { m_scatterTarget = target; }    // set the target 
    void FrightenedMode(Vector2D oppositeDir, Vector2D curLoc);     // the path finding asociated with the mode
    void ScatterMode(Vector2D oppositeDir, Vector2D curLoc, Vector2D target);   // standard pathfinding for the ghosts
    void Eatten();  // changes sprite and pathfinds back to ghost house
    void SetMode(Ghost::GhostModes newMode);    // sets the mode of the ghost
    void InHouse(Vector2D oppositeDir, Vector2D curLoc);    // the behavior for when a ghost is in the house
    void LeaveHouse(Vector2D oppositeDir, Vector2D curLoc); // the behavior for how the ghost leaves the house
    void NowLeaveHouse() { m_canUseHouseDoor = true; }  // sets the bool so ghost can leave the house
    void SetFrightendModeTextures(TextureEvent afraid, TextureEvent eatten);    // sets the textures for frightened mode

    // return type funcs
    bool CanBeEatten() const { return m_canBeEatten; }  // returns true if it can be eatten
    bool IsInHouse() const { return m_isInHouse; }  // returns true if in the ghost house
    GhostModes CurrentMode() { return m_curMode; }  // returns the current mode the ghost is in

};