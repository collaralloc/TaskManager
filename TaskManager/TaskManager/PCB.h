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
public:
	Plist();
	void insertP(PCB* p);//��������������˳�����
	void insertR(PCB* p);//��ת�����뵽�������еĶ�β
	PCB* firstin();		//��һ������Ͷ������
	void print();
	PCB* create();
	bool empty();
	PCB* getHead();
};
