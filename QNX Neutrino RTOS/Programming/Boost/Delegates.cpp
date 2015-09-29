#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

class Widget
{
public:
    boost::function<void ()> onClick;
};

class Player
{
public:
    void play()
    {
    	std::cout << "Player::play" << std::endl;
    }
    
    void stop()
    {
    	std::cout << "Player::stop" << std::endl;
    }
};

void testDelegates()
{
	Widget playWidget;
	Widget stopWidget;
	Player player;

    playWidget.onClick = boost::bind(&Player::play, &player);
    stopWidget.onClick = boost::bind(&Player::stop, &player);
    
    playWidget.onClick();
    stopWidget.onClick();
}

