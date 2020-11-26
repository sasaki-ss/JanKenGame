#include <DxLib.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//プロトタイプ宣言
void Background();
bool CollisionDetection(int _px, int _py, int _pw, int _ph,
	int _ex, int _ey, int _ew, int _eh);
int PosXCorrection(int _posX, string _text);

//座標構造体
typedef struct {
	int x;
	int y;
}Pos;

//シーンの種類
enum class eScene {
	Main,
	Result
};

//勝敗の種類
enum class eResult {
	Draw,
	Lose,
	Win
};

//画像の種類
enum class ImageType {
	Gu,
	Choki,
	Pa,
	Com_Gu,
	Com_Choki,
	Com_Pa
};

//手の種類
enum class HandType {
	Gu,
	Choki,
	Pa
};

//画像クラス
class Image {
private:
	vector<int> images;	//画像リソース
public:
	//画像クラスのコンストラクタ
	Image();
	//指定した画像を取得する
	int GetImage(int _imageType) { return images[_imageType]; }
	//指定したパスの画像を読み込む
	void MyLoadGraph(const char* _filePath);
	//画像リソースを削除する
	void Clear();
};

//フォントクラス
class Font {
private:
	LPCSTR rogotype;	//ロゴたいぷゴシックを格納する変数

	int rogo_30_5;	//ロゴたいぷゴシック
public:
	Font();
	~Font();

	//フォントのゲッター
	int GetFont_Rogo_30_5(){ return rogo_30_5; }
};

//マウスクラス
class Mouse {
private:
	Pos pos;			//マウスの座標
	int leftPressCount;	//マウスの入力
public:
	//マウスクラスのコンストラクタ
	Mouse();
	//現在のマウスの座標を取得する
	Pos GetPos() { return pos; }
	//マウスの座標を更新する
	void Update();
	//マウスの押され状態を取得する
	int PushLeftBotton() { return leftPressCount; }
};

//ハンドクラス
class Hand {
private:
	Pos pos;	//手の座標
	int width;	//横幅
	int height;	//高さ

	HandType handType;		//手の種類
	ImageType imageType;	//画像の種類
public:
	//ハンドクラスのコンストラクタ
	Hand(int _x, int _y, int _width, int _height, HandType _handType);
	//描画処理
	void Draw(Image* _image) {
		DrawGraph(pos.x, pos.y,
			_image->GetImage(static_cast<int>(imageType)), TRUE);
	}
	//手の座標を取得する
	Pos GetPos() { return pos; }
	//手の横幅を取得する
	int GetWidth() { return width; }
	//手の高さを取得する
	int GetHeight() { return height; }
	//手の種類を取得する
	HandType GetHandType() { return handType; }
};

//コンピューターハンドクラス
class ComHand {
private:
	Pos pos;	//手の座標
	int width;	//横幅
	int height;	//高さ

	HandType handType;		//手の種類
	ImageType imageType;	//画像の種類
public:
	//コンピューターハンドクラスのコンストラクタ
	ComHand(int _x, int _y);
	//描画処理
	void Draw(Image* _image) {
		DrawGraph(pos.x, pos.y,
			_image->GetImage(static_cast<int>(imageType)), TRUE);
	}
	//手の種類を取得する
	HandType GetHandType() { return handType; }
};

//ハンドマネージャー
class HandManager {
private:
	HandType selectHandType;
public:
	HandManager() { selectHandType = HandType::Gu; }
	//一時保存する手をセット
	void SetHand(HandType _handType) { selectHandType = _handType; }
	//一時保存されている手を取得する
	HandType GetHand() { return selectHandType; }
};

//シーン切り替えのインターフェイス
class ISceneChanger {
public:
	ISceneChanger() = default;
	//デストラクタ
	virtual ~ISceneChanger() = default;
	//シーン切り替え処理
	virtual void ChangeScene(eScene _nextScene, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font) = 0;
};

//シーンクラス
class Scene {
protected:
	eScene nowScene;				//現在のシーン
	ISceneChanger* sceneChanger;	//シーン切り替えインターフェイス

