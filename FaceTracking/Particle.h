#ifndef PARTICLE_H_
#define PARTICLE_H_

class Particle
{
 private:
  int x,y,vx,vy;
  double weight;
  
 public:
  Particle();
  Particle(int _x, int _y, int _vx, int _vy, double _w);

  virtual ~Particle();
  
  int get_x();
  int get_y();
  int get_vx();
  int get_vy();
  double getWeight();
  
  void set_x(int _x);
  void set_y(int _y);
  void set_vx(int _vx);
  void set_vy(int _vy);
  void setWeight(double _w);
  
};

#endif /*PARTICLE_H_*/