#pragma once
#include "Monster.h"
class StarPart :
    public Monster
{
public:
	StarPart();
	static GameObject* Initialize(GameObject* const obj, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree);
	void Initialize(MonsterType _monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree);
	~StarPart();
	void Update() override;
	void Render() override;
	void OnShow(const Event&);
	void OnCollision(const CollisionEvent& event);
private:
	float m_BasicDegree = 0.f;
	float m_Degree = 0.f;
	DirectX::XMMATRIX m_Parent;
	MonsterType monsterType;
	CharacterState* currentFireState;
	CharacterState* currentMoveState;
	std::vector<CharacterState*> moveStateList;
	std::vector<CharacterState*> fireStateList;
};

