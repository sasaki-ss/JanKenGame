#include <DxLib.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

//�v���g�^�C�v�錾
void Background();
bool CollisionDetection(int _px, int _py, int _pw, int _ph,
	int _ex, int _ey, int _ew, int _eh);
int PosXCorrection(int _posX, string _text);

//���W�\����
typedef struct {
	int x;
	int y;
}Pos;

//�V�[���̎��
enum class eScene {
	Main,
	Result
};

//���s�̎��
enum class eResult {
	Draw,
	Lose,
	Win
};

//�摜�̎��
enum class ImageType {
	Gu,
	Choki,
	Pa,
	Com_Gu,
	Com_Choki,
	Com_Pa
};

//��̎��
enum class HandType {
	Gu,
	Choki,
	Pa
};

//�摜�N���X
class Image {
private:
	vector<int> images;	//�摜���\�[�X
public:
	//�摜�N���X�̃R���X�g���N�^
	Image();
	//�w�肵���摜���擾����
	int GetImage(int _imageType) { return images[_imageType]; }
	//�w�肵���p�X�̉摜��ǂݍ���
	void MyLoadGraph(const char* _filePath);
	//�摜���\�[�X���폜����
	void Clear();
};

//�t�H���g�N���X
class Font {
private:
	LPCSTR rogotype;	//���S�����ՃS�V�b�N���i�[����ϐ�

	int rogo_30_5;	//���S�����ՃS�V�b�N
public:
	Font();
	~Font();

	//�t�H���g�̃Q�b�^�[
	int GetFont_Rogo_30_5(){ return rogo_30_5; }
};

//�}�E�X�N���X
class Mouse {
private:
	Pos pos;			//�}�E�X�̍��W
	int leftPressCount;	//�}�E�X�̓���
public:
	//�}�E�X�N���X�̃R���X�g���N�^
	Mouse();
	//���݂̃}�E�X�̍��W���擾����
	Pos GetPos() { return pos; }
	//�}�E�X�̍��W���X�V����
	void Update();
	//�}�E�X�̉������Ԃ��擾����
	int PushLeftBotton() { return leftPressCount; }
};

//�n���h�N���X
class Hand {
private:
	Pos pos;	//��̍��W
	int width;	//����
	int height;	//����

	HandType handType;		//��̎��
	ImageType imageType;	//�摜�̎��
public:
	//�n���h�N���X�̃R���X�g���N�^
	Hand(int _x, int _y, int _width, int _height, HandType _handType);
	//�`�揈��
	void Draw(Image* _image) {
		DrawGraph(pos.x, pos.y,
			_image->GetImage(static_cast<int>(imageType)), TRUE);
	}
	//��̍��W���擾����
	Pos GetPos() { return pos; }
	//��̉������擾����
	int GetWidth() { return width; }
	//��̍������擾����
	int GetHeight() { return height; }
	//��̎�ނ��擾����
	HandType GetHandType() { return handType; }
};

//�R���s���[�^�[�n���h�N���X
class ComHand {
private:
	Pos pos;	//��̍��W
	int width;	//����
	int height;	//����

	HandType handType;		//��̎��
	ImageType imageType;	//�摜�̎��
public:
	//�R���s���[�^�[�n���h�N���X�̃R���X�g���N�^
	ComHand(int _x, int _y);
	//�`�揈��
	void Draw(Image* _image) {
		DrawGraph(pos.x, pos.y,
			_image->GetImage(static_cast<int>(imageType)), TRUE);
	}
	//��̎�ނ��擾����
	HandType GetHandType() { return handType; }
};

//�n���h�}�l�[�W���[
class HandManager {
private:
	HandType selectHandType;
public:
	HandManager() { selectHandType = HandType::Gu; }
	//�ꎞ�ۑ��������Z�b�g
	void SetHand(HandType _handType) { selectHandType = _handType; }
	//�ꎞ�ۑ�����Ă������擾����
	HandType GetHand() { return selectHandType; }
};

//�V�[���؂�ւ��̃C���^�[�t�F�C�X
class ISceneChanger {
public:
	ISceneChanger() = default;
	//�f�X�g���N�^
	virtual ~ISceneChanger() = default;
	//�V�[���؂�ւ�����
	virtual void ChangeScene(eScene _nextScene, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font) = 0;
};

