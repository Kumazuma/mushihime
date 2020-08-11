#pragma once
#include "Character.h"
#include "CharacterState.h"
#include <vector>
enum class MonsterType
{
	NONE = 0,
	MOB01,
	MOB02,
	BOSS,
	STAR,
	END
};

class Monster :
	public Character
{
	//method
public:
	Monster();
	static GameObject* Initialize(GameObject* const self, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos);
	void Initialize(MonsterType monsterType, const DirectX::XMFLOAT2& firstPos);
	~Monster();
	void Update() override;
	void Render() override;
	void OnShow(const Event&);
	void OnCollision(const CollisionEvent& event);
	//variable
public:
	RECT monsterRect;
	MonsterType monsterType;
	CharacterState* currentFireState;
	CharacterState* currentMoveState;
	std::vector<CharacterState*> moveStateList;
	std::vector<CharacterState*> fireStateList;
	DirectX::XMVECTOR m_P[3];
	DirectX::XMVECTOR m_Q[3];
};

