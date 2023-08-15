#include <iostream>
#include <cmath>
#include <vector>
#include <gtkmm.h>
using namespace std;

const double pi = 3.141592653589793;

namespace ogcpp {
	class Position {
		public:
			double x;
			double y;
			Position(double x = 0, double y = 0) {
				this->x = x;
				this->y = y;
			}
	};
	class Size {
		public:
			int width;
			int height;
			Size(int width = 200, int height = 200) {
				this->width = width;
				this->height = height;
			}
	};
	class Sprite {
		public:
			Sprite() {
				size.width = 50;
				size.height = 50;
			}
			Position position;
			Position move (double direction, double length) {
				position.x += sin((direction/180)*pi)*length;
				position.y += cos((direction/180)*pi)*length;
				update();
				return position;
			}
			Position move (string direction, double length) {
				if (direction=="up") {
					position.y += length;
				} else if (direction=="down") {
					position.y -= length;
				} else if (direction=="right") {
					position.x += length;
				} else if (direction=="left") {
					position.x -= length;
				}
				update();
				return position;
			}
			Position moveTo (double x, double y) {
				position.x = x;
				position.y = y;
				update();
				return position;
			}
			Position moveTo (Position position) {
				this->position.x = position.x;
				this->position.y = position.y;
				update();
				return this->position;
			}
			Position moveRelative (double direction, double length) {
				move(rotation+direction, length);
				update();
				return this->position;
			}
			Size size;
			Size resize (int width, int height) {
				size.width = width;
				size.height = height;
				update();
				return size;
			}
			Size resize (double xFactor, double yFactor) {
				size.width *= xFactor;
				size.height *= yFactor;
				update();
				return size;
			}
			Size resize (double factor) {
				size.width *= factor;
				size.height *= factor;
				update();
				return size;
			}
			Size resize (Size size) {
				this->size.width = size.width;
				this->size.height = size.height;
				update();
				return size;
			}
			double rotation = 0;
			double rotate (double value) {
				rotation += value;
				update();
				return rotation;
			}
			double rotateTo (double value) {
				rotation = value;
				update();
				return rotation;
			}
			bool visibility = true;
			bool show () {
				visibility = true;
				update();
				return visibility;
			}
			bool hide () {
				visibility = false;
				update();
				return visibility;
			}
			bool toggle () {
				visibility = !visibility;
				update();
				return visibility;
			}
			string costume;
			string changeCostume (string src) {
				costume = src;
				update();
				return costume;
			}
			virtual void keypress (GdkEventKey* event) {}
			double distance (Sprite* reference) {
				double ogcpp_temporal_xDistance = max(position.x, reference->position.x) - min(position.x, reference->position.x);
				double ogcpp_temporal_yDistance = max(position.y, reference->position.y) - min(position.y, reference->position.y);
				return sqrt((ogcpp_temporal_xDistance*ogcpp_temporal_xDistance)+(ogcpp_temporal_yDistance*ogcpp_temporal_yDistance));
			}
			struct {
				string type = "range";
				double rangeRadius = 25;
			} hitbox;
			bool collision (Sprite* reference) {
				double ogcpp_temporal_distance = distance(reference);
				if (ogcpp_temporal_distance<=hitbox.rangeRadius+reference->hitbox.rangeRadius) {
					return true;
				} else {
					return false;
				}
			}
			virtual void update () {}
	};
	class Stage {
		public:
			Stage(string title = "", int width = 200, int height = 200) {
				window.add(system);
				system.show();
				rename(title);
				resize(width, height);
				window.signal_key_press_event().connect(sigc::mem_fun(*this, &Stage::keypress));
				update();
			}
			string title;
			string rename (string title) {
				this->title = title;
				update();
				return this->title;
			}
			Size size;
			Size resize (int width, int height) {
				size.width = width;
				size.height = height;
				update();
				updateAllChilds();
				return size;
			}
			Size resize (double xFactor, double yFactor) {
				size.width *= xFactor;
				size.height *= yFactor;
				update();
				updateAllChilds();
				return size;
			}
			Size resize (double factor) {
				size.width *= factor;
				size.height *= factor;
				update();
				updateAllChilds();
				return size;
			}
			Size resize (Size size) {
				this->size.width = size.width;
				this->size.height = size.height;
				update();
				updateAllChilds();
				return this->size;
			}
			Position toPixel (double x, double y) {
				Position ogcpp_temporal_position(
					(size.width/2) + x,
					(size.height/2) - y
				);
				return ogcpp_temporal_position;
			}
			Position toPixel (Position position) {
				Position ogcpp_temporal_position(
					(size.width/2) + position.x,
					(size.height/2) - position.y
				);
				return ogcpp_temporal_position;
			}
			Position toCoordinate (double x, double y) {
				Position ogcpp_temporal_position(
					x - (size.width/2),
					(y - (size.height/2))*-1
				);
				return ogcpp_temporal_position;
			}
			Position toCoordinate (Position position) {
				Position ogcpp_temporal_position(
					position.x - (size.width/2),
					(position.y - (size.height/2))*-1
				);
				return ogcpp_temporal_position;
			}
			bool keypress (GdkEventKey* event) {
				for (Sprite* i : child) {
					i->keypress(event);
				}
				return false;
			}
			Gtk::Window window;
			Gtk::Fixed system;
			vector<Sprite*> child = {};
			void updateAllChilds () {
				for (Sprite* i : child) {
					i->update();
				}
			};
			void update () {
				window.set_title(title);
				window.set_size_request(size.width, size.height);
			}
	};
	class Figure : public Sprite {
		public:
			Figure(Stage* parent, string src) {
				parent->system.put(element, 0, 0);
				this->parent = parent;
				changeCostume(src);
				parent->child.push_back(this);
				update();
			}
			Gtk::Image element;
			Stage* parent;
			virtual void update () {
				parent->system.move(element, parent->toPixel(position).x-(size.width/2), parent->toPixel(position).y-(size.height/2));
				element.set(costume);
				if (!visibility) {
					element.hide();
				} else {
					if (parent->toPixel(position).x>(parent->size.width-(size.width/2)) || parent->toPixel(position).y>(parent->size.height-(size.height/2))) {
						element.hide();
					} else {
						element.show();
					}
				}
				parent->update();
				onUpdate();
			}
			virtual void onUpdate () {}
	};
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