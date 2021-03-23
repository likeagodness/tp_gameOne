#include <iostream>
#include <SFML/Window.hpp>
#include <game.h>

game::game(): Window(sf::VideoMode(1216, 960), "Paintball"), gameOver(false), gameStarted(false),
			  msgStart(420, 420, "Press \'Enter\' to start"), msgOver(500, 415, "Game over"),
	          msgLost(520, 480, "You lost!"), msgWon_1(480, 480, "Player 1 won!"), msgWon_2(480, 480, "Player 2 won!"),
			  msgPlayer1Control(100, 600, "Player 1 Controls:\nMove with WASD\nShoot with SPACE"), 
			  msgPlayer2Control(800, 600, "Player 2 Controls:\nMove with arrows\nShoot with ENTER"), frags(0)
//����������� ������������, �������� � ���� ������, ������ ����, �������� ����
//������������� ��������� ���� - ����������, ��������, ��������� � ������� ������� ������� 'Enter' ��� ������ ����, 
//��������� � ����� ����, ���������, ��������, ���-�� ������ ������������� �� 0
{
	sf::Clock clock; //������ clock ��� ������������ ���������� � ������ ���� �������

	Window.setFramerateLimit(60); //������������� ����������� ������ �� 60 � �������

	plist(); //�������� �������, � ������� ��������� ������ ��������
	elist(); //�������� �������, � ������� ��������� ������ �������

	while (Window.isOpen()) //���� ������� ���� � �����
	{
		sf::Int64 time = clock.getElapsedTime().asMicroseconds(); //��������� ��������� ����� � ������� ��� ����������� �����
		clock.restart(); //������������� ����
		time /= 800; //�������� ����

		process_events(); //�������� ������� ��������� �������

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //���� ������ ������� 'Enter'
			gameStarted = true; //�������� ����

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) //���� ������ ������� 'Escape'
			gameOver = true; //���������� ����

		if (!gameStarted) //����������� ��������� ���� ������ ����� � ������
		{
			gameOver = false;
		}

		if (gameStarted && !gameOver) //���� ���� �������� � ��� �� ����������
			update(time); //��������� ����

		render(); //������ �������
	}

	//����� �������� ����
	for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //�� ������ �� ����� ������ ����
	{
		delete (*itb); //�������� ������ �� ��������-����
	}

	for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //�� ������ �� ����� ������ ������
	{
		delete (*ite); //�������� ������ �� ��������-������
	}

	for (itp = Players.begin(); itp != Players.end(); itp++) //�� ������ �� ����� ������ �������
	{
		delete (*itp); //�������� ������ �� ��������-�������
	}
}

game::~game() //����������� �����������
{
	std::cout << "Game destroyed\n"; //������� ��������� ��� �������� �������� �������
}

void game::process_events() //���������� ������� ��������� �������
{
	sf::Event event; //������ event, ������� ���������� ������� ������� � � ���������
	while (Window.pollEvent(event)) //���������� ����� �������
	{
		switch (event.type) //����
		{
		case sf::Event::Closed: //������� Closed
			Window.close(); //��������� ����
			break;
		}
	}
}

