#include "../src/ogcpp.h"
#include <iostream>
using namespace ogcpp;

int main (int argc, char* argv[]) {
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Stage stage;
	return app->run(stage.window);
}