//�V�[���N���X
class Scene {
protected:
	eScene nowScene;				//���݂̃V�[��
	ISceneChanger* sceneChanger;	//�V�[���؂�ւ��C���^�[�t�F�C�X

	Font* font;					//�t�H���g�N���X�̃I�u�W�F�N�g
	Image* image;				//�摜�N���X�̃I�u�W�F�N�g
	Mouse* mouse;				//�}�E�X�N���X�̃I�u�W�F�N�g
	HandManager* handManager;	//�n���h�}�l�[�W���[
public:
	//�V�[���N���X�̃R���X�g���N�^
	Scene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
	//�X�V����
	virtual void Update() = 0;
	//�`�揈��
	virtual void Draw() = 0;
	//�V�[���؂�ւ�����
	void SwitchingOperation(HandType _handType, eScene _nextScene);

	//���݂̃V�[�����擾����
	eScene GetNowScene() { return nowScene; }
};

//���C���V�[���N���X
class MainScene : public virtual Scene {
private:
	Hand* gu;		//�O�[
	Hand* choki;	//�`���L
	Hand* pa;		//�p�[
public:
	//���C���V�[���N���X�̃R���X�g���N�^
	MainScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
};

//���U���g�V�[���N���X
class ResultScene : public virtual Scene {
private:
	Hand* selectHand;	//�I�����ꂽ��
	ComHand* comHand;	//�R���s���[�^�[�̎�

	int result;		//���s����p
	string text;	//�\������e�L�X�g

	Pos textPos;	//�e�L�X�g��\��������W
public:
	//���U���g�V�[���N���X�̃R���X�g���N�^
	ResultScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
};

class SceneManager :public ISceneChanger {
private:
	Scene* scene;	//�V�[���N���X
public:
	//�V�[���}�l�[�W���[�̃R���X�g���N�^
	SceneManager(Image* _image, Mouse* _mouse, HandManager* _handManager, Font* _font) {
		scene = new MainScene(this, _image, _mouse, _handManager, _font);
	}
	//�X�V����
	void Update() { scene->Update(); }
	//�`�揈��
	void Draw() { scene->Draw(); }
	//�V�[���؂�ւ�����
	void ChangeScene(eScene _nextScene, Image* _image, Mouse* _mouse,
		HandManager* _handManager, Font* _font);
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//DX���C�u�����̏����ݒ�
	ChangeWindowMode(TRUE);
	SetMainWindowText("����񂯂�Q�[��");
	SetGraphMode(1000, 1000, 32);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);

	Font font;		//�t�H���g�N���X�̃I�u�W�F�N�g
	Image image;	//�摜�N���X�̃I�u�W�F�N�g
	Mouse mouse;	//�}�E�X�N���X�̃I�u�W�F�N�g

	HandManager handManager;	//�n���h�}�l�[�W���[

	SceneManager sceneManager(&image, &mouse, &handManager, &font);	//�V�[���}�l�[�W���[�̃I�u�W�F�N�g

	//��Ɏ��s���鏈��
	while (!ProcessMessage()) {
		//��ʂ��N���A
		ClearDrawScreen();

		//�w�i�̕`��
		Background();

		//�V�[���̏���
		sceneManager.Update();
		sceneManager.Draw();

		//�}�E�X���W�̍X�V
		mouse.Update();

		//���`���\��ʂɔ��f
		ScreenFlip();
	}

	//DX���C�u�����̏I������
	DxLib_End();
	return 0;
}

//�w�i�`��֐�
void Background() {
	DrawBox(0, 0, 1000, 1000, GetColor(255, 255, 255), TRUE);
	DrawBox(0, 0, 1000, 50, GetColor(0, 0, 255), TRUE);
	DrawString(0, 25, "����񂯂�Q�[��", GetColor(255, 255, 255));
}

//�����蔻�菈��
bool CollisionDetection(int _px, int _py, int _pw, int _ph,
	int _ex, int _ey, int _ew, int _eh) {

	//�����蔻��
	if (_px + _pw >= _ex &&
		_px <= _ex + _ew &&
		_py + _ph >= _ey &&
		_py <= _ey + _eh) {
		return true;
	}

	return false;
}

