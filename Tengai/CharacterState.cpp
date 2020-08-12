#include "stdafx.h"
#include "CharacterState.h"
#include "TimeManager.h"
#include "Bullet.h"
BezierCurveMoveToState::BezierCurveMoveToState(Character* const _pCharacter, const DirectX::XMFLOAT2& _start, const DirectX::XMFLOAT2& _center, const DirectX::XMFLOAT2& _dest) :
	MoveToState{ _pCharacter, _start, _dest }, center{ _center }
{

}
bool BezierCurveMoveToState::Update()
{
	const float _speed =TimeManager::DeltaTime()* pCharacter->speed / 10.f;
	for (int i = 0; i < 10; ++i)
	{
		//���α׷����� 1�� �ǰų�, ���������� ����������
		DirectX::XMVECTOR destination{DirectX::XMLoadFloat2(&this->destination)};
		DirectX::XMVECTOR characterPosition{ DirectX::XMLoadFloat2(&pCharacter->position) };
		float length{ DirectX::XMVectorGetByIndex(DirectX::XMVector2Length(DirectX::XMVectorSubtract(destination, characterPosition)), 0) };
		if (progress >= 0.75f && length < _speed)
		{
			pCharacter->position = this->destination;
			//�̸� ������ ���� ���� ��ȯ�Ѵ�.
			return true;
		}
		DirectX::XMVECTOR next{ XMLoadFloat2(&this->next) };
		length = DirectX::XMVectorGetByIndex(DirectX::XMVector2Length(DirectX::XMVectorSubtract(next, characterPosition)), 0);
		//���� ��ġ���� �����ϸ�, ���� ��ġ�� ����Ѵ�.
		if (length < _speed)
		{
			pCharacter->position = this->next;
			progress += 0.001f;
			float restTime = 1.f - progress;//(1-t)
			//2�� ������ ��� ���� ���
			DirectX::XMVECTOR start{ DirectX::XMLoadFloat2(&this->start) };
			DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&this->center) };
			//(1-t){(1-t)A + tB} + +t{(1-t)B + tC}
			next =
				restTime * (restTime * start + progress * center) +
				progress * (restTime * center + progress * destination);
			DirectX::XMStoreFloat2(&this->next, next);
		}
		//���� ��ǥ�� ��Ÿ���� ���Ϳ� ���� ��ǥ�� ��Ÿ���� ���͸� ����, �� ���ͻ��̸� �մ� ���Ͱ� ���´�.
		//�׸��� �� ���͸� ����ȭ(normalize)��Ű�� �������� ����Ű�� ���⺤�Ͱ� �ȴ�.
		DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(next - DirectX::XMLoadFloat2(&pCharacter->position));
		DirectX::XMFLOAT2 xy{};
		DirectX::XMStoreFloat2(&xy, _speed * direction);
		DirectX::XMMATRIX mat{ DirectX::XMMatrixTranslation(xy.x, xy.y, 0.f) };
		characterPosition = DirectX::XMVector3Transform(characterPosition, mat);
		DirectX::XMStoreFloat2(&pCharacter->position, characterPosition);

	}
	return false;
}

void BezierCurveMoveToState::Reset()
{
	MoveToState::Reset();
	next = start;
}

WaitState::WaitState(float _time) :
	time{ _time }, current{ 0 }
{

}

void WaitState::Reset()
{
	current = 0;
}

bool WaitState::Update()
{
	if (current >= time)
	{
		return true;
	}
	current += TimeManager::DeltaTime();
	return false;
}

MoveToState::MoveToState(Character* const _pCharecter, const DirectX::XMFLOAT2& _start, const DirectX::XMFLOAT2& _dest) :
	pCharacter{ _pCharecter }, progress {0.f}, start{ _start }, destination{ _dest }
{
}

void MoveToState::Reset()
{
	progress = 0.0f;
}

LinearMoveToState::LinearMoveToState(Character* const _pCharecter, const DirectX::XMFLOAT2& _start, const DirectX::XMFLOAT2& _dest) :
	MoveToState{ _pCharecter, _start, _dest }
{
}

