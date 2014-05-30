#ifndef PFILTER_H_
#define PFILTER_H_


#include "Particle.h"
#include "calcHSVHist.h"

typedef struct {
	int x,y, vx,vy;
} LIMIT;

typedef struct {
	int x,y,vx,vy;
} NOISE;


class PFilter{
private:
	double calcLikelihood(Mat img, int x, int y, Size _blockSize);

	// 各変数の上限、下限
	LIMIT upper, lower;

	// ノイズ
	NOISE noise;

	//パーティクル数
	int num;

	//顔画像のサイズ（ベースヒストグラムとの比較用）
	Size blockSize;

	//ヒストグラム計算用クラス
	calcHSVHist ch;

public:
	//コンストラクタ
	PFilter();
	PFilter(int num, LIMIT upper,  LIMIT lower, NOISE noise);

	//デストラクタ
	~PFilter();

	//パーティクル配列
	vector<Particle*> particles;
	// パーティクル配列（事前推定用）
	vector<Particle*> pre_particles;

	//状態遷移モデルを適用
	void predict();

	//重みに基づきサンプリング
	void resample();

	//重み付け
	void weight(Mat img, Size _blockSize, Mat baseImg);

	//パーティクル群の重みつき平均を出力
	void  measure(Particle* r);

};
#endif /*PFILTER_H_*/