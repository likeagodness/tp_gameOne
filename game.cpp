#include <iostream>
#include <SFML/Window.hpp>
#include <game.h>

game::game(): Window(sf::VideoMode(1216, 960), "Paintball"), gameOver(false), gameStarted(false),
			  msgStart(420, 420, "Press \'Enter\' to start"), msgOver(500, 415, "Game over"),
	          msgLost(520, 480, "You lost!"), msgWon_1(480, 480, "Player 1 won!"), msgWon_2(480, 480, "Player 2 won!"),
			  msgPlayer1Control(100, 600, "Player 1 Controls:\nMove with WASD\nShoot with SPACE"), 
			  msgPlayer2Control(800, 600, "Player 2 Controls:\nMove with arrows\nShoot with ENTER"), frags(0)
//определение конструктора, передаем в него ширину, высоту окна, название окна
//устанавливаем состояния игры - неокончена, неначата, сообщение о просьбе нажатия клавиши 'Enter' для начала игры, 
//сообщения о конце игры, проигрыше, выйгрыше, кол-во фрагов устанавливаем на 0
{
	sf::Clock clock; //объект clock для отслеживания прошедшего с начала игры времени

	Window.setFramerateLimit(60); //устанавливаем ограничение кадров на 60 в секунду

	plist(); //вызываем функцию, в которой заполняем список игроками
	elist(); //вызываем функцию, в которой заполняем список врагами

	while (Window.isOpen()) //пока открыто окно с игрой
	{
		sf::Int64 time = clock.getElapsedTime().asMicroseconds(); //считываем прошедшее время с запуска или перезапуска часов
		clock.restart(); //перезапускаем часы
		time /= 800; //скорость игры

		process_events(); //вызываем функцию обработки событий

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //если нажали клавишу 'Enter'
			gameStarted = true; //начинаем игру

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) //если нажали клавишу 'Escape'
			gameOver = true; //оканчиваем игру

		if (!gameStarted) //возможность завершить игру только после её начала
		{
			gameOver = false;
		}

		if (gameStarted && !gameOver) //Если игра началась и ещё не окончилась
			update(time); //обновляем игру

		render(); //рисуем объекты
	}

	//после закрытия окна
	for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //от начала до конца списка пуль
	{
		delete (*itb); //вычищаем память от объектов-пуль
	}

	for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //от начала до конца списка врагов
	{
		delete (*ite); //вычищаем память от объектов-врагов
	}

	for (itp = Players.begin(); itp != Players.end(); itp++) //от начала до конца списка игроков
	{
		delete (*itp); //вычищаем память от объектов-игроков
	}
}

game::~game() //определение деструктора
{
	std::cout << "Game destroyed\n"; //выводим сообщение для проверки удаления объекта
}

void game::process_events() //объявление функции обработки событий
{
	sf::Event event; //объект event, который определяет событие системы и её параметры
	while (Window.pollEvent(event)) //производим опрос события
	{
		switch (event.type) //если
		{
		case sf::Event::Closed: //событие Closed
			Window.close(); //закрываем окно
			break;
		}
	}
}

