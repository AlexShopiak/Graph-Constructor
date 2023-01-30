#pragma once
#include <string>

class Graph {
private:
	std::wstring expression;
	COLORREF color;
	int xMin;
	int xMax;
public:
	 ~Graph();
	Graph* makeGraph();
	
	int getMin();
	int getMax();
	COLORREF getColor();
	std::wstring getName();
	std::wstring getExpression();

	void setProperties(std::wstring, COLORREF, int,int);
};