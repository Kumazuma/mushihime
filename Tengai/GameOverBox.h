#pragma once
#include "UI.h"
class GameOverBox :
	public UI
{
public:
	GameOverBox();
	virtual ~GameOverBox();
	virtual void Update() override;
	virtual void Render() override;

	void Show();
	void Hide();
};

