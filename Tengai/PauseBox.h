#pragma once
#include "UI.h"
#include "Transform.h"

class ResumeButton;
class ExitButton;

class PauseBox : public UI
{
private:
public:
	PauseBox();
	virtual ~PauseBox();

	virtual void Update() override;
	virtual void Render() override;

	void Show();
	void Hide();
	void Select();

	ResumeButton* pResumeButton = nullptr;
	ExitButton* pExitButton = nullptr;
	// false : resume / ture : Exit
	bool selector = false;
};

class ResumeButton : public UI
{
public:
	ResumeButton()
	{
		offset.x = 0.f;
		offset.y = -35.f;
		area = { -100,-30,100,30 };
		color = RGB(120, 120, 250);
	}

	void SetParent(GameObject* pObj)
	{
		pParent = pObj;
	}

	virtual void Update() override
	{
		if (pParent == nullptr) return;
		DirectX::FXMVECTOR parentPosition{ DirectX::XMLoadFloat2(&pParent->position) };
		DirectX::FXMMATRIX translateMatrix{ DirectX::XMMatrixTranslation(offset.x, offset.y, 0.f)};
		DirectX::FXMVECTOR myPosition{DirectX::XMVector4Transform(parentPosition, translateMatrix) };
		DirectX::XMStoreFloat2(&this->position, myPosition);
	}
	virtual void Render() override
	{
		if (pParent == nullptr) return;

		if (isFocused)
		{
			RenderManager::DrawRect(area + position, color);
		}
		else
		{
			RenderManager::DrawRect(area + position, RGB(120, 120, 120));
		}

		RenderManager::DrawString(L"계속하기",static_cast<int>(position.x - 27.5f), static_cast<int>(position.y) );
	}

	COLORREF color;
	GameObject* pParent = nullptr;
	DirectX::XMFLOAT2 offset;
	bool isFocused = true;
};

class ExitButton : public UI
{
public:
	ExitButton()
	{
		offset.x = 0.f;
		offset.y = 35.f;
		area = { -100,-30,100,30 };
		color = RGB(255, 120, 120);
	}
	void SetParent(GameObject* pObj)
	{
		pParent = pObj;
	}

	virtual void Update() override
	{
		if (pParent == nullptr) return;

		DirectX::FXMVECTOR parentPosition{ DirectX::XMLoadFloat2(&pParent->position) };
		DirectX::FXMMATRIX translateMatrix{ DirectX::XMMatrixTranslation(offset.x, offset.y, 0.f) };
		DirectX::FXMVECTOR myPosition{ DirectX::XMVector4Transform(parentPosition, translateMatrix) };
		DirectX::XMStoreFloat2(&this->position, myPosition);

	}
	virtual void Render() override
	{
		if (pParent == nullptr) return;

		if (isFocused)
		{
			
			RenderManager::DrawRect(area + position, color);
		}
		else
		{
			RenderManager::DrawRect(area + position, RGB(120, 120, 120));
		}
		
		RenderManager::DrawString(L"종료",static_cast<int>( position.x - 15),static_cast<int>( position.y) );
	}
	
	COLORREF color;
	GameObject* pParent = nullptr;
	DirectX::XMFLOAT2 offset;
	bool isFocused = false;
};

