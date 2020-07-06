#pragma once
class PCB
{
public:
	int pid;	//进程标识符
	int prio;	//进程优先数
	int round;	//进程轮转时间片
	int cpu_time;//进程占用CPU时间
	int count;	//计数器
	int need_time;	//进程到完成还需要的CPU时间
	int status;	//状态
	PCB* next;


	PCB();
	~PCB();
	void show();
	

};
class Plist {
	PCB* pcb;	//队首
	int pc;		//自增编号
	int length;	//进程数
	Plist();
	void insertP(PCB* p);
	void insertR(PCB* p);
	void firstin();
	void print();
	PCB* create();
};