//�\���������x���W��␳����֐�
int PosXCorrection(int _posX,string _text) {
	int textSize;				//�e�L�X�g�T�C�Y
	bool isHalfSize = false;	//���p�t���O

	/*************************************************** 
	�S�p���菈��
	�S�p�̏ꍇ _text�̔������i�[���� ���S�p��2�o�C�g�̈�
	���p�̏ꍇ _text���i�[����
	***************************************************/
	if (IsDBCSLeadByte(_text[0]) == 0) {
		textSize = _text.size();
		isHalfSize = true;
	}
	else {
		textSize = _text.size() / 2;
	}

	//�e�L�X�g�T�C�Y����̏ꍇ�����ɂ���
	if (textSize % 2 != 0) {
		textSize++;
	}

	//���p�̏ꍇ�̕␳�����l��Ԃ�
	if (isHalfSize == true) {
		return _posX - (15 * (textSize / 2));
	}

	//�S�p�̏ꍇ�̕␳�����l��Ԃ�
	return _posX - (15 * textSize);
}

/*******************************************************************************
�V�[���N���X�̏���
*******************************************************************************/

//�V�[���N���X�̃R���X�g���N�^
Scene::Scene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font)
	:sceneChanger(_sceneChanger) {
	nowScene = eScene::Main;
	image = _image;
	mouse = _mouse;
	handManager = _handManager;
	font = _font;
}

//�V�[���؂�ւ�����
void Scene::SwitchingOperation(HandType _handType, eScene _nextScene) {
	//�n���h�}�l�[�W���[�ɑI��������̎�ނ�ۑ�
	handManager->SetHand(_handType);

	//���݂̃V�[�������̃V�[����
	nowScene = _nextScene;

	//�V�[����؂�ւ���
	sceneChanger->ChangeScene(_nextScene, image, mouse, handManager, font);
}

/*******************************************************************************
���C���V�[���N���X�̏���
*******************************************************************************/

MainScene::MainScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font)
	:Scene(_sceneChanger, _image, _mouse, _handManager, _font) {
	gu = new Hand(50, 350, 300, 300, HandType::Gu);
	choki = new Hand(350, 350, 300, 300, HandType::Choki);
	pa = new Hand(650, 350, 300, 300, HandType::Pa);
}

//�X�V����
void MainScene::Update() {
	//�}�E�X�̍��N���b�N�����m�����ꍇ
	if (mouse->PushLeftBotton() == 1) {
		//�O�[��I�������ꍇ�̏���
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			gu->GetPos().x, gu->GetPos().y, gu->GetWidth(), gu->GetHeight()) == true) {

			SwitchingOperation(HandType::Gu, eScene::Result);

			return;
		}

		//�`���L��I�������ꍇ�̏���
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			choki->GetPos().x, choki->GetPos().y, choki->GetWidth(), choki->GetHeight()) == true) {

			SwitchingOperation(HandType::Choki, eScene::Result);

			return;
		}

		//�p�[��I�������ꍇ�̏���
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			pa->GetPos().x, pa->GetPos().y, pa->GetWidth(), pa->GetHeight()) == true) {

			SwitchingOperation(HandType::Pa, eScene::Result);

			return;
		}
	}
}

//�`�揈��
void MainScene::Draw() {
	//�I��������`�悷��
	gu->Draw(image);
	choki->Draw(image);
	pa->Draw(image);
}

/*******************************************************************************
���U���g�V�[���N���X�̏���
*******************************************************************************/

//���U���g�V�[���N���X�̃R���X�g���N�^
ResultScene::ResultScene(ISceneChanger* _sceneChanger, Image* _image, Mouse* _mouse,
	HandManager* _handManager, Font* _font)
	:Scene(_sceneChanger, _image, _mouse, _handManager, _font) {
	selectHand = new Hand(350, 50, 300, 300, _handManager->GetHand());
	comHand = new ComHand(316, 456);
	text = "";

	//���s����
	result = (static_cast<int>(selectHand->GetHandType()) -
		static_cast<int>(comHand->GetHandType()) + 3) % 3;
	//�\������e�L�X�g�����s�ɂ���Ċi�[����
	switch (static_cast<eResult>(result)) {
	case eResult::Lose:
		text = "�Ȃ�ŕ������������܂ōl���Ă�������";
		break;
	case eResult::Draw:
		text = "��������";
		break;
	case eResult::Win:
		text = "You Win!! Congratulation";
		break;
	}

	textPos.x = PosXCorrection(500, text);
	textPos.y = 400;
}

