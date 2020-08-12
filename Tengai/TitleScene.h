#pragma once
#include "SceneManager.h"
class TitleScene : public IScene
{
public:
	TitleScene();
	~TitleScene();
	// IScene��(��) ���� ��ӵ�
	virtual void OnLoaded() override;
	virtual void OnUnloaded() override;
	virtual void Update() override;

	std::shared_ptr<GameObject> titleUI = nullptr;
	std::shared_ptr<GameObject> pressUI = nullptr;
};

