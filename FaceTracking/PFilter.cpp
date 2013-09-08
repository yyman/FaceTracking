

#include "PFilter.h"

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <opencv/cv.h>

//#define DEBUG


PFilter::PFilter() {
}

//コンストラクタ
PFilter::PFilter(int _num, LIMIT _upper, LIMIT _lower, NOISE _noise)
{
  srand((unsigned) time(NULL));
  this->num = _num;

  //上限、下限、ノイズ
  this->upper = _upper;
  this->lower = _lower;
  this->noise = _noise;

  //パーティクル配列の初期化
  for(int i=0; i<num; i++){
    int x = rand()%(upper.x - lower.x ) + lower.x;
    int y = rand()%(upper.y - lower.y ) + lower.y;
    int vx = rand()%(upper.vx - lower.vx ) + lower.vx;
    int vy = rand()%(upper.vy - lower.vy ) + lower.vy;
    double w = 1.0/(double)num;

    Particle* p = new Particle(x, y, vx, vy, w);

    particles.push_back(p);
  }
  for(int i=0; i<num; i++){
    pre_particles.push_back(particles[i]);
  }

#ifdef _DEBUG
  for(int i=0; i<num; i++){
    printf("%3d : (%3d, %3d, %3d, %3d), w=%lf \n", 
	   i, particles[i]->get_x(), particles[i]->get_y(), 
	   particles[i]->get_vx(), particles[i]->get_vy(),  particles[i]->getWeight());
  }
#endif

}

//デストラクタ
PFilter::~PFilter()
{
}

//事前推定（遷移モデルの適用）から次のパーティクルの位置を予測
void PFilter::predict(){
 
  int pre_x,pre_y;
    
  for(int i=0; i<num; i++){

    //ランダムノイズの更新
    NOISE n;
    n.x =  (int)(((double)rand() / ((double)RAND_MAX + 1)) * (double)noise.x * 2 - (double)noise.x);
    n.y =  (int)(((double)rand() / ((double)RAND_MAX + 1)) * (double)noise.y * 2 - (double)noise.y);
    n.vx = (int)(((double)rand() / ((double)RAND_MAX + 1)) * (double)noise.vx* 2 - (double)noise.vx); 
    n.vy = (int)(((double)rand() / ((double)RAND_MAX + 1)) * (double)noise.vy* 2 - (double)noise.vy); 


    // 等速直線運動をモデルに移動したと予測する
    pre_x = particles[i]->get_x() + particles[i]->get_vx() + n.x;
    pre_particles[i]->set_x( pre_x );
    pre_y  = particles[i]->get_y() + particles[i]->get_vy() + n.y;
    pre_particles[i]->set_y( pre_y );
    pre_particles[i]->set_vx(n.vx);
    pre_particles[i]->set_vy(n.vy);


    //下限より小さいとき、下限の値とする
    if(pre_particles[i]->get_x() < lower.x) pre_particles[i]->set_x( lower.x );
    if(pre_particles[i]->get_y() < lower.y) pre_particles[i]->set_y( lower.y );
    if(pre_particles[i]->get_vx() < lower.vx) pre_particles[i]->set_vx( lower.vx );
    if(pre_particles[i]->get_vy() < lower.vy) pre_particles[i]->set_vy( lower.vy );

    //上限より大きいとき、上限の値とする
    if(pre_particles[i]->get_x() > upper.x) pre_particles[i]->set_x( upper.x );
    if(pre_particles[i]->get_y() > upper.y) pre_particles[i]->set_y( upper.y );
    if(pre_particles[i]->get_vx() > upper.vx) pre_particles[i]->set_vx( upper.vx );
    if(pre_particles[i]->get_vy() > upper.vy) pre_particles[i]->set_vy( upper.vy );

  }
}

//予測後の各パーティクルについて重み付けを行う
void PFilter::weight(IplImage* img)
{
  for(int i=0; i<num; i++){
    int x = pre_particles[i]->get_x();
    int y = pre_particles[i]->get_y();
    pre_particles[i]->setWeight( calcLikelihood(img, x, y) );
  }

  //正規化
  double sum = 0.0;
  for(int i=0; i<num; i++){
    sum += pre_particles[i]->getWeight();
  }
  for(int i=0; i<num; i++){
    double w = pre_particles[i]->getWeight() / sum;
    pre_particles[i]->setWeight( w );
  }
}


// 尤度の計算　ここを編集
double PFilter::calcLikelihood(IplImage* img, int x, int y){
  double result =0.0;

  // img が3ch(RGB)の場合
  if(img->nChannels==3){
    // RGB 色空間の場合
    unsigned char b, g, r;
    b = img->imageData[img->widthStep*y + x*3];     // B
    g = img->imageData[img->widthStep*y + x*3 + 1]; // G
    r = img->imageData[img->widthStep*y + x*3 + 2]; // R
    
    double dist = 0.0, sigma = 50.0;  
    // 赤色らしさをユークリッド距離として求める
    dist = sqrt( b*b + g*g + (255-r)*(255-r));
    // 距離(dist)を平均、sigmaを分散として持つ、正規分布を尤度関数とする
    result = 1.0 / (sqrt(2.0*CV_PI)*sigma) * expf(-dist*dist/(2.0*sigma*sigma));
  }
  // グレイスケールの場合
  else if( img->nChannels == 1){
    unsigned char gray;
    gray = img->imageData[img->widthStep*y + x];
    if(gray > 0){
      result = 1.0;
    }
    else {
      result = 0.00001;
    }
  }
  return result;
}


//重みに基づき再サンプリングする（ルーレット選択）
void PFilter::resample()
{  

  // 累積重み（＝ルーレット）
  double * w = new double[num];
  w[0] = pre_particles[0]->getWeight();
  for(int i=1; i<num; i++){
    w[i] = w[i-1] + pre_particles[i]->getWeight();
  }

  for(int i=0; i<num; i++){
    double darts = ((double)rand() / ((double)RAND_MAX + 1));
    for(int j=0; j<num; j++){
      if( darts > w[j] ){
	continue;
      }
      else {
	// リサンプリング
	particles[i]->set_x( pre_particles[j]->get_x() );
	particles[i]->set_y( pre_particles[j]->get_y() );
	particles[i]->set_vx( pre_particles[j]->get_vx() );
	particles[i]->set_vy( pre_particles[j]->get_vy() );
	//particles[i]->setWeight( pre_particles[j]->getWeight() );
	particles[i]->setWeight( 0.0 );
	break;
      }     
    }
  }
}

//パーティクルの重みつき平均を推定結果
void PFilter::measure( Particle* result)
{
  double x=0, y=0, vx=0, vy=0;
  for(int i=0; i<num; i++){
    x += (double)particles[i]->get_x() * particles[i]->getWeight();
    y += (double)particles[i]->get_y() * particles[i]->getWeight();
    vx += (double)particles[i]->get_vx() * particles[i]->getWeight();
    vy += (double)particles[i]->get_vy() * particles[i]->getWeight();
  }
  result->set_x( (int)x );
  result->set_y( (int)y );
  result->set_vx( (int)vx );
  result->set_vy( (int)vy );
}