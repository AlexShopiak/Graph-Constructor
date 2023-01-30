#include "framework.h"
#include "editor.h"

//Singleton
Editor* Editor::p_instance = 0;
Editor* Editor::getInstance() {
    if (!p_instance) {
        p_instance = new Editor();
    }
    return p_instance;
}
Editor::~Editor() {};

Parser* obj = Parser::getInstance();
string Editor::expr[] = {};
COLORREF Editor::colors[] = {};
HWND Editor::hWndEditor = NULL;
HWND Editor::hWndParent = NULL;
LPCSTR Editor::path = "table.txt";
LPCSTR Editor::path2 = "my_table.txt";
int Editor::xMax;
int Editor::xMin;
int Editor::gridDensity = DEF_DEN;
COLORREF Editor::curColor=0;

INT_PTR CALLBACK EditorDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    Editor* ped = Editor::getInstance(); //instance of Editor
    UNREFERENCED_PARAMETER(lParam);
    char fun_buffer[DB_SIZE];
    char min_buffer[DB_SIZE];
    char max_buffer[DB_SIZE];
    char grid_buffer[DB_SIZE];
    int color;
    int shift; //shift left from 1st empty lot
    switch (message)
    {
    case WM_INITDIALOG:
        EnableWindow(GetDlgItem(hDlg, IDREDRAW), false); //lock button 
        
        //filling combo box
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Black" );
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Red"   );
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Orange");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Yellow");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Green" );
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Blue"  );
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Purple");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)L"Pink"  );

        //inserting default
        SetDlgItemTextA(hDlg, IDC_EDIT2, DEF_XMIN);
        SetDlgItemTextA(hDlg, IDC_EDIT3, DEF_XMAX);
        SetDlgItemTextA(hDlg, IDC_EDIT4, DEF_GDEN);
        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SELECTSTRING, 0, (LPARAM)L"Red");

        return (INT_PTR)TRUE;
    case WM_MOUSEWHEEL:
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDDRAW)
        {
            shift = 0;

            GetDlgItemTextA(hDlg, IDC_EDIT1, fun_buffer, DB_SIZE);
            GetDlgItemTextA(hDlg, IDC_EDIT2, min_buffer, DB_SIZE);
            GetDlgItemTextA(hDlg, IDC_EDIT3, max_buffer, DB_SIZE);
            GetDlgItemTextA(hDlg, IDC_EDIT4, grid_buffer, DB_SIZE);
            color = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);

            Editor::updateParams(min_buffer, max_buffer, grid_buffer);
            //Editor::addExpression(fun_buffer, shift, color);
            //Editor::addGraph();
            if (ped) ped->addGraph();
            if (ped) ped->changePointer(new Graph);

            EnableWindow(GetDlgItem(hDlg, IDREDRAW), true);//unlock button 
            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDREDRAW)
        {           
            shift = 1; 

            GetDlgItemTextA(hDlg, IDC_EDIT1, fun_buffer, DB_SIZE);
            GetDlgItemTextA(hDlg, IDC_EDIT2, min_buffer, DB_SIZE);
            GetDlgItemTextA(hDlg, IDC_EDIT3, max_buffer, DB_SIZE); 
            GetDlgItemTextA(hDlg, IDC_EDIT4, grid_buffer, DB_SIZE);
            color = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);           

            Editor::updateParams(min_buffer, max_buffer, grid_buffer);
            //Editor::addExpression(fun_buffer, shift, color); 

            return (INT_PTR)TRUE;
        }
        if (LOWORD(wParam) == IDCLEAR)
        {
            Editor::clearArray();
            EnableWindow(GetDlgItem(hDlg, IDREDRAW), false); //lock button 
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Editor::OnCreate(HWND hWnd, HINSTANCE hInst) {
    hWndParent = hWnd;
    hWndEditor = CreateDialog(hInst, MAKEINTRESOURCE(IDD_EDITOR), 0, EditorDlg);
    ShowWindow(hWndEditor, SW_SHOW);

    fopen_s(&fout, path, "wt");
    if (fout) {
        fprintf(fout, "");
        fclose(fout);
    }
}

void Editor::changePointer(Graph* graph) {
    if (pgr) delete pgr;
    pgr = graph;
    pgraph[pos] = pgr->makeGraph();
    pgraph[pos]->setProperties(L"2*x", RED_CLR, -300, 300);
    updateWindow();
};


void Editor::incZoom() {
    if (zoom < MAX_ZOOM)zoom++;
}

void Editor::decZoom() {
    if(zoom > MIN_ZOOM)zoom--;
}

void Editor::Show() {
    ShowWindow(hWndEditor, SW_SHOW);
    SetForegroundWindow(hWndEditor);
}

void Editor::setClientSize(HWND hWnd, int width, int height) { 
    RECT rcClient, rcWind;
    POINT ptDiff;

    clWidth = width;
    clHeight = height;

    GetClientRect(hWnd, &rcClient);
    GetWindowRect(hWnd, &rcWind);
    ptDiff.x = (rcWind.right - rcWind.left) - rcClient.right;
    ptDiff.y = (rcWind.bottom - rcWind.top) - rcClient.bottom;

    MoveWindow(hWnd, rcWind.left, rcWind.top, width + ptDiff.x, height + ptDiff.y, TRUE);
};

void Editor::drawGrid(HWND hWnd, HDC hdc) {

    HPEN hPen = CreatePen(PS_SOLID, 1, GRAY_CLR);  //create gray pen
    HPEN hPenOld = (HPEN)SelectObject(hdc, hPen); //save previous pen

    HFONT hFont = CreateFont(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, NULL); //create the smallest font
    HFONT hTmp = (HFONT)SelectObject(hdc, hFont); //save previous font

    int den = gridDensity * 2; //number of lines in the grid must be even
    wchar_t xFrac[DB_SIZE];
    wchar_t yFrac[DB_SIZE];

    for (int i = 1; i < den; i++) 
    {
        int xCord = round(clWidth * (den - i) / den);
        int yCord = round(clHeight * (den - i) / den);

        swprintf_s(xFrac, L"%d", (clWidth*(den-i)/den-300)/zoom );
        swprintf_s(yFrac, L"%d", (-clHeight*(den-i)/den+300)/zoom );

        TextOut(hdc, xCord, clHeight/2, xFrac, wcslen(xFrac));
        TextOut(hdc, clWidth/2, yCord, yFrac, wcslen(yFrac));

        MoveToEx(hdc, xCord, 0, NULL);
        LineTo(hdc, xCord, clHeight);

        MoveToEx(hdc, 0, yCord, NULL);
        LineTo(hdc, clWidth, yCord);
    }

    SelectObject(hdc, hPenOld); //select default pen
    DeleteObject(hPen);         //delete pen

    SelectObject(hdc, hTmp);   //select default font
    DeleteObject(hFont);       //delete font
};

void Editor::drawAxes(HWND hWnd, HDC hdc) {
    MoveToEx(hdc, 0, clHeight/2, NULL);
    LineTo(hdc, clWidth, clHeight/2);

    MoveToEx(hdc, clWidth/2, 0, NULL);
    LineTo(hdc, clWidth/2, clHeight);
};

void Editor::drawGraph(HDC hdc) {
    if (!pgr) { return; }
    
    wstring expression = pgraph[pos]->getExpression();
    COLORREF clr = pgraph[pos]->getColor();
    int min = pgraph[pos]->getMin();
    int max = pgraph[pos]->getMax();

   HPEN hPen = CreatePen(PS_SOLID, 1, clr); //create current color
   SelectObject(hdc, hPen); //select current

   char xvalue[DB_SIZE];
   char expstr[DB_SIZE];

   string str = wstrToStr(expression);
   strcpy_s(expstr, str.c_str()); //str ->char[]

   for (double x = min; x <= max; x += 0.5) {
      string s = "x=" + to_string(x); //double ->str
      strcpy_s(xvalue, s.c_str()); //cstr ->char[]

      obj->eval_exp(xvalue);                 //parce and calculte X
      double answer = obj->eval_exp(expstr); //parce and calculate Y

      int xCord = round(zoom * x + clWidth / 2);
      int yCord = round(zoom * answer * (-1) + clHeight / 2);
      LineTo(hdc, xCord, yCord);
   }
   DeleteObject(hPen);
};

string Editor::wstrToStr(wstring wstr) {
    std::wstring string_to_convert = wstr;
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string converted_str = converter.to_bytes(string_to_convert);
    return converted_str;
}

string Editor::charToStr(char* buffer) {
    string s = "";
    int size = sizeof(buffer) / sizeof(char);
    for (int i = 0; i < size; i++) {
        s = s + buffer[i];
    }
    return s;
}

void Editor::addGraph() {
    //string s = charToStr(buffer);
    ////search the 1st empty lot
    //for (int i = 0; i < GRAPH_ARRAY_SIZE; i++) {
    //    if (expr[i].length() == 0) { 
    //        expr[i-shift] = s; 
    //        colors[i - shift] = intToClr(clr);
    //        return;
    //    }
    //}
    updateWindow();
}

COLORREF Editor::intToClr(int clr) {
    if      (clr == 0) { return BLACK_CLR;  }
    else if (clr == 1) { return RED_CLR;    }
    else if (clr == 2) { return ORANGE_CLR; }
    else if (clr == 3) { return YELLOW_CLR; }
    else if (clr == 4) { return GREEN_CLR;  }
    else if (clr == 5) { return BLUE_CLR;   }
    else if (clr == 6) { return PURPLE_CLR; }
    else if (clr == 7) { return PINK_CLR;   }
} 

int Editor::clrToInt(COLORREF clr) {
    if      (clr == BLACK_CLR ) { return 0; }
    else if (clr == RED_CLR   ) { return 1; }
    else if (clr == ORANGE_CLR) { return 2; }
    else if (clr == YELLOW_CLR) { return 3; }
    else if (clr == GREEN_CLR ) { return 4; }
    else if (clr == BLUE_CLR  ) { return 5; }
    else if (clr == PURPLE_CLR) { return 6; }
    else if (clr == PINK_CLR  ) { return 7; }
}

void Editor::updateParams(char* min, char* max, char* den) {
    xMin = atoi(min);
    xMax = atoi(max);
    int gDen = atoi(den);
    if (gDen > MIN_DEN && gDen < MAX_DEN) { gridDensity = gDen; }
}

string* Editor::getArray() {
    return expr;
};

int* Editor::getColors() {
    int clrs[GRAPH_ARRAY_SIZE];
    for (int i = 0; i < GRAPH_ARRAY_SIZE; i++) {
        clrs[i] = clrToInt(colors[i]);
    }
    return clrs;
};

void Editor::clearArray() {
    for (int i = 0; i < GRAPH_ARRAY_SIZE; i++) expr[i] = "";
    updateWindow();
};

void Editor::updateWindow() { 
    InvalidateRect(hWndParent, NULL, TRUE); 
};

void Editor::exportList() {
    for (int i = 0; i < GRAPH_ARRAY_SIZE; i++) {
        if (expr[i] != "") {

            //std::wstring widestr = std::wstring(expr[i].begin(), expr[i].end());
            //const wchar_t* widecstr = widestr.c_str();

            fopen_s(&fout, path, "a, ccs=UTF-8");
            if (fout) {
                fwprintf(fout, L"%s\n", expr[i].c_str());
                //fwprintf(fout, L"y = %s\n", widecstr);
                fclose(fout);
            }
        }
    }
};