#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	//コンストラクタ
	//零ベクトル
	Vector3();
	//x成分、y成分、z成分を指定して生成
	Vector3(float x, float y, float z);

	//メンバ関数

	//ノルム(長さ)を求める
	float length() const;

	//正規化
	Vector3& normalize();

	//内積を求める
	float dot(const Vector3& v)const;

	//外積を求める
	Vector3 cross(const Vector3& v)const;

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

};

//2項演算子オーバーロード
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

//線形補間
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

