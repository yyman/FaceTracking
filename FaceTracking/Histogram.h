#pragma once
#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

class Histogram
{
private:
	int bins;
    int dimension;
    int sum;
	int maxBin;
	int minBin;
	vector<int> val;
public:
	Histogram(int b, int d);
	~Histogram(void);
	int getVal(int dim, int x);
	void setVal(int b,int v);
	void setMaxBin(int n){maxBin = n;};
	void setMinBin(int n){minBin = n;};
	void increment(int dim, int bin);
};
#endif /*HISTOGRAM_H_*/
