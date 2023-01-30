#pragma once
#include <string>
#include <cmath>
#include "editor.rh"
#include "graph.h"
#include "table.h"
#include "parser.h"

#define GRAPH_ARRAY_SIZE 10  //graph array size
#define MAX_ZOOM  20
#define MIN_ZOOM  1
#define DB_SIZE  256 //default buffer size

#define DEF_XMIN  "-300" //default CHAR xMin
#define DEF_XMAX  "300" //default CHAR xMax

#define DEF_GDEN  "12" //default CHAR grid density
#define DEF_DEN  12 //default INT grid density
#define MAX_DEN  16 //default INT grid density
#define MIN_DEN  1 //default INT grid density

#define WHITE_CLR   RGB(255, 255, 255)
#define GRAY_CLR    RGB(170, 170, 170)
#define BLACK_CLR   RGB(0, 0, 0)

#define RED_CLR     RGB(255, 0, 0)
#define ORANGE_CLR  RGB(255,174,66)
#define YELLOW_CLR  RGB(255,215,0)
#define GREEN_CLR   RGB(0, 138, 0)
#define BLUE_CLR    RGB(0, 0, 255)
#define PURPLE_CLR  RGB(75,0,130)
#define PINK_CLR    RGB(238,130,238)

class Editor {
private:
	static Editor* p_instance;
	Editor() {};
	Editor(const Editor&);
	Editor& operator = (Editor&);

	static HWND hWndEditor;

	Graph* pgr = NULL;
	Graph** pgraph = new Graph * [GRAPH_ARRAY_SIZE] {};

	int pos = 0;
	int zoom = 5;          //initial zoom
	int clWidth=0;         //var for client window width 
	int clHeight=0;        //var for client window height
	int isFirstPoint = 0;  //flag to be only one time

	static int xMin;         //x minimum
    static int xMax;         //x maximum
	static int gridDensity;  //number of lines on quadrant
	static COLORREF curColor;

	static LPCSTR path;
	static LPCSTR path2;
	FILE* fout;
public:
	~Editor();
	static Editor* getInstance();

	static HWND hWndParent;
	static string expr[GRAPH_ARRAY_SIZE];
	static COLORREF colors[GRAPH_ARRAY_SIZE];

	void OnCreate(HWND, HINSTANCE);
	void changePointer(Graph*);
	void Show();
	void incZoom();
	void decZoom();
	void setClientSize(HWND, int, int);
	void drawGrid(HWND, HDC);
	void drawAxes(HWND, HDC);
	void drawGraph(HDC);
	void exportList();

	static string wstrToStr(wstring);
	static string charToStr(char[]);
	void addGraph();
	static void updateParams(char*,char*,char*);
	static void updateWindow();
	static string* getArray();
	static int* getColors();
	static void clearArray();
	static COLORREF intToClr(int);
	static int clrToInt(COLORREF);
	
};