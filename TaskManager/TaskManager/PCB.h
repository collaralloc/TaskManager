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
public:
	Plist();
	void insertP(PCB* p);//按进程优先数的顺序插入
	void insertR(PCB* p);//轮转法插入到就绪队列的队尾
	PCB* firstin();		//第一个进程投入运行
	void print();
	PCB* create();
	bool empty();
	PCB* getHead();
};