void game::update(const sf::Int64 time) //���������� ������� ���������� ����
{
	for (itp = Players.begin(); itp != Players.end(); itp++) //���������� �� ������ �������
		if (!(*itp)->life) //���� ����
			++frags; //���� ��������
		else //�����
		{
			frags = 0; //�������� ���-�� �������
			break;
		}

	if (frags == 2) //���� ����� ������� �����
		gameOver = true; //���� ��������

	for (itb = Bullets.begin(); itb!= Bullets.end(); ) //���������� �� ������ ����
	{
		if (!(*itb)-> present) //���� ���� �����������
		{ 
			delete (*itb); //�������� ������
			itb = Bullets.erase(itb); //������� �� ������
		} 
		else //�����
			itb++; //���� ������ �� ������
	}

	for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //���������� �� ������ ����
	{
		if ((*itb)->owner == 'p' && (*itb)->present == true) //���� ���� �������� ����� � ��� ������������ �� �����
			for (itp = Players.begin(); itp != Players.end(); itp++) //���������� �� ������ �������
			{
				(*itb)->update(Map, time, (*itp)->t_x, (*itp)->t_y, (*itp)->t_dir); //��������� ���� � ������������ ������
			}
		if ((*itb)->owner == 'e' && (*itb)->present == true) //���� ���� �������� ���� � ��� ������������ �� �����
			for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //���������� �� ������ ������
			{
				(*itb)->update(Map, time / 4, (*ite)->t_x, (*ite)->t_y, (*ite)->t_dir); //��������� ���� � ������������ �����
			}
	}

	int PlayersCount = 1;
	for (itp = Players.begin(); itp != Players.end(); itp++) { //���������� �� ������ �������
		if ((*itp)->life) //���� ����� ���
		{
			switch (PlayersCount) //��������� ����� ����� �� �����
			{
			case 1:
				(*itp)->update_1(time, Map); //��������� �������
				(*itp)->timeBeforeShot += time; //��������� ����� �� �������� 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //���� ������ ������
				{
					if ((*itp)->timeBeforeShot > 500) //���� ����� �� ������� ����������
					{
						Bullets.push_back(new bullet((*itp)->t_x + 24, (*itp)->t_y + 32, 'p', (*itp)->t_dir)); //���������� � ��������� ������-����
						itb = Bullets.end(); //��������� �� ����� ������
						itb--; //��������� �� ������������� ����
						(*itb)->update(Map, time, (*itp)->t_x, (*itp)->t_y, (*itp)->t_dir); //��������� �
						(*itb)->present = true; //������������
						(*itp)->timeBeforeShot = 0; //�������� ����� �� ��������
					}
				}
				if ((*itp)->t_Sprite.getGlobalBounds().intersects(Flag.mSprite.getGlobalBounds())) //������ ������ 1 ��������� �� �������� �����
				{
					Flag.captureStatus = '1'; //����� 1 �������� ����
					gameOver = true; //���� ��������
				}
				break;
			case 2:
				(*itp)->update_2(time, Map); //��������� �������
				(*itp)->timeBeforeShot += time; //��������� ����� �� �������� 
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) //���� ������ ������
				{
					if ((*itp)->timeBeforeShot > 500) //���� ����� �� ������� ����������
					{
						Bullets.push_back(new bullet((*itp)->t_x + 24, (*itp)->t_y + 32, 'p', (*itp)->t_dir)); //���������� � ��������� ������-����
						itb = Bullets.end(); //��������� �� ����� ������
						itb--; //��������� �� ������������� ����
						(*itb)->update(Map, time, (*itp)->t_x, (*itp)->t_y, (*itp)->t_dir); //��������� �
						(*itb)->present = true; //������������
						(*itp)->timeBeforeShot = 0; //�������� ����� �� ��������
					}
				}
				if ((*itp)->t_Sprite.getGlobalBounds().intersects(Flag.mSprite.getGlobalBounds())) //������ ������ 2 ��������� �� �������� �����
				{
					Flag.captureStatus = '2'; //����� 2 �������� ����
					gameOver = true; //���� ��������
				}
				break;
			}
		}
		PlayersCount++;
	}


	for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //���� �� ������ ������
	{
		if ((*ite)->life) //���� ���� ���
		{
			(*ite)->update(time, Map); //��������� ���
			(*ite)->timeBeforeShot += time; //��������� ����� �� �������� 

			if ((*ite)->timeBeforeShot > 1000) //���� ���� �� ������� �������
			{
				if (rand() % 2) //�������� �������� ������ ��� ����, ���� 1
				{
					Bullets.push_back(new bullet((*ite)->t_x, (*ite)->t_y, 'e', (*ite)->t_dir)); //���������� � ��������� ������-����
					itb = Bullets.end(); //��������� �� ����� ������
					itb--; //��������� �� ������������� ����
					(*itb)->update(Map, time, (*ite)->t_x, (*ite)->t_y, (*ite)->t_dir); //��������� �
					(*itb)->present = true; //������������
				}
				(*ite)->timeBeforeShot = 0; // ����� �������� ����� �� ��������
			}

			for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //���������� �� ������ ����
			{
				for (itp = Players.begin(); itp != Players.end(); itp++) //���������� �� ������ �������
				{
					if (((*itp)->life == true) && ((*ite)->life == true)) //���� �������������� � ����������� ����� � ��������� ����
					{
						if ((*itb)->b_Sprite.getGlobalBounds().intersects((*itp)->t_Sprite.getGlobalBounds()) && (*itb)->owner == 'e') //���� ��������� ���� � ������ ���� ��������� �� �������� ������
						{
							(*itp)->collapse(); //����� ����
							(*itb)->present = false; //���� �����������
						}

						if ((*itb)->b_Sprite.getGlobalBounds().intersects((*ite)->t_Sprite.getGlobalBounds()) && (*itb)->owner == 'p') //���� ��������� ����� � ������ ���� ��������� �� �������� �����
						{
							(*ite)->collapse(); //���� ����
							(*itb)->present = false; //���� �����������
						}
					}
				}
			}

			if ((*ite)->t_Sprite.getGlobalBounds().intersects(Flag.mSprite.getGlobalBounds())) //������ ����� ��������� �� �������� �����
				{
						Flag.captureStatus = 'e'; //���� �������� ����
						gameOver = true; //���� ��������
				}
		}
	}
}

void game::render() //����������� ������� ���������� ����, � ������� �������� ������� ����
{
	Window.clear();		//������� ����
	Map.draw(Window);	//������ �����

	for (itp = Players.begin(); itp != Players.end(); itp++)
	{
		if ((*itp)->life) //���� ����� ���
			Window.draw((*itp)->t_Sprite); //������
	}

	for (itb = Bullets.begin(); itb != Bullets.end(); itb++) //���������� �� ������ ����
	if ((*itb)->present) //���� ���� ������������
		Window.draw((*itb)->b_Sprite); //������

	for (ite = Enemies.begin(); ite != Enemies.end(); ite++) //���������� �� ������ ������
	{ 
		if ((*ite)->life) //���� ���� ���
			Window.draw((*ite)->t_Sprite); //������
	}

	if (Flag.captureStatus == '0') //���� ���� �� ��������
		Window.draw(Flag.mSprite);  //������

	if (!gameStarted) { //���� ���� �� �������� 
		msgStart.print(Window); //������� ��������� � ������ ����
		msgPlayer1Control.print(Window);
		msgPlayer2Control.print(Window);
	}
	if (gameOver) //���� ���� ��������
	{
		msgOver.print(Window); //������� ��������� �� ��������� ����

		switch (Flag.captureStatus)
		{
		case '1':
			msgWon_1.print(Window); //������� ��������� � ������ 1 ������
			break;
		case '2':
			msgWon_2.print(Window); //������� ��������� � ������ 2 ������
			break;
		case 'e':
			msgLost.print(Window); //������� ��������� � ���������
			break;
		}

		if (frags == 2) //���� ���������� ����� ���� �������
			msgLost.print(Window); //������� ��������� � ������
	} 

	Window.display(); //���������� ��������� � ����
}