#ifndef OGCPP_PLAYER_H
#define OGCPP_PLAYER_H
#include "./core.h"
using namespace std;

namespace ogcpp {
	class Player : public Figure {
		public:
			Player(Stage* stage, string src) : Figure(stage, src) {}
			struct {
				int up = 119;
				int down = 115;
				int right = 100;
				int left = 97;
			} keys;
			double length = 10;
			void keypress (GdkEventKey* event) {
				if (event->keyval==keys.up) {
					move("up", length);
				} else if (event->keyval==keys.down) {
					move("down", length);
				} else if (event->keyval==keys.right) {
					move("right", length);
				} else if (event->keyval==keys.left) {
					move("left", length);
				}
				update();
			}
	};
}
#endif