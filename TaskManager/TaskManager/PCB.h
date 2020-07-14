#pragma once
class PCB
{
public:
	int pid;	//���̱�ʶ��
	int prio;	//����������
	int round;	//������תʱ��Ƭ
	int cpu_time;//����ռ��CPUʱ��
	int count;	//������,��¼ִ�д���
	int need_time;	//���̵���ɻ���Ҫ��CPUʱ��
	int arrive;	//���̴���ʱ��
	int status;	//״̬
	PCB* next;

	PCB();
	~PCB();
	void show();
};
class Plist {
	PCB* pcb;	//����
	PCB* tail;	//��β
	int pc;		//�������
	int length;	//������
public:
	Plist();
	void insertP(PCB* p);//��������������˳�����
	void insertR(PCB* p);//��ת�����뵽�������еĶ�β
	PCB* firstin();		//��һ������Ͷ������
	void print();		//��ӡ���
	PCB* create();		//�������һ������
	bool empty();		//���̶����Ƿ�Ϊ��
	PCB* getHead();		//�õ�����
	int size();			//���г���
};
