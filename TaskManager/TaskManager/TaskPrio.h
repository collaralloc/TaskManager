#pragma once
#include"PCB.h"
class TaskPrio
{
public:
	PCB* pr;	//�������еĽ���
	Plist* plist;//�����б�
	PCB* pf;	//����б�
	TaskPrio();
	~TaskPrio();
	void myTask();
	void show(PCB* p);
};

