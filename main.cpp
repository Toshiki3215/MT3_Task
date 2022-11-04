#include "DxLib.h"
#include "global.h"
#include <vector>
#include "Vector3.h"

int SetCameraPositionAndTargetAndUpVec
(
	const Vector3& cameraPosition, //�J�����̈ʒu
	const Vector3& cameraTarget,   //�J�����̒����_
	const Vector3& cameraUp        //�J�����̏�̌���
);

int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

//x,y,z ���̕`��
void DrawAxis3D(const float length);

Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
                   _In_ int nCmdShow) {
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);   //Z�o�b�t�@��L���ɂ���
	SetWriteZBuffer3D(TRUE); //Z�o�b�t�@�ւ̏������݂�L���ɂ���

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	
	//�N���b�v�ʁ@�߁@�@�@��
	//�J�����̗L���͈͂�ݒ�
	SetCameraNearFar(1.0f, 1000.0f);

	//��ʂ̒��S���J�����̒��S�ɍ��킹��
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);

	SetCameraPositionAndTargetAndUpVec
	(
		Vector3(-20.0f, 20.0f, -200.0f), //�J�����ʒu
		Vector3(  0.0f,  0.0f,    0.0f),  //�J�����̒����_
		Vector3(  0.0f,  1.0f,    0.0f)   //�J�����̏�̌���
	);

	//���Ԍo�߂ɕK�v�ȃf�[�^
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//��ԂŎg���f�[�^
	//�@start �� end �� 5 [s]�Ŋ���������
	Vector3 p0(-100.0f,   0.0f,   0.0f); //�X�^�[�g�n�_
	Vector3 p1( -50.0f,  50.0f, +50.0f); //����_
	Vector3 p2( +50.0f, -30.0f, -50.0f); //����_
	Vector3 p3(+100.0f,   0.0f,   0.0f); //�G���h�n�_
	float maxTime = 5.0f;
	float timeRate;

	//���̈ʒu
	Vector3 position;

	std::vector<Vector3>points{ p0,p0,p1,p2,p3,p3 };

	size_t startIndex = 1;

	//���s�O�ɃJ�E���^�l���擾
	startCount = GetNowHiPerformanceCount(); // long long int�^ 64bit int

	// �ŐV�̃L�[�{�[�h���p
	char keys[256] = {0};

	// 1���[�v(�t���[��)�O�̃L�[�{�[�h���
	char oldkeys[256] = {0};

	// �Q�[�����[�v
	while (true) {
		// �ŐV�̃L�[�{�[�h��񂾂������̂�1�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�
		// �ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
			startIndex = 1;
		}

		//�o�ߎ���(elapsedTime [s] )�̌v�Z
		nowCount = GetNowHiPerformanceCount();
		elapsedCount = nowCount - startCount;
		float elapsedTime = static_cast<float>(elapsedCount) / 1000000.0f;

		timeRate = elapsedTime / maxTime;

		if (timeRate >= 1.0f)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex++;
				timeRate -= 1.0f;
				startCount = GetNowHiPerformanceCount();
			}
			else
			{
				timeRate = 1.0f;
			}
		}

		position = splinePosition(points, startIndex, timeRate);

		// �`�揈��
		//��ʂ�����
		ClearDrawScreen();

		//x,y,z���̕`��
		DrawAxis3D(500.0f);

		//���̕`��
		DrawSphere3D(p0, 2.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p1, 2.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p2, 2.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
		DrawSphere3D(p3, 2.0f, 32, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

		DrawSphere3D(position, 5.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);

		DrawFormatString(0,  0, GetColor(255, 255, 255), "position (%5.1f,%5.1f,%5.1f)", position.x, position.y, position.z);
		DrawFormatString(0, 20, GetColor(255, 255, 255), "startIndex %d", startIndex);
		DrawFormatString(0, 40, GetColor(255, 255, 255), "timeRate %7.3f [%%]", timeRate);
		DrawFormatString(0, 60, GetColor(255, 255, 255), "elapedTime %7.3f [s]", elapsedTime);
		DrawFormatString(0, 80, GetColor(255, 255, 255), "[R] : Restart");

		DrawFormatString(0, 100, GetColor(255, 255, 255), "p0 (%6.1f,%6.1f,%6.1f)", p0.x, p0.y, p0.z);
		DrawFormatString(0, 120, GetColor(255, 255, 255), "p1 (%6.1f,%6.1f,%6.1f)", p1.x, p1.y, p1.z);
		DrawFormatString(0, 140, GetColor(255, 255, 255), "p2 (%6.1f,%6.1f,%6.1f)", p2.x, p2.y, p2.z);
		DrawFormatString(0, 160, GetColor(255, 255, 255), "p3 (%6.1f,%6.1f,%6.1f)", p3.x, p3.y, p3.z);

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1) {
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib_End();

	// ����I��
	return 0;
}

int SetCameraPositionAndTargetAndUpVec
(
	const Vector3& cameraPosition, //�J�����̈ʒu
	const Vector3& cameraTarget,   //�J�����̒����_
	const Vector3& cameraUp        //�J�����̏�̌���
)
{
	VECTOR position = { cameraPosition.x,cameraPosition.y,cameraPosition.z };
	VECTOR target = { cameraTarget.x,cameraTarget.y,cameraTarget.z };
	VECTOR up = { cameraUp.x,cameraUp.y,cameraUp.z };

	return SetCameraPositionAndTargetAndUpVec(position, target, up);
}

int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
	VECTOR centerPos = { CenterPos.x,CenterPos.y,CenterPos.z };

	return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color)
{
	VECTOR p1 = { Pos1.x,Pos1.y,Pos1.z };
	VECTOR p2 = { Pos2.x,Pos2.y,Pos2.z };

	return DrawLine3D(p1, p2, Color);
}

void DrawAxis3D(const float length)
{
	DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));
	DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));
	DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));
}

Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t)
{
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];
	if (startIndex < 1)return points[1];

	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	Vector3 position = 0.5f * (2 * p1 + (-p0 + p2) * t +
					          (2 * p0 - 5 * p1 + 4 * p2 - p3) * (t * t) +
		                      (-p0 + 3 * p1 - 3 * p2 + p3) * (t * t * t));

	return position;
}