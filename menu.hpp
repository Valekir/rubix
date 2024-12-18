#pragma once

#include <ncurses.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

void center_text(int y, int x, const std::string& text);

// Функция для чтения сохранений из файла
std::vector<std::string> load_saves();

bool toggle_checkbox(bool current);

std::string input(const std::string& prompt, const std::string& defaultValue);

void settings_menu();

void draw_menu(const std::string choices[], int highlight, int size);

int menu_control();

void save_menu();

int getColorInput(const std::string& prompt);
