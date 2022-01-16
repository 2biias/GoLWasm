#include <gameOfLife/application.hpp>
#include <gameEngine/controller.hpp>

int main()
{
    auto app = GameOfLife::Application(CELLSPRAXIS, 15);
    app.start();
}