	Font* font;					//フォントクラスのオブジェクト
	Image* image;				//画像クラスのオブジェクト
	Mouse* mouse;				//マウスクラスのオブジェクト
	HandManager* handManager;	//ハンドマネージャー
public:
	//シーンクラスのコンストラクタ
	Scene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
	//更新処理
	virtual void Update() = 0;
	//描画処理
	virtual void Draw() = 0;
	//シーン切り替え処理
	void SwitchingOperation(HandType _handType, eScene _nextScene);

	//現在のシーンを取得する
	eScene GetNowScene() { return nowScene; }
};

//メインシーンクラス
class MainScene : public virtual Scene {
private:
	Hand* gu;		//グー
	Hand* choki;	//チョキ
	Hand* pa;		//パー
public:
	//メインシーンクラスのコンストラクタ
	MainScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
	//更新処理
	void Update();
	//描画処理
	void Draw();
};

//リザルトシーンクラス
class ResultScene : public virtual Scene {
private:
	Hand* selectHand;	//選択された手
	ComHand* comHand;	//コンピューターの手

	int result;		//勝敗判定用
	string text;	//表示するテキスト

	Pos textPos;	//テキストを表示する座標
public:
	//リザルトシーンクラスのコンストラクタ
	ResultScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
	//更新処理
	void Update();
	//描画処理
	void Draw();
};

class SceneManager :public ISceneChanger {
private:
	Scene* scene;	//シーンクラス
public:
	//シーンマネージャーのコンストラクタ
	SceneManager(Image* _image, Mouse* _mouse, HandManager* _handManager, Font* _font) {
		scene = new MainScene(this, _image, _mouse, _handManager, _font);
	}
	//更新処理
	void Update() { scene->Update(); }
	//描画処理
	void Draw() { scene->Draw(); }
	//シーン切り替え処理
	void ChangeScene(eScene _nextScene, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//DXライブラリの初期設定
	ChangeWindowMode(TRUE);
	SetMainWindowText("じゃんけんゲーム");
	SetGraphMode(1000, 1000, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	Font font;		//フォントクラスのオブジェクト
	Image image;	//画像クラスのオブジェクト
	Mouse mouse;	//マウスクラスのオブジェクト

	HandManager handManager;	//ハンドマネージャー

	SceneManager sceneManager(&image, &mouse, &handManager, &font);	//シーンマネージャーのオブジェクト

	//常に実行する処理
	while (!ProcessMessage()) {
		//画面をクリア
		ClearDrawScreen();

		//背景の描画
		Background();

		//シーンの処理
		sceneManager.Update();
		sceneManager.Draw();

		//マウス座標の更新
		mouse.Update();

		//裏描画を表画面に反映
		ScreenFlip();
	}

	//DXライブラリの終了処理
	DxLib_End();
	return 0;
}

//背景描画関数
void Background() {
	DrawBox(0, 0, 1000, 1000, GetColor(255, 255, 255), TRUE);
	DrawBox(0, 0, 1000, 50, GetColor(0, 0, 255), TRUE);
	DrawString(0, 25, "じゃんけんゲーム", GetColor(255, 255, 255));
}

//当たり判定処理
bool CollisionDetection(int _px, int _py, int _pw, int _ph,
	int _ex, int _ey, int _ew, int _eh) {

	//当たり判定
	if (_px + _pw >= _ex &&
		_px <= _ex + _ew &&
		_py + _ph >= _ey &&
		_py <= _ey + _eh) {
		return true;
	}

	return false;
}

//表示文字列のx座標を補正する関数
int PosXCorrection(int _posX,string _text) {
	int textSize;				//テキストサイズ
	bool isHalfSize = false;	//半角フラグ

	/*************************************************** 
	全角判定処理
	全角の場合 _textの半分を格納する ※全角が2バイトの為
	半角の場合 _textを格納する
	***************************************************/
	if (IsDBCSLeadByte(_text[0]) == 0) {
		textSize = _text.size();
		isHalfSize = true;
	}
	else {
		textSize = _text.size() / 2;
	}

	//テキストサイズが奇数の場合偶数にする
	if (textSize % 2 != 0) {
		textSize++;
	}

	//半角の場合の補正した値を返す
	if (isHalfSize == true) {
		return _posX - (15 * (textSize / 2));
	}

	//全角の場合の補正した値を返す
	return _posX - (15 * textSize);
}

/*******************************************************************************
シーンクラスの処理
*******************************************************************************/

//シーンクラスのコンストラクタ
Scene::Scene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font)
	:sceneChanger(_sceneChanger) {
	nowScene = eScene::Main;
	image = _image;
	mouse = _mouse;
	handManager = _handManager;
	font = _font;
}

//シーン切り替え処理
void Scene::SwitchingOperation(HandType _handType, eScene _nextScene) {
	//ハンドマネージャーに選択した手の種類を保存
	handManager->SetHand(_handType);

	//現在のシーンを次のシーンに
	nowScene = _nextScene;

	//シーンを切り替える
	sceneChanger->ChangeScene(_nextScene, image, mouse, handManager, font);
}

/*******************************************************************************
メインシーンクラスの処理
*******************************************************************************/

MainScene::MainScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font)
	:Scene(_sceneChanger, _image, _mouse, _handManager, _font) {
	gu = new Hand(50, 350, 300, 300, HandType::Gu);
	choki = new Hand(350, 350, 300, 300, HandType::Choki);
	pa = new Hand(650, 350, 300, 300, HandType::Pa);
}

