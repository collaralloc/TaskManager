#include "stdafx.h"
#include "PCB.h"
#include<cstdio>

PCB::PCB()
{
	puts("make new PCB class");
}


PCB::~PCB()
{
	PCB* p;
	while (next != NULL) {
		p = next;
		next = next->next;
		delete p;
	}
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
	p->need_time = rand()%10;
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
