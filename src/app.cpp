#include "headers/game.hpp"
#include <memory>

int main(int /*argn*/, char* /*args*/ [])
{
    std::unique_ptr<NSGame::Game> game(new NSGame::Game());
    int game_res = game->exec();
    return game_res;
}