bool LinearMoveToState::Update()
{
	
	const float _speed = TimeManager::DeltaTime() * pCharacter->speed / 10.f;
	for (int i = 0; i < 10; ++i)
	{
		DirectX::XMVECTOR destination{ DirectX::XMLoadFloat2(&this->destination) };
		DirectX::XMVECTOR characterPosition{ DirectX::XMLoadFloat2(&pCharacter->position) };
		float length{ DirectX::XMVectorGetByIndex(DirectX::XMVector2Length(DirectX::XMVectorSubtract(destination, characterPosition)), 0) };
		//���α׷����� 1�� �ǰų�, ���������� ����������
		if (progress >= 0.75f && length < _speed)
		{
			pCharacter->position = this->destination;
			//�̸� ������ ���� ���� ��ȯ�Ѵ�.
			return true;
		}
		DirectX::XMVECTOR direction = DirectX::XMVector3Normalize(destination - DirectX::XMLoadFloat2(&pCharacter->position));
		DirectX::XMFLOAT2 xy{};
		DirectX::XMStoreFloat2(&xy, _speed * direction);
		DirectX::XMMATRIX mat{ DirectX::XMMatrixTranslation(xy.x, xy.y, 0.f) };
		characterPosition = DirectX::XMVector3Transform(characterPosition, mat);
		DirectX::XMStoreFloat2(&pCharacter->position, characterPosition);
	}
	return false;
}

FocusOnPlayerFireState::FocusOnPlayerFireState(Character* _pCharacter, float _interval,  float _time):
	FireState{ _pCharacter, _interval,  _time }
{

}

bool FocusOnPlayerFireState::Update()
{
	do
	{
		if (tick < interval)
		{
			break;
		}
		//���
		Character const * const pPlayer = (Character*)ObjectManager::GetInstance()->pPlayer;
		if (pPlayer == nullptr)
		{
			break;
		}
		DirectX::XMVECTOR dpos =
			DirectX::XMVector3Normalize(DirectX::XMLoadFloat2(&pPlayer->position) - DirectX::XMLoadFloat2(&pCharacter->position));
		//���� ���͸� �ϳ� �ӽ÷� �����Ѵ�.
		DirectX::XMVECTOR forward{ DirectX::XMVectorSet(1.f,0.f,0.f,0.f) };
		//�� ���� ������ ��Ʈ��(����)�� �� ������ ���� ���� Cos�̴� 
		auto tmp{ DirectX::XMVector3Dot(dpos, forward) };
		float cosV = DirectX::XMVectorGetZ(tmp);
		float radian = acosf(cosV);
		if (pPlayer->position.y < pCharacter->position.y)
		{
			radian *= -1.f;
		}
		GameObject* bullet = ObjectManager::CreateObject<Bullet010>(ObjectType::BULLET);
		MetaBullet::Initialize(bullet, BulletType::_10/*_01*/, pCharacter->position, radian, false);
		tick -= interval;
	} while (false);
	return FireState::Update();
}


FireState::FireState(Character* pCharacter, float _interval, float _time):
	pCharacter{ pCharacter }, interval{ _interval }, tick{ 0.f }, current{_time}, time{_time}
{
}

bool FireState::Update()
{
	auto delta = TimeManager::DeltaTime();
	if (current <= delta)
	{
		return true;
	}
	current -= delta;
	tick += delta;
	return false;
}

void FireState::Reset()
{
	tick = 0.f;
	current = time;
}

FlowerFireState::FlowerFireState(Character* pCharacter, float _interval, float _time):
	FireState{ pCharacter, _interval, _time }
{
}

bool FlowerFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };
		//auto move = Matrix::Translate(length, 0);
		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_03, pos, radian, false);
		}
		tick -= interval;
	}
	return FireState::Update();
}

FlowerCurvesFireState::FlowerCurvesFireState(Character* pCharacter, float _interval,  float _time):
	FireState{ pCharacter, _interval , _time }
{
}

bool FlowerCurvesFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };
		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_02, pos, radian, false); 
		}
		tick -= interval;
	}
	return FireState::Update();
}

DeleteState::DeleteState(Character* const _pCharacter):
	pCharacter{ _pCharacter }
{

}

bool DeleteState::Update()
{
	pCharacter->Die();
	return false;
}

void DeleteState::Reset()
{
}

CharacterState::CharacterState():
	pNextState{nullptr}
{
}

PlayerBasicAttackState::PlayerBasicAttackState(Character* pCharacter, float _interval, float _time):
	FireState{pCharacter, _interval, _time}
{

}


PlayerAdditionalAttackState::PlayerAdditionalAttackState(Character* pCharacter, float _interval, float _time):
	PlayerBasicAttackState{ pCharacter, _interval, _time }
{
	
}
bool PlayerBasicAttackState::Update()
{

	// ����Ű
	if (InputManager::GetKey('A'))
	{
		if (tick >= interval)
		{
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_04, pCharacter->position, -1.f * 90.f * 3.14f / 180, true);
			tick = 0;
		}
		
	}
	FireState::Update();
	return false;
}

