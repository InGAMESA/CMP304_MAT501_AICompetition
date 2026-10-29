// RacingComp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameController.h"


/*
HandleEvents will capture SFML keyboard and mouse input and route to Game Controller to handle
*/
void HandleEvents(sf::RenderWindow& window, GameController* gameController)
{

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                window.close();
            else
            {
                gameController->HandleKeyPress(keyPressed);
            }
        }
     
    }
}




int main()
{
    // define window
    sf::RenderWindow window(sf::VideoMode({ 1024, 640 }), "Racing Game");
    window.setVerticalSyncEnabled(true);

    //define the views
    sf::View gameView(sf::FloatRect({ 0, 0 }, { 1024, 640 }));

    gameView.setViewport(sf::FloatRect({ 0.f, 0.f }, { 1.f, 1.f }));
  
    gameView.setCenter({ 512 ,320 });

    sf::Clock clock;

    // define the game controller, init and start the game 
    GameController gameController;

    gameController.Init();


    //display and game loop
    while (window.isOpen())
    {
        window.clear();

        // handle all events
        HandleEvents(window, &gameController);

        // tick the game controller 
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        gameController.Update(dt);

        gameController.Render(window);

        //call the function to display
        window.display();



    }
}

