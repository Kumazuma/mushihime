#include "stdafx.h"
#include "Monster.h"
#include "RenderManager.h"
#include "item.h"
#include "GameWinBox.h"
Monster::Monster() 
{
	this->type = ObjectType::MONSTER;
	currentFireState = nullptr;
	currentMoveState = nullptr;
}
//우선 클리어
GameObject* Monster::Initialize(GameObject* const obj, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos)
{
	if (obj->type == ObjectType::MONSTER)
	{
		Monster* self = static_cast<Monster*>(obj);
		self->Initialize(monsterType, firstPos);
	}
	return obj;
}

void Monster::Initialize(const MonsterType _monsterType, const DirectX::XMFLOAT2& firstPos)
{
	Bind(EventId::PASS_TIME, &Monster::OnShow);
	Bind(EventId::COLLISION_OBJ, &Monster::OnCollision);
	position = firstPos;
	isEnable = false;
	for (auto it : fireStateList)
	{
		delete it;
	}
	fireStateList.clear();
	for (auto it : moveStateList)
	{
		delete it;
	}
	moveStateList.clear();
	switch (_monsterType)
	{
	case MonsterType::MOB01:
	{
		//화면 아래에서 나타나서 화면 위로 사라지는 몬스터
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 0.f} ,firstPos + DirectX::XMFLOAT2{ WINDOW_WIDTH / 2 , 1000} ,firstPos + DirectX::XMFLOAT2{ WINDOW_WIDTH ,  0} }
		);
		moveStateList.push_back(
			new DeleteState{ this }
		);
		fireStateList.push_back(
			new WaitState{ 0.1f });
		fireStateList.push_back(
			new FocusOnPlayerFireState{ this ,0.4f, 3.f });
		fireStateList.push_back(
			new WaitState{ 1 });
		speed = 100;
		moveStateList[0]->pNextState = moveStateList[1];

		fireStateList[0]->pNextState = fireStateList[1];
		fireStateList[1]->pNextState = fireStateList[2];
		fireStateList[2]->pNextState = fireStateList[0];
		monsterRect = RECT{ -18, -18, 18, 18 };
		colliders.push_back(RECT{ -16, -16, 16, 16 });
		hp = 1;
	}
	break;
	case MonsterType::MOB02:
	{
		//화면 오른쪽에서 나타나서 화면을 순회하는 몬스터
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 0.f} ,firstPos + DirectX::XMFLOAT2{ -1.f * WINDOW_WIDTH / 4, 700.f } ,firstPos + DirectX::XMFLOAT2{ 0.f,  400.f} }
		);
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 400.f} ,firstPos + DirectX::XMFLOAT2{ 1.f * WINDOW_WIDTH / 4 , 150.f } ,firstPos + DirectX::XMFLOAT2{ 0.f,  100.f} }
		);
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{ 0.f, 100.f} ,firstPos + DirectX::XMFLOAT2{ 1.f * WINDOW_WIDTH / 4,  150.f} ,firstPos + DirectX::XMFLOAT2{ 0.f,  400.f} }
		);
		fireStateList.push_back(
			new WaitState{ 0.1f });
		fireStateList.push_back(
			new FlowerFireState{ this ,3.f, 10.f });
		fireStateList.push_back(
			new FocusOnPlayerFireState{ this, 0.4f, 3.f });
		speed = 100;
		moveStateList[0]->pNextState = moveStateList[1];
		moveStateList[1]->pNextState = moveStateList[2];
		moveStateList[2]->pNextState = moveStateList[1];

		fireStateList[0]->pNextState = fireStateList[1];
		fireStateList[1]->pNextState = fireStateList[2];
		fireStateList[2]->pNextState = fireStateList[1];

		monsterRect = RECT{ -28, -28, 28, 28 };
		colliders.push_back(RECT{ -24, -24, 24, 24 });
		hp = 5;
	}
	break;
	case MonsterType::BOSS:
		//화면 오른쪽에서 나타나서 화면을 순회하는 몬스터
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 0.f} ,firstPos + DirectX::XMFLOAT2{-1.f * WINDOW_WIDTH / 4, 700.f} ,firstPos + DirectX::XMFLOAT2{0.f,  400.f} }
		);
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 400.f} ,firstPos + DirectX::XMFLOAT2{ 1.f * WINDOW_WIDTH / 4 , 150.f } ,firstPos + DirectX::XMFLOAT2{ 0.f,  100.f} }
		);
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{ 0.f, 100.f} ,firstPos + DirectX::XMFLOAT2{ 1.f * WINDOW_WIDTH / 4,  150.f} ,firstPos + DirectX::XMFLOAT2{ 0.f,  400.f} }
		);
		fireStateList.push_back(
			new WaitState{ 0.1f });
		fireStateList.push_back(
			new FlowerFireState{ this ,2.f, 6.f });
		fireStateList.push_back(
			new FocusOnPlayerFireState{ this, 0.4f, 3.f });
		fireStateList.push_back(
			new FlowerCurvesFireState{ this, 0.2f, 3.f });
		fireStateList.push_back(
			new FocusOnPlayerFireState{ this, 0.4f, 3.f });
		speed = 50;
		moveStateList[0]->pNextState = moveStateList[1];
		moveStateList[1]->pNextState = moveStateList[2];
		moveStateList[2]->pNextState = moveStateList[1];

		fireStateList[0]->pNextState = fireStateList[1];
		fireStateList[1]->pNextState = fireStateList[2];
		fireStateList[2]->pNextState = fireStateList[3];
		fireStateList[3]->pNextState = fireStateList[4];
		fireStateList[4]->pNextState = fireStateList[1];

		monsterRect = RECT{ -36, -36, 36, 36 };
		colliders.push_back(RECT{ -32, -32, 32, 32 });
		hp = 10;
		break;
	case MonsterType::STAR:
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{0.f, 0.f} ,firstPos + DirectX::XMFLOAT2{-200.f, -1.f * WINDOW_HEIGHT / 4 } ,firstPos + DirectX::XMFLOAT2{-400.f,  0.f} }
		);
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{-400.f, 0.f} ,firstPos + DirectX::XMFLOAT2{-150.f, 1.f * WINDOW_HEIGHT / 4 } ,firstPos + DirectX::XMFLOAT2{-100.f,  0.f} }
		);
		moveStateList.push_back(
			new BezierCurveMoveToState{ this, firstPos + DirectX::XMFLOAT2{-100.f, 0.f} ,firstPos + DirectX::XMFLOAT2{-150.f, -1.f * WINDOW_HEIGHT / 4 } ,firstPos + DirectX::XMFLOAT2{-400.f,  0.f} }
		);
		fireStateList.push_back(
			new WaitState{ 0.1f });
		fireStateList.push_back(
			new FlowerFireState{ this ,2.f, 6.f });
		fireStateList.push_back(
			new FocusOnPlayerFireState{ this, 0.4f, 3.f });
		fireStateList.push_back(
			new FlowerCurvesFireState{ this, 0.2f, 3.f });
		fireStateList.push_back(
			new FocusOnPlayerFireState{ this, 0.4f, 3.f });
		speed = 50;
		moveStateList[0]->pNextState = moveStateList[1];
		moveStateList[1]->pNextState = moveStateList[2];
		moveStateList[2]->pNextState = moveStateList[1];

		fireStateList[0]->pNextState = fireStateList[1];
		fireStateList[1]->pNextState = fireStateList[2];
		fireStateList[2]->pNextState = fireStateList[3];
		fireStateList[3]->pNextState = fireStateList[4];
		fireStateList[4]->pNextState = fireStateList[1];

		DirectX::XMMATRIX matStar, Scale, RotZ, Trans, RevZ, Parent;
		matStar = DirectX::XMMatrixIdentity();
		Scale = DirectX::XMMatrixScaling(1.f, 1.f, 0.f);
		RotZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f));
		Trans = DirectX::XMMatrixTranslation(0.f, -50.f, 0.f);
		RevZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f));
		Parent = DirectX::XMMatrixTranslation(position.x, position.y, 0.f);
		matStar = Scale * RotZ * Trans * RevZ * Parent;
		
		auto s{ DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f), Parent) };
		DirectX::XMStoreFloat2(&position, s);

		monsterRect = RECT{ -36, -36, 36, 36 };
		colliders.push_back(RECT{ -32, -32, 32, 32 });
		hp = 10;
		break;
	}
	monsterType = _monsterType;
	simpleCollider = CreateSimpleCollider(colliders);
	currentMoveState = this->moveStateList.front();
	currentFireState = this->fireStateList.front();
	currentMoveState->Reset();
	currentFireState->Reset();
}

