// TaskManager.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "TaskManager.h"
#include <cstdio>
#include "TaskPrio.h"
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define MAX_LOADSTRING 100
// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

int algo_type;		//算法类型
int cpu_clock=0;		//cpu时钟
bool timectrl=false;//时钟控制
HWND clk;			//时钟显示
Plist task;			//进程列表
PCB* pReady;			//预备缓存
PCB* pf;			//完成列表
HWND hReady;		//预备队列
HWND hRun;			//运行队列
HWND hFinish;		//完成队列
HFONT font;
// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);
    // TODO: 在此处放置代码。
	AllocConsole();
	 
	FILE *stream;
	freopen_s(&stream,"CONOUT$", "w+t", stdout);// 申请写
	//_tfreopen(_T("CONOUT$"), _T("w+t"), stdout);
	//_tfreopen(_T("CONIN$"), _T("r+t"), stdin);
	//_tsetlocale(LC_ALL, _T("chs"));
	_tprintf(_T("开始消息循环!\n"));
	printf("MBCS 开始消息循环!\n");
	wprintf(L"UNICODE 开始消息循环!\n");
	
	pReady = new PCB();
	pReady->next = pReady;
	pReady->pid = -1;
	//task.myTask();
	font = CreateFont(-28/*高*/, -14/*宽*/, 0, 0, 400 /*400表示正常字体*/, FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("微软雅黑"));
	

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TASKMANAGER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TASKMANAGER));

    MSG msg;
	

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		//printf("%d\n",GetTickCount());
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TASKMANAGER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TASKMANAGER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

int char2int(TCHAR* p) {
	int x = 0;
	while (*p >=L'0' && *p<=L'9') {
		x = x * 10 + (*p) - L'0';
		p++;
	}
	return x;
}

void EnumProcess(int md) {
	LV_ITEM vitem;

	//初始化                
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	if (md == 1) {
		
		ListView_DeleteItem(hRun, 0);
		return;
	}
	if (md == 0) {
		vitem.iItem = 0;                //行                
		vitem.iSubItem = 0;                //列
		ListView_InsertItem(hRun, &vitem);    //一个宏和SendMessage作用一样
	}

	WCHAR buf[16];
	PCB* head = task.getHead();
	int i = 0;
	while (head != NULL) {
		swprintf(buf, 16, L"P%d", head->pid);
		vitem.pszText = buf;    //值                
		vitem.iItem = i;                //行                
		vitem.iSubItem = 0;                //列
		ListView_SetItem(hRun, &vitem);

		swprintf(buf, 16, L"%d", head->prio);
		vitem.pszText = buf;    //值                
		vitem.iItem = i;                //行                
		vitem.iSubItem = 1;                //列
		ListView_SetItem(hRun, &vitem);

		swprintf(buf, 16, L"%d", head->arrive);
		vitem.pszText = buf;    //值                
		vitem.iItem = i;                //行                
		vitem.iSubItem = 2;                //列
		ListView_SetItem(hRun, &vitem);

		swprintf(buf, 16, L"%d", head->cpu_time);
		vitem.pszText = buf;    //值                
		vitem.iItem = i;                //行                
		vitem.iSubItem = 3;                //列
		ListView_SetItem(hRun, &vitem);

		swprintf(buf, 16, L"%d", head->need_time);
		vitem.pszText = buf;    //值                
		vitem.iItem = i;                //行                
		vitem.iSubItem = 4;                //列
		ListView_SetItem(hRun, &vitem);

		if (head->status==0) vitem.pszText = (WCHAR*)L"waiting";
		else if (head->status==1) vitem.pszText = (WCHAR*)L"running";
		vitem.iItem = i;                //行                
		vitem.iSubItem = 5;                //列
		ListView_SetItem(hRun, &vitem);

		head = head->next;
		i++;
	}

}


