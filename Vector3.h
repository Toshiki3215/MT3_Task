#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;

public:
	//�R���X�g���N�^
	//��x�N�g��
	Vector3();
	//x�����Ay�����Az�������w�肵�Đ���
	Vector3(float x, float y, float z);

	//�����o�֐�

	//�m����(����)�����߂�
	float length() const;

	//���K��
	Vector3& normalize();

	//���ς����߂�
	float dot(const Vector3& v)const;

	//�O�ς����߂�
	Vector3 cross(const Vector3& v)const;

	//�P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

};

//2�����Z�q�I�[�o�[���[�h
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

//���`���
const Vector3 lerp(const Vector3& start, const Vector3& end, const float t);