Monster::~Monster()
{
	for (auto it : fireStateList)
	{
		delete it;
	}
	fireStateList.clear();
	for (auto it : moveStateList)
	{
		delete it;
	}
	moveStateList.clear();
}

void Monster::Update()
{
	if (isEnable == false)
	{
		return;
	}
	if (currentMoveState != nullptr)
	{
		if (currentMoveState->Update())
		{
			auto pNext = currentMoveState->pNextState;
			if (pNext != nullptr)
			{
				pNext->Reset();
				currentMoveState = pNext;
			}
		}
	}
	if (currentFireState != nullptr)
	{
		if (currentFireState->Update())
		{
			auto pNext = currentFireState->pNextState;
			if (pNext != nullptr)
			{
				pNext->Reset();
				currentFireState = pNext;
			}
		}
	}
	if (monsterType == MonsterType::STAR)
	{
		position.x += 1;
	}
}

void Monster::Render()
{
	if (monsterType == MonsterType::STAR)
	{
		DirectX::XMVECTOR triangle[3]{
			{0.f, -20.f, 0.f },
			{ -20.f, 20.f , 0.f },
			{ 20.f, 20.f, 0.f }
		};
		auto scale{ DirectX::XMMatrixScaling(1.f, 1.f, 0.f) };
		auto rot{ DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(0.f)) };
		auto trans{ DirectX::XMMatrixTranslation(position.x, position.y, 0.f) };
		auto transform = scale * rot * trans;
		for (int i = 0; i < 3; ++i)
		{
			triangle[i] = DirectX::XMVector3TransformCoord(triangle[i], transform);
		}
		RenderManager::DrawTriangle(triangle);
	}
	else
		RenderManager::DrawSimpleCollider(monsterRect + position, RGB(0, 200, 0));
}

void Monster::OnShow(const Event&)
{
	isEnable = true;
}

void Monster::OnCollision(const CollisionEvent& event)
{
	if (event.other->type == ObjectType::BULLET && this->isEnable)
	{
		const Bullet* const pBullet = (const Bullet*) event.other;
		hp -= pBullet->isAlias;
		if (hp <= 0)
		{
			Die();
			if (monsterType == MonsterType::BOSS)
			{
				GameObject::Die();
				UI* ui = (UI*)ObjectManager::CreateObject<GameWinBox>(ObjectType::UI);
				if (SceneManager::GetInstance()->pCurrentScene->ShowBox(ui) == false)
				{
					SceneManager::GetInstance()->pCurrentScene->HideBox();
					SceneManager::GetInstance()->pCurrentScene->ShowBox(ui);
				}
				ui->Show();
				MainGame::Pause();
				return;
			}
			//아이템을 생성한다.
			ItemType itemType = ItemType::NONE;
			switch (rand() % 2)
			{
			case 0:
				itemType = ItemType::HEAL;
				break;
			case 1:
				itemType = ItemType::SKILL;
				break;
			}
			Item::Initialize(ObjectManager::CreateObject(ObjectType::ITEM), itemType, position);
		}
	}
}
