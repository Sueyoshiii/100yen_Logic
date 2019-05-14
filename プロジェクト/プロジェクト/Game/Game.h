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

	// �V�[���؂�ւ�
	void ChangeScene(Scene* scene);
private:
	Game();
	Game(const Game&) = delete;
	void operator=(const Game&) = delete;

	std::shared_ptr<MyLib> lib;
	std::shared_ptr<Scene> scene;
};
