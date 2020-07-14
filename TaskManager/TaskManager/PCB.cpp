#include "stdafx.h"
#include "PCB.h"
#include<cstdio>

PCB::PCB()
{
	puts("make new PCB class");
	cpu_time = 0;
	arrive = 0;
	status = 0;
	next = NULL;
}


PCB::~PCB()
{
	PCB* p;
	while (next != NULL && next != 0) {
		p = next;
		next = next->next;
		delete p;
	}
	puts("delete PCB class");
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
	p->prio = rand() % 100;
	p->round = rand() % 100;
	p->need_time = rand() % 10;

	return p;
}
void Plist::insertP(PCB* p) {
	length++;
	if (pcb == NULL) {
		pcb = p;
		return;
	}
	if (p->prio > pcb->prio) {
		p->next = pcb;
		pcb = p;
		return;
	}
	PCB* tp = pcb;
	while (tp->next != NULL && tp->next->prio > p->prio) {
		tp = tp->next;
	}
	p->next = tp->next;
	tp->next = p;
}

void Plist::insertR(PCB* p) {
	length++;
	if (pcb == NULL) {
		pcb = p;
		tail = p;
		return;
	}
	tail->next = p;
	tail = p;
}

PCB* Plist::firstin() {
	PCB *p = pcb;
	length--;
	pcb = pcb->next;
	p->next = NULL;
	return p;
}
PCB* Plist::getHead() {
	return pcb;
}
bool Plist::empty() {
	return (length==0);
}
int Plist::size() {
	return length;
}