bool PlayerAdditionalAttackState::Update()
{
	if (InputManager::GetKey('A'))
	{
		if (tick >= interval)
		{
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_05, pCharacter->position, 1.5f, true);
		}
	}
	PlayerBasicAttackState::Update();
	return false;
}

UniqueFlowerFireState::UniqueFlowerFireState(Character* pCharacter, float _interval, float _time):
	FireState{ pCharacter, _interval, _time }
{
}

bool UniqueFlowerFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };

		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject<Bullet07>(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_07, pos, radian, true);
		}
		tick -= interval;
	}
	return FireState::Update();
}

UniqueWarmFireState::UniqueWarmFireState(Character* pCharacter, float _interval, float _time):
	FireState{ pCharacter, _interval, _time }
{
}

bool UniqueWarmFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };

		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject<Bullet08>(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_08, pos, radian, true);
		}
		tick -= interval;
	}
	return FireState::Update();
}

ZigzagWarmFireState::ZigzagWarmFireState(Character* pCharacter, float _interval, float _time) :
	FireState{ pCharacter, _interval, _time }
{
}

bool ZigzagWarmFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };

		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject<Bullet09>(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_09, pos, radian, true);
		}
		tick -= interval;
	}
	return FireState::Update();
}

GuidedBulletFireState::GuidedBulletFireState(Character* pCharacter, float _interval, float _time):
	FireState{ pCharacter, _interval, _time }
{
}

bool GuidedBulletFireState::Update()
{
	do
	{
		if (tick < interval)
		{
			break;
		}
		//���
		Character const* const pPlayer = (Character*)ObjectManager::GetInstance()->pPlayer;
		if (pPlayer == nullptr)
		{
			break;
		}
		DirectX::XMVECTOR dpos =
			DirectX::XMVector3Normalize(DirectX::XMLoadFloat2(&pPlayer->position) - DirectX::XMLoadFloat2(&pCharacter->position));
		//���� ���͸� �ϳ� �ӽ÷� �����Ѵ�.
		DirectX::XMVECTOR forward{ DirectX::XMVectorSet(1.f,0.f,0.f,0.f) };
		//�� ���� ������ ��Ʈ��(����)�� �� ������ ���� ���� Cos�̴� 
		auto tmp{ DirectX::XMVector3Dot(dpos, forward) };
		float cosV = DirectX::XMVectorGetZ(tmp);
		float radian = acosf(cosV);
		if (pPlayer->position.y < pCharacter->position.y)
		{
			radian *= -1.f;
		}
		GameObject* bullet = ObjectManager::CreateObject<Bullet010>(ObjectType::BULLET);
		MetaBullet::Initialize(bullet, BulletType::_10, pCharacter->position, radian, false);
		tick -= interval;
	} while (false);
	return FireState::Update();
}

ReverseFlowerCurvesFireState::ReverseFlowerCurvesFireState(Character* pCharacter, float _interval, float _time):
	FireState{ pCharacter, _interval, _time }
{
}

bool ReverseFlowerCurvesFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };
		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_11, pos, radian, false);
		}
		tick -= interval;
	}
	return FireState::Update();
}

CrossCurvesFireState::CrossCurvesFireState(Character* pCharacter, float _interval, float _time)
	:
	FireState{ pCharacter, _interval, _time }
{
}

bool CrossCurvesFireState::Update()
{
	if (tick >= interval)
	{
		float length{
			DirectX::XMVectorGetX(
			DirectX::XMVector2Length(DirectX::XMVectorSet((float)pCharacter->simpleCollider.left, (float)pCharacter->simpleCollider.top, 0.f, 0.f))) };
		const DirectX::XMVECTOR center{ DirectX::XMLoadFloat2(&pCharacter->position) };
		DirectX::XMMATRIX parent{ DirectX::XMMatrixTranslationFromVector(center) };
		constexpr int BULLET_COUNT = 22;
		//���
		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_11, pos, radian, false);

			
		}

		for (int i = 0; i < BULLET_COUNT; ++i)
		{
			const float radian = PI * i * (360 / BULLET_COUNT) / 180;
			DirectX::XMVECTOR v{};
			DirectX::XMFLOAT2 pos{};
			auto mat = DirectX::XMMatrixTranslation(length, 0.f, 0.f) * DirectX::XMMatrixRotationZ(radian) * parent;
			v = DirectX::XMVector3Transform(v, mat);
			DirectX::XMStoreFloat2(&pos, v);
			GameObject* bullet = ObjectManager::CreateObject(ObjectType::BULLET);
			MetaBullet::Initialize(bullet, BulletType::_02, pos, radian, false);
		}

		tick -= interval;
	}
	return FireState::Update();
}
