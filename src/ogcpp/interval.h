#include <functional>
#include <thread>
#include <chrono>
#ifndef OGCPP_INTERVAL_H
#define OGCPP_INTERVAL_H
using namespace std;

namespace ogcpp {
	class Interval {
		public:
			Interval(function<void()> method, int interval){
				this->method = method;
				this->interval = interval;
				element = thread([this](){
					for (int i=0; this->state; i++) {
						this_thread::sleep_for(chrono::milliseconds(this->interval));
						this->iteration++;
						this->method();
					}
				});
			}
			~Interval(){
				element.join();
			}
			thread element;
			int interval;
			int iteration = 0;
			function<void()> method;
			void stop () {
				this->state = false;
			}
		protected:
			bool state = true;
	};
}
#endif