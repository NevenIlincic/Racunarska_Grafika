#pragma once // ili
#include <string>
#include "State.cpp"

class Seat {
public:
	float x, y, z;
	float r, g, b;
	State state;
	int row, column;

	Seat(float _x, float _y, float _z, int _row, int _column)
		: x(_x), y(_y), z(_z), row(_row), column(_column), r(0.0f), g(0.0f), b(1.0f), state(State::Free) {
	};

	void reserveSeat() {
		if (state != State::Bought) {
			if (state == State::Free) {
				state = State::Reserved;
				r = 1.0f;
				g = 1.0f;
				b = 0.0f;
			}
			else {
				resetSeat();
			}
		}
	};

	void buySeat() {
		state = State::Bought;
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	void resetSeat() {
		state = State::Free;
		r = 0.0f;
		g = 0.0f;
		b = 1.0f;
	}
};
