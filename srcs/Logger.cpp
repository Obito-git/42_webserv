//
// Created by Anton on 28/08/2022.
//
#include "Logger.hpp"

const char *Logger::get_bg_value(Logger::BG_COLORS color) {
	switch (color) {
		case BG_BLACK:		return "40";
		case BG_RED:		return "41";
		case BG_GREEN:		return "42";
		case BG_YELLOW:		return "43";
		case BG_BLUE:		return "44";
		case BG_MAGENTA:	return "45";
		case BG_CYAN:		return "46";
		default:			return "47";
	}
}

const char *Logger::get_txt_value(Logger::TXT_COLORS color) {
	switch (color) {
		case TXT_BLACK:		return "30";
		case TXT_RED:		return "31";
		case TXT_GREEN:		return "32";
		case TXT_YELLOW:	return "33";
		case TXT_BLUE:		return "34";
		case TXT_MAGENTA:	return "35";
		case TXT_CYAN:		return "36";
		default:			return "37";
	}
}
