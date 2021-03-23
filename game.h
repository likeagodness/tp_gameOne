#pragma once
#include <SFML/Graphics.hpp>
#include <map.h>
#include <player.h>
#include <enemy.h>
#include <flag.h>
#include <message.h>
#include <list>

class game //класс игры
{
private:
	bool gameOver; //булевска€ переменна€-состо€ние игры (закончена/не закончена)
	bool gameStarted; //булевска€ переменна€-начало игры (начата/не начата)

	int frags; //переменна€, в которой отслеживает количество уничтоженных врагами игроков

	sf::RenderWindow Window; //объект Window класса RenderWindow, в котором будет отрисовыватьс€ наша игра
	flag Flag; //объект-флаг
	map Map; //объект-карта

	std::list<player*> Players; //список с игроками
	std::list<player*>::iterator itp; //итератор дл€ работы с этим списком

	std::list<enemy*> Enemies; //список с врагами
	std::list<enemy*>::iterator ite; //итератор дл€ работы с этим списком

	std::list<bullet*> Bullets; //список с пул€ми 
	std::list<bullet*>::iterator itb; //итератор дл€ работы с этим списком

	int ex[4] = { 64 , 1088 , 64 , 1088 }; //массив координат врагов по x
	int ey[4] = { 64 , 65 , 448 , 448 }; //массив координат врагов по y

	int px[2] = { 128 , 1024}; //массив координат игроков по x
	int py[2] = { 768 , 768}; //массив координат игроков по y

	message msgStart; //объект-сообщение о старте игры
	message msgOver; //объект-сообщение о конце игры
	message msgLost; //объект-сообщение о проигрыше игры
	message msgWon_1; //объект-сообщение о победе первого игрока
	message msgWon_2; //объект-сообщение о победе второго игрока
	message msgPlayer1Control; //объект-сообщение об управлении первым игроком
	message msgPlayer2Control; //объект-сообщение об управлении вторым игроком

public:
	game(); //объ€вление конструктора без параметров

	~game(); //объ€вление деструктора

	void plist() //функци€, в которой заполн€етс€ список врагами с координатами из массива
	{
		for (int i = 0; i < 2; i++) //2 игрока
			Players.push_back(new player(px[i], py[i])); //отправл€ем последовательно в конец списка
	}

	void elist() //функци€, в которой заполн€етс€ список врагами с координатами из массива
	{
		for (int i = 0; i < 4; i++) //4 врага
			Enemies.push_back(new enemy(ex[i], ey[i])); //отправл€ем последовательно в конец списка
	}

	void process_events(); //объ€вление функции обработки событий
	void update(const sf::Int64 time); //объ€вление функции обновлени€ игры
	void render(); //объ€вление функции обновлени€ игры, в которой рисуютс€ объекты игры
};
