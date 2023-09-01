#include <functional>
#include <thread>
#include <chrono>
#ifndef OGCPP_TIMEOUT_H
#define OGCPP_TIMEOUT_H
using namespace std;

namespace ogcpp {
	class Timeout {
		public:
			Timeout(function<void()> method, int interval){
				this->method = method;
				this->interval = interval;
				element = thread([this](){
					this_thread::sleep_for(chrono::milliseconds(this->interval));
					this->method();
				});
			}
			~Timeout(){
				element.join();
			}
			thread element;
			int interval;
			function<void()> method;
	};
}
#endif