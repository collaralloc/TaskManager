#pragma once
#include"PCB.h"
class TaskPrio
{
public:
	PCB* pr;	//正在运行的进程
	Plist* plist;//进程列表
	PCB* pf;	//完成列表
	TaskPrio();
	~TaskPrio();
	void myTask();
	void show(PCB* p);
};

