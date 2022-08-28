//
// Created by Anton on 28/08/2022.
//

#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP
#include <iostream>

class Logger {
#define RESET "\x1b[0m"
#define COLOR_START "\x1b["

public:
	enum BG_COLORS {
		BG_BLACK = 40,
		BG_RED,
		BG_GREEN,
		BG_YELLOW,
		BG_BLUE,
		BG_MAGENTA,
		BG_CYAN,
		BG_WHITE
	};
	
	enum TXT_COLORS {
		TXT_BLACK = 30,
		TXT_RED,
		TXT_GREEN,
		TXT_YELLOW,
		TXT_BLUE,
		TXT_MAGENTA,
		TXT_CYAN,
		TXT_WHITE
	};
	
	/********************************** ONE PARAMETER NEW LINE ****************************************/

	template<typename T>
	static void println(const T &t) {
		print(t);
		std::cout << std::endl;
	}
	
	template<typename T>
	static void println(TXT_COLORS txt_color, const T &t) {
		print(txt_color, t);
		std::cout << std::endl;
	}
	
	template<typename T>
	static void println(TXT_COLORS txt_color, BG_COLORS bg_color, const T &t) {
		print(txt_color, bg_color, t);
		std::cout << std::endl;
	}
	
	template<typename T>
	 static void println(BG_COLORS bg_color, const T &t) {
		print(bg_color, t);
		std::cout << std::endl;
	}
	
	/********************************** TWO PARAMETERS NEW LINE ****************************************/

	template<typename T, typename X>
	static void println(const T &t, const X &x) {
		print(t, x);
		std::cout << std::endl;
	}
	
	template<typename T, typename X>
	static void println(TXT_COLORS txt_color, const T &t, const X &x) {
		print(txt_color, t, x);
		std::cout << std::endl;
	}

	template<typename T, typename X>
	static void println(TXT_COLORS txt_color, BG_COLORS bg_color, const T &t, const X &x) {
		print(txt_color, bg_color, t, x);
		std::cout << std::endl;
	}

	template<typename T, typename X>
	static void println(BG_COLORS bg_color, const T &t, const X &x) {
		print(bg_color, t, x);
		std::cout << std::endl;
	}
	
	/********************************** THREE PARAMETERS NEW LINE ****************************************/

	template<typename T, typename X, typename U>
	static void println(const T &t, const X &x, const U &u) {
		print(t, x, u);
		std::cout << std::endl;
	}
	
	template<typename T, typename X, typename U>
	static void println(TXT_COLORS txt_color, const T &t, const X &x, const U &u) {
		print(txt_color, t, x, u);
		std::cout << std::endl;
	}

	template<typename T, typename X, typename U>
	static void println(TXT_COLORS txt_color, BG_COLORS bg_color, const T &t, const X &x, const U &u) {
		print(txt_color, bg_color, t, x, u);
		std::cout << std::endl;
	}

	template<typename T, typename X, typename U>
	static void println(BG_COLORS bg_color, const T &t, const X &x, const U &u) {
		print(bg_color, t, x, u);
		std::cout << std::endl;
	}

	/********************************** ONE PARAMETER ****************************************/

	template<typename T>
	static void print(const T &t) {
		std::cout << t;
	}
	
	template<typename T>
	static void print(TXT_COLORS txt_color, const T &t) {
		std::cout << COLOR_START << get_txt_value(txt_color) << "m" << t << RESET;
	}

	template<typename T>
	static void print(TXT_COLORS txt_color, BG_COLORS bg_color, const T &t) {
		std::cout << COLOR_START << get_txt_value(txt_color) << ";"
				  << get_bg_value(bg_color) << "m" << t << RESET;
	}

	template<typename T>
	static void print(BG_COLORS bg_color, const T &t) {
		std::cout << COLOR_START << get_bg_value(bg_color) << "m" << t << RESET;
	}

	/********************************** TWO PARAMETERS ****************************************/

	template<typename T, typename X>
	static void print(const T &t, const X &x) {
		std::cout << t << " " << x;
	}
	
	template<typename T, typename X>
	static void print(TXT_COLORS txt_color, const T &t, const X &x) {
		std::cout << COLOR_START << get_txt_value(txt_color) << "m" << t << " " << x << RESET;
	}

	template<typename T, typename X>
	static void print(TXT_COLORS txt_color, BG_COLORS bg_color, const T &t, const X &x) {
		std::cout << COLOR_START << get_txt_value(txt_color) << ";"
				  << get_bg_value(bg_color) << "m" << t << " " << x << RESET;
	}

	template<typename T, typename X>
	static void print(BG_COLORS bg_color, const T &t, const X &x) {
		std::cout << COLOR_START << get_bg_value(bg_color) << "m" << t << " " << x << RESET;
	}

	/********************************** THREE PARAMETERS ****************************************/
	
	template<typename T, typename X, typename U>
	static void print(const T &t, const X &x, const U &u) {
		std::cout << t << " " << x << " " << u;
	}
	
	template<typename T, typename X, typename U>
	static void print(TXT_COLORS txt_color, const T &t, const X &x, const U &u) {
		std::cout << COLOR_START << get_txt_value(txt_color) << "m" << t << " "
				  << x << " " << u << RESET;
	}

	template<typename T, typename X, typename U>
	static void print(TXT_COLORS txt_color, BG_COLORS bg_color, const T &t, const X &x, const U &u) {
		std::cout << COLOR_START << get_txt_value(txt_color) << ";"
				  << get_bg_value(bg_color) << "m" << t << " " << x << " " << u << RESET;
	}

	template<typename T, typename X, typename U>
	static void print(BG_COLORS bg_color, const T &t, const X &x, const U &u) {
		std::cout << COLOR_START << get_bg_value(bg_color) << "m" << t << " " << x << " " << u << RESET;
	}
	
private:
	static const char* get_bg_value(BG_COLORS color);
	static const char *get_txt_value(TXT_COLORS color);
};

#endif //WEBSERV_LOGGER_HPP
