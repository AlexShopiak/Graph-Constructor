#pragma once
#include <string>
#include <fstream>
#include <codecvt>
#include "table.rh"

#define MAX_FUNNUMBER 10  //size of graphs array

class Table {
private:
	static Table* p_instance;
	Table() {};
	Table(const Table&);
	Table& operator = (Table&);

	static HWND hWndTable;
public:
	~Table();
	
	static Table* getInstance();

	void OnCreate(HWND, HINSTANCE);
	void Show();
	static void drawTable(std::string[], int[]);
};