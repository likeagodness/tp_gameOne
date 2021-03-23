#pragma once
#include <SFML/Window.hpp>
#include <shooter.h>

class player : public shooter //класс игрока - наследует пол€ и методы класса стрелка с модификатором доступа public
{
public:
	player(const float x, const float y); //объ€вление конструктора с параметрами - координатами
	player(const player& player) : player(player.t_x, player.t_y) //конструктор копировани€ - дл€ копировани€ объекта, создани€ нескольких копий	
	{

	}
	~player(); //объ€вление деструктора наследника

	float timeBeforeShot; //переменна€ - врем€ до выстрела
	void move_1(const sf::Int64 time); //объ€вление функции-перемещени€ игрока 1 
	void update_1(const sf::Int64 time, map& map); //объ€вление функции-обновлени€ объекта-игрока 1 во врем€ игры по карте
	void move_2(const sf::Int64 time); //объ€вление функции-перемещени€ игрока 2 
	void update_2(const sf::Int64 time, map& map); //объ€вление функции-обновлени€ объекта-игрока 2 во врем€ игры по карте
};