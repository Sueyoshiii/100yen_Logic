#pragma once
#include "Scene.h"

class Main :
	public Scene
{
public:
	// �R���X�g���N�^
	Main(std::weak_ptr<MyLib>lib);
	// �f�X�g���N�^
	~Main();

	// �`��
	void Draw(void);
	
	// ����
	void UpData(void);

private:

};