//初始化进程窗口，就是给ListView控件添加列
HWND initProcessView(HWND hDlg,int id) {
	LV_COLUMN lv;
	HWND hListProcess;

	//初始化，局部变量堆栈中分配，不知道是什么数据所以先清零                                
	memset(&lv, 0, sizeof(LV_COLUMN));
	//获取IDC_LIST_PROCESS句柄                                
	hListProcess = GetDlgItem(hDlg, id);
	//设置整行选中，窗口是windows来管理的无法直接操作，程序能做的只能发送一个消息来让windows直到该怎么做                                
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	//第一列                                
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	WCHAR header[6][8] = { {L"进程名"},{L"优先级"},
	{L"创建时间"},{L"已运行时间"},{L"所需时间"},{L"进程状态"}
	};
	for (int i = 0; i < 6; i++) {
		lv.pszText = header[i];
		lv.cx = 90;
		lv.iSubItem = i;
		ListView_InsertColumn(hListProcess, i, &lv);
		if (i == 3 && id == 51003) {
			lv.pszText = header[5];
			lv.cx = 90;
			lv.iSubItem = 4;
			ListView_InsertColumn(hListProcess, 4, &lv);
			break;
		}
	}
	return hListProcess;
}

HWND initListView(HWND hDlg) {
	LV_COLUMN lv;
	HWND hListProcess;

	//初始化，局部变量堆栈中分配，不知道是什么数据所以先清零                                
	memset(&lv, 0, sizeof(LV_COLUMN));
	//获取IDC_LIST_PROCESS句柄                                
	hListProcess = GetDlgItem(hDlg, 51002);
	//设置整行选中，窗口是windows来管理的无法直接操作，程序能做的只能发送一个消息来让windows直到该怎么做                                
	SendMessage(hListProcess, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	WCHAR header[4][8] = { {L"进程名"},
	{L"创建时间"},{L"所需时间"}
	};
	for (int i = 0; i < 3; i++) {
		lv.pszText = header[i];
		lv.cx = 100;
		lv.iSubItem = i;
		ListView_InsertColumn(hListProcess, i, &lv);
	}
	return hListProcess;
}

void updateReady(int md) {
	LV_ITEM vitem;

	//初始化                
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	if (md == 1){
		//puts("delete 行");
		ListView_DeleteItem(hReady, 0);
		return;
	}
	
	int itemNum = ListView_GetItemCount(hReady);
	//ListView_InsertColumn(hReady, itemNum, "12",5,6);
	
	WCHAR buf[16];
	swprintf(buf,16,L"P%d",pReady->pid);
	vitem.pszText = buf;
	vitem.iItem = itemNum;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);                
	SendMessage(hReady, LVM_INSERTITEM, 0, (DWORD)&vitem);

	swprintf(buf, 16, L"%d", pReady->arrive);
	vitem.pszText = buf;
	vitem.iSubItem = 1;
	ListView_SetItem(hReady, &vitem);

	swprintf(buf, 16, L"%d", pReady->need_time);
	vitem.pszText = buf;
	vitem.iSubItem = 2;
	ListView_SetItem(hReady, &vitem);
}
void updateFinish(int md) {
	LV_ITEM vitem;

	//初始化                
	memset(&vitem, 0, sizeof(LV_ITEM));
	vitem.mask = LVIF_TEXT;

	if (md == 1) {
		//puts("delete 行");
		ListView_DeleteItem(hFinish, 0);
		return;
	}

	int itemNum = ListView_GetItemCount(hFinish);
	//ListView_InsertColumn(hReady, itemNum, "12",5,6);

	WCHAR buf[16];
	swprintf(buf, 16, L"P%d", pf->pid);
	vitem.pszText = buf;
	vitem.iItem = itemNum;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);                
	SendMessage(hFinish, LVM_INSERTITEM, 0, (DWORD)&vitem);

	swprintf(buf, 16, L"%d", pf->prio);
	vitem.pszText = buf;
	vitem.iSubItem = 1;
	ListView_SetItem(hFinish, &vitem);

	swprintf(buf, 16, L"%d", pf->arrive);
	vitem.pszText = buf;
	vitem.iSubItem = 2;
	ListView_SetItem(hFinish, &vitem);

	swprintf(buf, 16, L"%d", pf->cpu_time);
	vitem.pszText = buf;
	vitem.iSubItem = 3;
	ListView_SetItem(hFinish, &vitem);

	swprintf(buf, 16, L"%d", pf->need_time);
	vitem.pszText = buf;
	vitem.iSubItem = 4;
	ListView_SetItem(hFinish, &vitem);

	vitem.pszText = (WCHAR*)L"Finish";
	vitem.iSubItem = 4;
	ListView_SetItem(hFinish, &vitem);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_INITDIALOG:
		
		printf("init view");
		break;
	case WM_CREATE:
		SetTimer(hWnd, 1, 500, NULL);
		CreateWindowEx(0,  //扩展风格  
			WC_LISTVIEW, //这是系统定义的宏，WC_LISTVIEW对应 "SysListView32"
			L"123", //窗口标题  
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN   | LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_SINGLESEL , //窗口风格 （普通控件风格在这里设置，扩展风格要单独设置） 
			470,   40, //窗口大小   
			540, //宽度一定要等于所有列宽的和，否则会看起来有空列 
			200, hWnd, //父窗口句柄  
			(HMENU)51001, //菜单句柄 hInstance, 
			hInst,	  //实例句柄 
			NULL //创建参数 
		);
		hRun=initProcessView(hWnd,51001);
		CreateWindowEx(0,  //扩展风格  
			WC_LISTVIEW, //这是系统定义的宏，WC_LISTVIEW对应 "SysListView32"
			L"1234", //窗口标题  
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_SINGLESEL, //窗口风格 （普通控件风格在这里设置，扩展风格要单独设置） 
			470, 240, //窗口大小   
			540, //宽度一定要等于所有列宽的和，否则会看起来有空列 
			230, hWnd, //父窗口句柄  
			(HMENU)51003, //菜单句柄 hInstance, 
			hInst,	  //实例句柄 
			NULL //创建参数 
		);
		hFinish = initProcessView(hWnd, 51003);
		CreateWindowEx(0,  //扩展风格  
			WC_LISTVIEW, //这是系统定义的宏，WC_LISTVIEW对应 "SysListView32"
			L"Pre", //窗口标题  
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_SINGLESEL, //窗口风格 （普通控件风格在这里设置，扩展风格要单独设置） 
			30, 240, //窗口大小   
			300, //宽度一定要等于所有列宽的和，否则会看起来有空列 
			230, hWnd, //父窗口句柄  
			(HMENU)51002, //菜单句柄 hInstance, 
			hInst,	  //实例句柄 
			NULL //创建参数 
		);
		hReady=initListView(hWnd);

		CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
			110, 60, 90, 20, hWnd, (HMENU)4001, hInst, nullptr);
		CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
			210, 60, 90, 20, hWnd, (HMENU)4002, hInst, nullptr);
		CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
			310, 60, 90, 20, hWnd, (HMENU)4003, hInst, nullptr);
		CreateWindow(_T("button"), _T("新建进程"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 10, 10, 400, 500, hWnd, (HMENU)5002, hInst, nullptr);
		CreateWindow(_T("button"), _T("系统进程状态"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 420, 10, 600, 500, hWnd, (HMENU)5001, hInst, nullptr);
		CreateWindow(_T("button"), _T("自动生成"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 30, 80, 20, hWnd, (HMENU)5003, hInst, nullptr);
		CreateWindow(_T("button"), _T("手动添加"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 20, 60, 80, 20, hWnd, (HMENU)5004, hInst, nullptr);

		CreateWindow(_T("button"), _T("优先级优先"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON |WS_GROUP, 20, 90, 100, 20, hWnd, (HMENU)5005, hInst, nullptr);
		CreateWindow(_T("button"), _T("时间片轮转"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 130, 90, 100, 20, hWnd, (HMENU)5006, hInst, nullptr);
		CreateWindow(_T("button"), _T("先来先服务"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 240, 90, 100, 20, hWnd, (HMENU)5007, hInst, nullptr);

		CreateWindow(_T("button"), _T("启动时钟"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_GROUP, 20, 120, 80, 20, hWnd, (HMENU)5008, hInst, nullptr);
		CreateWindow(_T("button"), _T("暂停时钟"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 130, 120, 80, 20, hWnd, (HMENU)5009, hInst, nullptr);
		CreateWindow(_T("button"), _T("清空进程"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 240, 120, 80, 20, hWnd, (HMENU)5010, hInst, nullptr);


		clk=CreateWindow(_T("static"), _T("CLOCK"), WS_CHILD | WS_VISIBLE, 30, 160, 140, 40, hWnd, (HMENU)3010, hInst, nullptr);
		SendMessage(clk, WM_SETFONT,
			(WPARAM)font
			, NULL);
		break;

	case WM_KEYDOWN: //键盘按下
		//MessageBox(hWnd, TEXT("键盘按下"), TEXT("键盘"), MB_OK);
		
		break;
	case WM_RBUTTONDOWN:
		//printf("wmID=%d\n", LOWORD(wParam));
		//MessageBox(hWnd, TEXT("右键点击预备列表则删除\n点击就绪队列则阻塞\n点击阻塞则就绪"), TEXT("右键点击"), MB_OK);

		break;
	case WM_TIMER:{
		WCHAR buf[16];
		swprintf(buf, 16, L"TIME:%d", cpu_clock);
		SetWindowText(clk, buf);
		SendMessage(clk, WM_SETFONT,(WPARAM)font, NULL);
		if (timectrl) { 
			 
			PCB* p = pReady->next->next;//获得第一个
			if (p->pid > 0) {
				printf("clk=%d  arr=%d\n", cpu_clock, p->arrive);
				if (cpu_clock == p->arrive) {
					pReady->next->next = p->next; //更新队首
					if (pReady->pid == p->pid) pReady = pReady->next;
					p->next = NULL;		//删除第一个
					if (algo_type == 0)task.insertP(p);
					else if (algo_type == 1) task.insertR(p);
					else if (algo_type == 2) task.insertR(p);
					
					updateReady(1);	//删除
					//puts("ADDDColumn");
					EnumProcess(0);	//添加
					//puts("-------------deleteColumn");
					
				}
				
			}
			
			cpu_clock++;
			
			if (task.empty()) break; //为空退出
			//运行进程
			PCB* pr;
			if (algo_type == 0) {//优先级
				pr=task.firstin();
				pr->need_time -= 1;
				pr->prio -= 3;
				pr->cpu_time += 1;
				pr->status = 0;	//waiting
				if (pr->need_time <= 0) {
					pr->next = pf;
					pf = pr;
					EnumProcess(1); //delete
					updateFinish(0);
				}
				else task.insertP(pr);
				
			}
			else if (algo_type == 1 && cpu_clock%2==0) {//时间片
				pr = task.firstin();
				pr->need_time -= 2;
				pr->cpu_time += 2;
				pr->status = 0;	//waiting
				if (pr->need_time <= 0) {
					pr->next = pf;
					pf = pr;
					EnumProcess(1); //delete
					updateFinish(0);
				}
				else task.insertR(pr);
			}
			else if (algo_type == 2) {//FIFO
				pr = task.getHead();
				pr->need_time -= 1;
				pr->cpu_time += 1;
				pr->status = 0;	//waiting
				if (pr->need_time <= 0) {
					pr = task.firstin();	//remove
					pr->next = pf;
					pf = pr;
					EnumProcess(1); //delete
					updateFinish(0);
					
				}
				
			}
			if (task.empty()) break; //为空退出
			pr = task.getHead();
			pr->status = 1; //running
			if (pr->next != NULL)pr->next->status = 0;
			EnumProcess(2);	//更新
			
		}
		//printf("%d\n",cpu_clock);
		break;
	}
		
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			PCB* p;
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case 5003:
				//MessageBox(hWnd, TEXT("click new button"), TEXT("click"), MB_OK);
				p = task.create();
				p->arrive = rand()%5+max(cpu_clock,pReady->arrive)+1;
				p->next = pReady->next; //指向队首
				pReady->next = p;		//队尾追加
				pReady = p;			//更新队尾
				updateReady(0);
				break;
			case 5004:
				TCHAR szText1[512];
				p = task.create();
				GetDlgItemText(hWnd,4001, (LPTSTR)szText1, 512);
				p->prio = char2int(szText1);
				GetDlgItemText(hWnd, 4002, (LPTSTR)szText1, 512);
				p->round = char2int(szText1);
				GetDlgItemText(hWnd, 4003, (LPTSTR)szText1, 512);
				p->need_time = char2int(szText1);
				p->arrive = rand() % 5 + max(cpu_clock, pReady->arrive);
				p->next = pReady->next; //指向队首
				pReady->next = p;		//队尾追加
				pReady = p;			//更新队尾
				updateReady(0);
				break;
			case 5005:
			case 5006:
			case 5007:
				algo_type = wmId - 5005;
				break;
			case 5008:
				timectrl = true;
				break;
			case 5009:
				timectrl = false;
				break;
			case 5010:
				timectrl = false;
				cpu_clock = 0;
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			TextOut(hdc, 430, 30, TEXT("等待队列"), 2);
			TextOut(hdc, 430, 230, TEXT("完成队列"), 2);
			TextOut(hdc, 110, 40, TEXT("优先级"), 3);
			TextOut(hdc, 210, 40, TEXT("时间片数"), 4);
			TextOut(hdc, 310, 40, TEXT("所需时间"), 4);
			
			printf("paint\n");
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		KillTimer(hWnd,1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
