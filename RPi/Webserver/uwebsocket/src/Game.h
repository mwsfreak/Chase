#include "Player.h"

class Game
{
public:
    Game(int Penalty, string Pl1, string Pl2, string Pl3, string Pl4 = "unknown", string Pl5 = "unknown", string Pl6 = "unknown", string Pl7 = "unknown", string Pl8 = "unknown");
    void updateGame(int penltyPlayer, int timePlayer, int time);
    ~Game();
private:
    static int gameState;
    static int totalPenalty;
    int maxPenalty;
    Player players[8];
};