//�X�V����
void ResultScene::Update() {

	if (mouse->PushLeftBotton() == 1) {
		if (CollisionDetection(mouse->GetPos().x, mouse->GetPos().y, 0, 0,
			0, 50, 100, 50)) {
			nowScene = eScene::Main;
			sceneChanger->ChangeScene(eScene::Main, image, mouse, handManager, font);
		}
	}
}

//�`�揈��
void ResultScene::Draw() {
	//�v���C���[�̑I���������\��
	selectHand->Draw(image);
	//�R���s���[�^�[�̎��\��
	comHand->Draw(image);
	//���s�e�L�X�g��\��
	DrawStringToHandle(textPos.x, textPos.y, text.c_str(),
		GetColor(0, 0, 0), font->GetFont_Rogo_30_5());

	//�߂�{�^����UI
	DrawBox(0, 50, 100, 100, GetColor(125, 125, 125), TRUE);
	DrawStringToHandle(PosXCorrection(50, (string)"�߂�"), 75-15, "�߂�", GetColor(0, 0, 0),
		font->GetFont_Rogo_30_5());
}

/*******************************************************************************
�V�[���}�l�[�W���[�N���X�̏���
*******************************************************************************/

//�V�[���؂�ւ��̏���
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
�摜�N���X�̏���
*******************************************************************************/

//�摜�N���X�̃R���X�g���N�^
Image::Image() {
	MyLoadGraph("./res/Image/gu.png");
	MyLoadGraph("./res/Image/choki.png");
	MyLoadGraph("./res/Image/pa.png");
	MyLoadGraph("./res/Image/com_gu.png");
	MyLoadGraph("./res/Image/com_choki.png");
	MyLoadGraph("./res/Image/com_pa.png");
}

//�摜�ǂݍ��ݏ���
void Image::MyLoadGraph(const char* _filePath) {
	int tmp = LoadGraph(_filePath);
	images.push_back(tmp);
}

//�摜���\�[�X���폜����
void Image::Clear() {
	for (auto image : images) {
		DeleteGraph(image);
	}
	images.clear();
}

/*******************************************************************************
�t�H���g�N���X�̏���
*******************************************************************************/

//�t�H���g�N���X�̃R���X�g���N�^
Font::Font() {
	rogotype = "./res/Font/���S�����ՃS�V�b�N.otf";	//���S�����ՃS�V�b�N�̃t�@�C���p�X

	//�t�H���g��ǂݍ���
	AddFontResourceEx(rogotype, FR_PRIVATE, NULL);

	//�t�H���g���쐬
	rogo_30_5 = CreateFontToHandle("07���S�����ՃS�V�b�N7", 30, 5, DX_FONTTYPE_ANTIALIASING);
}

//�t�H���g�N���X�̃f�X�g���N�^
Font::~Font() {
	RemoveFontResource(rogotype);

	InitFontToHandle();
}

/*******************************************************************************
�}�E�X�N���X�̏���
*******************************************************************************/

//�}�E�X�N���X�̃R���X�g���N�^
Mouse::Mouse() {
	leftPressCount = 0;

	SetMousePoint(500, 500);
	GetMousePoint(&pos.x, &pos.y);
}

void Mouse::Update() {
	//�}�E�X�̍��W���擾����
	GetMousePoint(&pos.x, &pos.y);

	//�}�E�X�̍��{�^���������ꂽ�ꍇleftPressCount�����Z
	if (GetMouseInput() & MOUSE_INPUT_LEFT != 0) {
		leftPressCount++;
	}
	//������ĂȂ��ꍇ0�ɂ���
	else {
		leftPressCount = 0;
	}
}

/*******************************************************************************
��N���X�̏���
*******************************************************************************/

//��N���X�̃R���X�g���N�^
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
�R���s���[�^�[�n���h�N���X�̏���
*******************************************************************************/

//�R���s���[�^�[�n���h�N���X�̃R���X�g���N�^
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