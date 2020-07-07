#include "stdafx.h"
#include "TaskPrio.h"
#include<cstdio>

TaskPrio::TaskPrio()
{
	pr = NULL;
	plist = new Plist();
}


TaskPrio::~TaskPrio()
{
}
void TaskPrio::show(PCB* p) {
	while (p != NULL) {
		printf("P%d=%d ",p->pid,p->prio);
		p = p->next;
	}
	puts("\n-------");
}

void TaskPrio::myTask() 
{
	for (int i = 1; i <= 10; i++) {
		PCB *p=plist->create();
		plist->insertP(p);
	}
	int i = 0;
	while (!plist->empty()) {
		pr = plist->firstin();
		pr->need_time -= 1;
		pr->prio -= 3;
		pr->cpu_time += 1;
		pr->need_time -= 1;
		if (pr->need_time <= 0) {
			pr->next = pf;
			pf = pr;
		}
		else plist->insertP(pr);
		show(plist->getHead());
		if (++i == 550) break;
	}
	show(pf);
}
