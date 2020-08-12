#pragma once
#include "Monster.h"
class StarPart :
    public Monster
{
public:
	StarPart();
	static std::shared_ptr<GameObject> Initialize(const std::shared_ptr<GameObject>& obj, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree);
	void Initialize(MonsterType _monsterType, const DirectX::XMFLOAT2& firstPos, const float BasicDegree);
	~StarPart();
	void Update() override;
	void Render() override;
	void OnShow(const Event&);
	void OnCollision(const CollisionEvent& event);
private:
	float m_BasicDegree = 0.f;
	float m_Degree = 0.f;
};

