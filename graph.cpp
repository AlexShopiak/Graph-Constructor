#include "framework.h"
#include "graph.h"

Graph::~Graph() {}

Graph* Graph:: makeGraph() 
{
	return new Graph;
}

int Graph::getMin() 
{
	return xMin; 
};

int Graph::getMax() 
{ 
	return xMax; 
};

COLORREF Graph::getColor() 
{ 
	return color; 
};

std::wstring Graph::getName()
{
	WCHAR name[50];
	const wchar_t* format = L"f(x) = %s";

	swprintf_s(name, 50, format, expression.c_str());
	return name;
}

std::wstring Graph::getExpression()
{ 
	return expression; 
};

void Graph::setProperties(std::wstring str, COLORREF clr, int min, int max)
{
	expression = str;
	color = clr;
	xMin = min;
	xMax = max;
};