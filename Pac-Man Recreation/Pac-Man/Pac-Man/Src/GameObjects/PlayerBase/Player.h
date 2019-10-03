// Made by Donovan Colen
#pragma once
#include "PlayerBase.h"

class Player : public PlayerBase
{
private:
    int m_score;
    int m_lives;

public:
    Player();

    // void funcs
    void Move(KeyCommand* pKeyCommand); //sets the move dir based on input
    void LoseLife() { m_lives--; }  // decriments the players lives
    void Update(float deltaTime_sec) override;
    void BindKey(const std::string& commandName, void(Player::*pDelegate)(KeyCommand*));    // binds the keys from the file to the function
    void AddScore(int amount) { m_score += amount; }    // add the amount to the players score

    // return type funcs
    int GetLivesLeft() { return m_lives; }  // returns the number of lives left
    int GetScore() { return m_score; }  // returns the players score
    bool IsValidDir(Vector2D newDir);   // returns true if the vector is a valid direction

};