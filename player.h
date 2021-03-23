#pragma once
#include <SFML/Window.hpp>
#include <shooter.h>

class player : public shooter //����� ������ - ��������� ���� � ������ ������ ������� � ������������� ������� public
{
public:
	player(const float x, const float y); //���������� ������������ � ����������� - ������������
	player(const player& player) : player(player.t_x, player.t_y) //����������� ����������� - ��� ����������� �������, �������� ���������� �����	
	{

	}
	~player(); //���������� ����������� ����������

	float timeBeforeShot; //���������� - ����� �� ��������
	void move_1(const sf::Int64 time); //���������� �������-����������� ������ 1 
	void update_1(const sf::Int64 time, map& map); //���������� �������-���������� �������-������ 1 �� ����� ���� �� �����
	void move_2(const sf::Int64 time); //���������� �������-����������� ������ 2 
	void update_2(const sf::Int64 time, map& map); //���������� �������-���������� �������-������ 2 �� ����� ���� �� �����
};