//更新処理
void MainScene::Update() {
	//マウスの左クリックを検知した場合
	if (mouse->PushLeftBotton() == 1) {
		//グーを選択した場合の処理
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			gu->GetPos().x, gu->GetPos().y, gu->GetWidth(), gu->GetHeight()) == true) {

			SwitchingOperation(HandType::Gu, eScene::Result);

			return;
		}

		//チョキを選択した場合の処理
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			choki->GetPos().x, choki->GetPos().y, choki->GetWidth(), choki->GetHeight()) == true) {

			SwitchingOperation(HandType::Choki, eScene::Result);

			return;
		}

		//パーを選択した場合の処理
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			pa->GetPos().x, pa->GetPos().y, pa->GetWidth(), pa->GetHeight()) == true) {

			SwitchingOperation(HandType::Pa, eScene::Result);

			return;
		}
	}
}

//描画処理
void MainScene::Draw() {
	//選択する手を描画する
	gu->Draw(image);
	choki->Draw(image);
	pa->Draw(image);
}

/*******************************************************************************
リザルトシーンクラスの処理
*******************************************************************************/

//リザルトシーンクラスのコンストラクタ
ResultScene::ResultScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font)
	:Scene(_sceneChanger, _image, _mouse, _handManager, _font) {
	selectHand = new Hand(350, 50, 300, 300, _handManager->GetHand());
	comHand = new ComHand(316, 456);
	text = "";

	//勝敗判定
	result = (static_cast<int>(selectHand->GetHandType()) -
		static_cast<int>(comHand->GetHandType()) + 3) % 3;
	//表示するテキストを勝敗によって格納する
	switch (static_cast<eResult>(result)) {
	case eResult::Lose:
		text = "なんで負けたか明日まで考えてください";
		break;
	case eResult::Draw:
		text = "引き分け";
		break;
	case eResult::Win:
		text = "You Win!! Congratulation";
		break;
	}

	textPos.x = PosXCorrection(500, text);
	textPos.y = 400;
}

//更新処理
void ResultScene::Update() {

	if (mouse->PushLeftBotton() == 1) {
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			0, 50, 100, 50)) {
			nowScene = eScene::Main;
			sceneChanger->ChangeScene(eScene::Main, image, mouse, handManager, font);
		}
	}
}

//描画処理
void ResultScene::Draw() {
	//プレイヤーの選択した手を表示
	selectHand->Draw(image);
	//コンピューターの手を表示
	comHand->Draw(image);
	//勝敗テキストを表示
	DrawStringToHandle(textPos.x, textPos.y, text.c_str(),
		GetColor(0, 0, 0), font->GetFont_Rogo_30_5());

	//戻るボタンのUI
	DrawBox(0, 50, 100, 100, GetColor(125, 125, 125), TRUE);
	DrawStringToHandle(PosXCorrection(50, (string)"戻る"), 75-15, "戻る", GetColor(0, 0, 0),
		font->GetFont_Rogo_30_5());
}

