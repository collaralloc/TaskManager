#include "stdafx.h"
#include "PCB.h"
#include<cstdio>

PCB::PCB()
{
	puts("make new PCB class");
}


PCB::~PCB()
{
}

void PCB::show()
{
	printf("PCB show\n");
}
Plist::Plist() {
	pc = 0;
	length = 0;
	pcb = NULL;
}
PCB* Plist::create() {
	PCB *p = new PCB();
	p->pid = ++pc;
	p->prio = rand()%100;
	p->round = rand();
	p->need_time = rand()%500;
	p->cpu_time = 0;
	p->status = 0;
	p->next = NULL;
	return p;
}
void Plist::insertP(PCB* p) {
	length++;
	if (pcb == NULL) {
		pcb = p;
		return;
	}

	while (pcb->next != NULL) {
	}
}
