#pragma once
#include <SFML/Graphics.hpp>
#include <map.h>
#include <player.h>
#include <enemy.h>
#include <flag.h>
#include <message.h>
#include <list>

class game //����� ����
{
private:
	bool gameOver; //��������� ����������-��������� ���� (���������/�� ���������)
	bool gameStarted; //��������� ����������-������ ���� (������/�� ������)

	int frags; //����������, � ������� ����������� ���������� ������������ ������� �������

	sf::RenderWindow Window; //������ Window ������ RenderWindow, � ������� ����� �������������� ���� ����
	flag Flag; //������-����
	map Map; //������-�����

	std::list<player*> Players; //������ � ��������
	std::list<player*>::iterator itp; //�������� ��� ������ � ���� �������

	std::list<enemy*> Enemies; //������ � �������
	std::list<enemy*>::iterator ite; //�������� ��� ������ � ���� �������

	std::list<bullet*> Bullets; //������ � ������ 
	std::list<bullet*>::iterator itb; //�������� ��� ������ � ���� �������

	int ex[4] = { 64 , 1088 , 64 , 1088 }; //������ ��������� ������ �� x
	int ey[4] = { 64 , 65 , 448 , 448 }; //������ ��������� ������ �� y

	int px[2] = { 128 , 1024}; //������ ��������� ������� �� x
	int py[2] = { 768 , 768}; //������ ��������� ������� �� y

	message msgStart; //������-��������� � ������ ����
	message msgOver; //������-��������� � ����� ����
	message msgLost; //������-��������� � ��������� ����
	message msgWon_1; //������-��������� � ������ ������� ������
	message msgWon_2; //������-��������� � ������ ������� ������
	message msgPlayer1Control; //������-��������� �� ���������� ������ �������
	message msgPlayer2Control; //������-��������� �� ���������� ������ �������

public:
	game(); //���������� ������������ ��� ����������

	~game(); //���������� �����������

	void plist() //�������, � ������� ����������� ������ ������� � ������������ �� �������
	{
		for (int i = 0; i < 2; i++) //2 ������
			Players.push_back(new player(px[i], py[i])); //���������� ��������������� � ����� ������
	}

	void elist() //�������, � ������� ����������� ������ ������� � ������������ �� �������
	{
		for (int i = 0; i < 4; i++) //4 �����
			Enemies.push_back(new enemy(ex[i], ey[i])); //���������� ��������������� � ����� ������
	}

	void process_events(); //���������� ������� ��������� �������
	void update(const sf::Int64 time); //���������� ������� ���������� ����
	void render(); //���������� ������� ���������� ����, � ������� �������� ������� ����
};
