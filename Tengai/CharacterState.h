#pragma once
#include "Transform.h"
#include "Character.h"
//ĳ������ ����, �̵� �� �Ѿ� �߻� ó���� �����Ѵ�.
struct CharacterState
{
	CharacterState();
	virtual bool Update() = 0;
	virtual void Reset() = 0;
	virtual ~CharacterState() = default;
	CharacterState* pNextState;
};
class MoveToState :public CharacterState
{
public:
	MoveToState(Character* const _pCharecter, const DirectX::XMFLOAT2& _start, const DirectX::XMFLOAT2& _dest);
	void Reset() override;
	float progress;/*[0 - 1]*/
	Character* pCharacter;
	DirectX::XMFLOAT2 destination;
	DirectX::XMFLOAT2 start;
};
//Ư���� ��ġ�� �������� �̵��Ѵ�.
class LinearMoveToState : public MoveToState
{
public:
	LinearMoveToState(Character* const _pCharecter, const DirectX::XMFLOAT2& _start, const DirectX::XMFLOAT2& _dest);
	bool Update() override;
};
//Ư���� ��ġ�� ������ ��� �̿��Ͽ� �̵��Ѵ�.
class BezierCurveMoveToState :public MoveToState
{
public:
	BezierCurveMoveToState(Character* const _pCharecter, const DirectX::XMFLOAT2& _start, const DirectX::XMFLOAT2& _center, const DirectX::XMFLOAT2& _dest);
	bool Update() override;
	void Reset() override;
	DirectX::XMFLOAT2 center;
	DirectX::XMFLOAT2 next;
};
class DeleteState : public CharacterState
{
public:
	DeleteState(Character* const _pCharacter);
	bool Update() override;
	void Reset() override;
	Character* const pCharacter;
};
//�ش� ��ġ�� ����ϴ� ����
class WaitState :public CharacterState
{
public:

	//-1�� infinite
	WaitState(float _time);
	void Reset() override;
	bool Update() override;
	float time;
	float current;
};
class FireState: public CharacterState
{
public:
	FireState(Character* pCharacter, float _interval,  float _time = 9999.f);
	bool Update() override;
	void Reset() override;
	Character* pCharacter;
	float tick;
	const float interval;
	const float time;
	float current;
};
//���ΰ��� �ִ� �������� �ѹ��� �߻��ϴ� �����̺��
class FocusOnPlayerFireState : public FireState
{
public:
	FocusOnPlayerFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};
//���� �ð����� �÷��̾ �Ѿư��� ����ź�� �߻��ϴ� �����̺��
class GuidedBulletFireState : public FireState
{
public:
	GuidedBulletFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};
//��������� ��� �Լ�
class FlowerFireState : public FireState
{
public:
	FlowerFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};
//��������� ���ݾ� ���� �Ѿ��� ��� �Լ�
class FlowerCurvesFireState : public FireState
{
public:
	FlowerCurvesFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};
//��������� ���ݾ� ���� �Ѿ��� ��� �Լ�
class ReverseFlowerCurvesFireState : public FireState
{
public:
	ReverseFlowerCurvesFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};

class CrossCurvesFireState : public FireState
{
public:
	CrossCurvesFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};

class UniqueFlowerFireState : public FireState
{
public:
	UniqueFlowerFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};

class UniqueWarmFireState : public FireState
{
public:
	UniqueWarmFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};

class ZigzagWarmFireState : public FireState
{
public:
	ZigzagWarmFireState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};

//�÷��̾��� �⺻ ����
class PlayerBasicAttackState : public FireState
{
public:
	PlayerBasicAttackState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};
class PlayerAdditionalAttackState : public PlayerBasicAttackState
{
public:
	PlayerAdditionalAttackState(Character* pCharacter, float _interval, float _time = 9999.f);
	bool Update() override;
};