
#include "PFilter.h"
#include "Histogram.h"


Histogram::Histogram(int b, int d)
{
	bins = b;
	dimension = d;
	//Bin���~�������̗̈�m�ہ@�O�ŏ�����
	for(int i = 0;i < b*d; i++){
		val.push_back(0);
	}
}


Histogram::~Histogram(void)
{
}


int Histogram::getVal(int dim, int x)
{
	int binVal = val[bins*(dim-1) + x - 1];
	printf_s("%d",binVal);
	return binVal;
}


void Histogram::setVal(int b, int v)
{
	vector<int>::iterator it = val.begin();  // �C�e���[�^�̃C���X�^���X��
	for(int i=0;i<b;i++){
		it++;
	}
	val[b] = v;
}


void Histogram::increment(int dim, int bin)
{
	val[bins*(dim-1) + bin - 1]++;
}