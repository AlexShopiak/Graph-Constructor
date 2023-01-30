#include "framework.h"
//#include <windows.h>
#include <atlbase.h>
#include "table.h"

HWND Table::hWndTable = NULL;

//Singleton
Table* Table::p_instance = 0;
Table* Table::getInstance() {
    if (!p_instance) {
        p_instance = new Table();
    }
    return p_instance;
}
Table::~Table() { };


INT_PTR CALLBACK TableDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDDELETE)
        {
            //Table::displayList(array);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Table::OnCreate(HWND hWnd, HINSTANCE hInst) {
    hWndTable = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TABLE), 0, TableDlg);
    ShowWindow(hWndTable, SW_SHOW);
}

void Table::Show() {
    ShowWindow(hWndTable, SW_SHOW);
    SetForegroundWindow(hWndTable);
}

void Table::drawTable(std::string* arr, int* clr) {
    USES_CONVERSION_EX;
    SendMessage(GetDlgItem(hWndTable, IDC_LIST1), LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < MAX_FUNNUMBER; i++) {
        if (arr[i].length() > 0) {
            std::string str = "y = " + arr[i];
            LPWSTR lp = A2W_EX(str.c_str(), str.length());
            SendMessage(GetDlgItem(hWndTable, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)lp);
        }
    }
}