void game::update(const sf::Int64 time) //объявление функции обновления игры
{
	for (itp = Players.begin(); itp != Players.end(); itp++) //проходимся по списку игроков
		if (!(*itp)->life) //если умер
			++frags; //игра окончена
		else //иначе
		{
			frags = 0; //обнуляем кол-во убийств
			break;
		}

	if (frags == 2) //если обоих игроков убили
		gameOver = true; //игра окончена

	for (itb = Bullets.begin(); itb!= Bullets.end(); ) //проходимся по списку пуль
	{
		if (!(*itb)-> present) //если пуля отсутствует
		{ 
			delete (*itb); //вычищаем память
			itb = Bullets.erase(itb); //убираем из списка
		} 
		else //иначе
			itb++; //идем дальше по списку
	}

	for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //проходимся по списку пуль
	{
		if ((*itb)->owner == 'p' && (*itb)->present == true) //если пулю запустил игрок и она присутствует на карте
			for (itp = Players.begin(); itp != Players.end(); itp++) //проходимся по списку игроков
			{
				(*itb)->update(Map, time, (*itp)->t_x, (*itp)->t_y, (*itp)->t_dir); //обновляем пулю с координатами игрока
			}
		if ((*itb)->owner == 'e' && (*itb)->present == true) //если пулю запустил враг и она присутствует на карте
			for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //проходимся по списку врагов
			{
				(*itb)->update(Map, time / 4, (*ite)->t_x, (*ite)->t_y, (*ite)->t_dir); //обновляем пулю с координатами врага
			}
	}

	int PlayersCount = 1;
	for (itp = Players.begin(); itp != Players.end(); itp++) { //проходимся по списку игроков
		if ((*itp)->life) //если игрок жив
		{
			switch (PlayersCount) //проверяем какой игрок по счету
			{
			case 1:
				(*itp)->update_1(time, Map); //обновляем первого
				(*itp)->timeBeforeShot += time; //приращаем время до выстрела 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //если нажали пробел
				{
					if ((*itp)->timeBeforeShot > 500) //если игрок не стрелял полсекунды
					{
						Bullets.push_back(new bullet((*itp)->t_x + 24, (*itp)->t_y + 32, 'p', (*itp)->t_dir)); //отправляем в контейнер объект-пулю
						itb = Bullets.end(); //переходим на конец списка
						itb--; //переходим на новосозданную пулю
						(*itb)->update(Map, time, (*itp)->t_x, (*itp)->t_y, (*itp)->t_dir); //обновляем её
						(*itb)->present = true; //присутствует
						(*itp)->timeBeforeShot = 0; //обнуляем время до выстрела
					}
				}
				if ((*itp)->t_Sprite.getGlobalBounds().intersects(Flag.mSprite.getGlobalBounds())) //спрайт игрока 1 пересекся со спрайтом флага
				{
					Flag.captureStatus = '1'; //игрок 1 захватил флаг
					gameOver = true; //игра окончена
				}
				break;
			case 2:
				(*itp)->update_2(time, Map); //обновляем второго
				(*itp)->timeBeforeShot += time; //приращаем время до выстрела 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //если нажали пробел
				{
					if ((*itp)->timeBeforeShot > 500) //если игрок не стрелял полсекунды
					{
						Bullets.push_back(new bullet((*itp)->t_x + 24, (*itp)->t_y + 32, 'p', (*itp)->t_dir)); //отправляем в контейнер объект-пулю
						itb = Bullets.end(); //переходим на конец списка
						itb--; //переходим на новосозданную пулю
						(*itb)->update(Map, time, (*itp)->t_x, (*itp)->t_y, (*itp)->t_dir); //обновляем её
						(*itb)->present = true; //присутствует
						(*itp)->timeBeforeShot = 0; //обнуляем время до выстрела
					}
				}
				if ((*itp)->t_Sprite.getGlobalBounds().intersects(Flag.mSprite.getGlobalBounds())) //спрайт игрока 2 пересекся со спрайтом флага
				{
					Flag.captureStatus = '2'; //игрок 2 захватил флаг
					gameOver = true; //игра окончена
				}
				break;
			}
		}
		PlayersCount++;
	}


	for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //идем по списку врагов
	{
		if ((*ite)->life) //если враг жив
		{
			(*ite)->update(time, Map); //обновляем его
			(*ite)->timeBeforeShot += time; //приращаем время до выстрела 

			if ((*ite)->timeBeforeShot > 1000) //если враг не стрелял секунду
			{
				if (rand() % 2) //случайно выбираем истину или ложь, если 1
				{
					Bullets.push_back(new bullet((*ite)->t_x, (*ite)->t_y, 'e', (*ite)->t_dir)); //отправляем в контейнер объект-пулю
					itb = Bullets.end(); //переходим на конец списка
					itb--; //переходим на новосозданную пулю
					(*itb)->update(Map, time, (*ite)->t_x, (*ite)->t_y, (*ite)->t_dir); //обновляем её
					(*itb)->present = true; //присутствует
				}
				(*ite)->timeBeforeShot = 0; // иначе обнуляем время до выстрела
			}

			for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //проходимся по списку пуль
			{
				for (itp = Players.begin(); itp != Players.end(); itp++) //проходимся по списку игроков
				{
					if (((*itp)->life == true) && ((*ite)->life == true)) //если учавствующиеся в перестрелке игрок и противник живы
					{
						if ((*itb)->b_Sprite.getGlobalBounds().intersects((*itp)->t_Sprite.getGlobalBounds()) && (*itb)->owner == 'e') //если выстрелил враг и спрайт пули пересекся со спрайтом игрока
						{
							(*itp)->collapse(); //игрок умер
							(*itb)->present = false; //пуля отсутствует
						}

						if ((*itb)->b_Sprite.getGlobalBounds().intersects((*ite)->t_Sprite.getGlobalBounds()) && (*itb)->owner == 'p') //если выстрелил игрок и спрайт пули пересекся со спрайтом врага
						{
							(*ite)->collapse(); //враг умер
							(*itb)->present = false; //пуля отсутствует
						}
					}
				}
			}

			if ((*ite)->t_Sprite.getGlobalBounds().intersects(Flag.mSprite.getGlobalBounds())) //спрайт врага пересекся со спрайтом флага
				{
						Flag.captureStatus = 'e'; //враг захватил флаг
						gameOver = true; //игра окончена
				}
		}
	}
}

void game::render() //определение функции обновления игры, в которой рисуются объекты игры
{
	Window.clear();		//очищаем окно
	Map.draw(Window);	//рисуем карту

	for (itp = Players.begin(); itp != Players.end(); itp++)
	{
		if ((*itp)->life) //если игрок жив
			Window.draw((*itp)->t_Sprite); //рисуем
	}

	for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //проходимся по списку пуль
	if ((*itb)->present) //если пуля присутствует
		Window.draw((*itb)->b_Sprite); //рисуем

	for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //проходимся по списку врагов
	{ 
		if ((*ite)->life) //если враг жив
			Window.draw((*ite)->t_Sprite); //рисуем
	}

	if (Flag.captureStatus == '0') //если флаг не захвачен
		Window.draw(Flag.mSprite);  //рисуем

	if (!gameStarted) { //если игра не началась 
		msgStart.print(Window); //выводим сообщение о старте игры
		msgPlayer1Control.print(Window);
		msgPlayer2Control.print(Window);
	}
	if (gameOver) //если игра окончена
	{
		msgOver.print(Window); //выводем сообщение об окончании игры

		switch (Flag.captureStatus)
		{
		case '1':
			msgWon_1.print(Window); //выводим сообщение о победе 1 игрока
			break;
		case '2':
			msgWon_2.print(Window); //выводим сообщение о победе 2 игрока
			break;
		case 'e':
			msgLost.print(Window); //выводим сообщение о проигрыше
			break;
		}

		if (frags == 2) //если противники убили всех игроков
			msgLost.print(Window); //выводим сообщение о победе
	} 

	Window.display(); //отоброжаем результат в окне
}