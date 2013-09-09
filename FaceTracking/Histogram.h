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
	std::vector<double> val;
public:
	Histogram(void);
	Histogram(int b, int d);
	~Histogram(void);

	int getBins(){return bins;};
	double getVal(int dim, int x);

	void setVal(int dim, int bin, double v);
	void setMaxBin(int n){maxBin = n;};
	void setMinBin(int n){minBin = n;};
	void increment(int dim, int bin);
	void show(char* widowName);
	void calcMaxMinBin();
};
#endif /*HISTOGRAM_H_*/
