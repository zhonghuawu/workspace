#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <utility>
template<typename T>
struct display {
	void operator()(const T& x){
		std::cout << x << " ";
	}
};
template<typename T1, typename T2>
struct display_map {
	void operator()(const std::pair<T1, T2>& x){
		std::cout << x.first << ":" << x.second << std::endl;
	}
};
#endif
