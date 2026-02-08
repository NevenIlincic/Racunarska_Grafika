#pragma once
#include <string>
#include "State.cpp"
#include "model.hpp"

class Seat {
public:
	float x, y, z;
	float r, g, b;
	State state;
	int row, column;

	Model seatModel;

	Seat(float _x, float _y, float _z, int _row, int _column);

	void reserveSeat();
	void buySeat();
	void resetSeat();
		
};
