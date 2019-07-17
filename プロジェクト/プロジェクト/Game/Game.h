#pragma once
#include <MyLib.h>
#include <memory>

#pragma comment(lib, "Lib.lib")

class Scene;

class Game
{
public:
	~Game();
	static Game& Get();

	// ������
	void Init();

	// �X�V
	void Update();

	// �`��
	void Draw();

	void Finish(void);

	// �V�[���؂�ւ�
	void ChangeScene(Scene* scene);
private:
	Game();
	Game(const Game&) = delete;
	void operator=(const Game&) = delete;
	
	// ���C�u����
	std::shared_ptr<MyLib> lib;

	// �V�[��
	std::shared_ptr<Scene> scene;
};
