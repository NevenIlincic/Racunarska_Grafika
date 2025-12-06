#include <string>
#include "State.cpp"

class Seat {
public:
	float x, y;
	float r, g, b;
	State state;

	Seat(float _x, float _y)
		: x(_x), y(_y), r(0.0f), g(0.0f), b(1.0f), state(State::Free) {
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
				state = State::Free;
				r = 0.0f;
				g = 0.0f;
				b = 1.0f;
			}
		}
	};

	void buySeat() {
		state = State::Bought;
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}
};