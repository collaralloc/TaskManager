#pragma once
class PCB
{
public:
	int pid;	//���̱�ʶ��
	int prio;	//����������
	int round;	//������תʱ��Ƭ
	int cpu_time;//����ռ��CPUʱ��
	int count;	//������
	int need_time;	//���̵���ɻ���Ҫ��CPUʱ��
	int status;	//״̬
	PCB* next;


	PCB();
	~PCB();
	void show();
	

};
class Plist {
	PCB* pcb;	//����
	int pc;		//�������
	int length;	//������
	Plist();
	void insertP(PCB* p);
	void insertR(PCB* p);
	void firstin();
	void print();
	PCB* create();
};
