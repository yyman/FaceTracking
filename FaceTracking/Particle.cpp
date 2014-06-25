#include "Particle.h"


Particle::Particle(){
  x = 0;
  y = 0;
  vx = 0;
  vy = 0;
  weight = 0;
}

Particle::Particle(int _x, int _y, int _vx, int _vy, double _w){
	x = _x;
	y = _y;
	vx = _vx;
	vy = _vy;
	weight = _w;
}

Particle::~Particle()
{
}

double Particle::getWeight(){
	return weight;
}

void Particle::setWeight(double w){
	weight = w;	
}

int Particle::get_x(){
	return x;
}

int Particle::get_y(){
	return y;
}

int  Particle::get_vx(){
	return vx;
}

int Particle::get_vy(){
	return vy;
}

void Particle::set_x(int _x){
  x = _x;
}

void Particle::set_y(int _y){
	y = _y;
}

void Particle::set_vx(int _vx){
	vx = _vx;
}

void Particle::set_vy(int _vy){
	vy = _vy;
}

/*Histogram Particle::getHist(){
	return pHist;
}

void Particle::setHist(Histogram _pHist){
	pHist = _pHist;
}*/

Block Particle::getBlock(){
	return pBlock;
}

void Particle::setBlock(Block _pBlock){
	pBlock = _pBlock;
}