/*******************************************************************************
シーンマネージャークラスの処理
*******************************************************************************/

//シーン切り替えの処理
void SceneManager::ChangeScene(eScene _nextScene, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font) {
	switch (_nextScene) {
	case eScene::Main:
		scene = new MainScene(this, _image, _mouse, _handManager, _font);
		break;
	case eScene::Result:
		scene = new ResultScene(this, _image, _mouse, _handManager, _font);
		break;
	}
}

/*******************************************************************************
画像クラスの処理
*******************************************************************************/

//画像クラスのコンストラクタ
Image::Image() {
	MyLoadGraph("./res/Image/gu.png");
	MyLoadGraph("./res/Image/choki.png");
	MyLoadGraph("./res/Image/pa.png");
	MyLoadGraph("./res/Image/com_gu.png");
	MyLoadGraph("./res/Image/com_choki.png");
	MyLoadGraph("./res/Image/com_pa.png");
}

//画像読み込み処理
void Image::MyLoadGraph(const char* _filePath) {
	int tmp = LoadGraph(_filePath);
	images.push_back(tmp);
}

//画像リソースを削除する
void Image::Clear() {
	for (auto image : images) {
		DeleteGraph(image);
	}
	images.clear();
}

/*******************************************************************************
フォントクラスの処理
*******************************************************************************/

//フォントクラスのコンストラクタ
Font::Font() {
	rogotype = "./res/Font/ロゴたいぷゴシック.otf";	//ロゴたいぷゴシックのファイルパス

	//フォントを読み込む
	AddFontResourceEx(rogotype, FR_PRIVATE, NULL);

	//フォントを作成
	rogo_30_5 = CreateFontToHandle("07ロゴたいぷゴシック7", 30, 5, DX_FONTTYPE_ANTIALIASING);
}

//フォントクラスのデストラクタ
Font::~Font() {
	RemoveFontResource(rogotype);

	InitFontToHandle();
}

/*******************************************************************************
マウスクラスの処理
*******************************************************************************/

//マウスクラスのコンストラクタ
Mouse::Mouse() {
	leftPressCount = 0;

	SetMousePoint(500, 500);
	GetMousePoint(&pos.x, &pos.y);
}

void Mouse::Update() {
	//マウスの座標を取得する
	GetMousePoint(&pos.x, &pos.y);

	//マウスの左ボタンが押された場合leftPressCountを加算
	if (GetMouseInput() & MOUSE_INPUT_LEFT != 0) {
		leftPressCount++;
	}
	//押されてない場合0にする
	else {
		leftPressCount = 0;
	}
}

/*******************************************************************************
手クラスの処理
*******************************************************************************/

//手クラスのコンストラクタ
Hand::Hand(int _x, int _y, int _width, int _height, HandType _handType) {
	pos.x = _x;
	pos.y = _y;
	width = _width;
	height = _height;
	handType = _handType;

	switch (_handType) {
	case HandType::Gu:
		imageType = ImageType::Gu;
		break;
	case HandType::Choki:
		imageType = ImageType::Choki;
		break;
	case HandType::Pa:
		imageType = ImageType::Pa;
		break;
	}
}

/*******************************************************************************
コンピューターハンドクラスの処理
*******************************************************************************/

//コンピューターハンドクラスのコンストラクタ
ComHand::ComHand(int _x, int _y) {
	pos.x = _x;
	pos.y = _y;

	switch (static_cast<HandType>(GetRand(2))){
	case HandType::Gu:
		handType = HandType::Gu;
		imageType = ImageType::Com_Gu;
		break;
	case HandType::Choki:
		handType = HandType::Choki;
		imageType = ImageType::Com_Choki;
		break;
	case HandType::Pa:
		handType = HandType::Pa;
		imageType = ImageType::Com_Pa;
	}
}