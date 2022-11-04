#include "DxLib.h"
#include "global.h"
#include <vector>
#include "Vector3.h"

int SetCameraPositionAndTargetAndUpVec
(
	const Vector3& cameraPosition, //カメラの位置
	const Vector3& cameraTarget,   //カメラの注視点
	const Vector3& cameraUp        //カメラの上の向き
);

int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum,
	const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

//x,y,z 軸の描画
void DrawAxis3D(const float length);

Vector3 splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
                   _In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(TRUE);   //Zバッファを有効にする
	SetWriteZBuffer3D(TRUE); //Zバッファへの書き込みを有効にする

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言
	
	//クリップ面　近　　　遠
	//カメラの有効範囲を設定
	SetCameraNearFar(1.0f, 1000.0f);

	//画面の中心をカメラの中心に合わせる
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);

	SetCameraPositionAndTargetAndUpVec
	(
		Vector3(-20.0f, 20.0f, -200.0f), //カメラ位置
		Vector3(  0.0f,  0.0f,    0.0f),  //カメラの注視点
		Vector3(  0.0f,  1.0f,    0.0f)   //カメラの上の向き
	);

	//時間経過に必要なデータ
	long long startCount = 0;
	long long nowCount = 0;
	long long elapsedCount = 0;

	//補間で使うデータ
	//　start → end を 5 [s]で完成させる
	Vector3 p0(-100.0f,   0.0f,   0.0f); //スタート地点
	Vector3 p1( -50.0f,  50.0f, +50.0f); //制御点
	Vector3 p2( +50.0f, -30.0f, -50.0f); //制御点
	Vector3 p3(+100.0f,   0.0f,   0.0f); //エンド地点
	float maxTime = 5.0f;
	float timeRate;

	//球の位置
	Vector3 position;

	std::vector<Vector3>points{ p0,p0,p1,p2,p3,p3 };

	size_t startIndex = 1;

	//実行前にカウンタ値を取得
	startCount = GetNowHiPerformanceCount(); // long long int型 64bit int

	// 最新のキーボード情報用
	char keys[256] = {0};

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = {0};

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		if (CheckHitKey(KEY_INPUT_R))
		{
			startCount = GetNowHiPerformanceCount();
			startIndex = 1;
		}

		//経過時間(elapsedTime [s] )の計算
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

		// 描画処理
		//画面を消去
		ClearDrawScreen();

		//x,y,z軸の描画
		DrawAxis3D(500.0f);

		//球の描画
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

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

int SetCameraPositionAndTargetAndUpVec
(
	const Vector3& cameraPosition, //カメラの位置
	const Vector3& cameraTarget,   //カメラの注視点
	const Vector3& cameraUp        //カメラの上の向き
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