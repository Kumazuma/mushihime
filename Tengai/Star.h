#pragma once
#include "Monster.h"
class Star :
    public Monster
{
public:
	Star();
	static std::shared_ptr<GameObject> Initialize(const std::shared_ptr<GameObject>& self, MonsterType monsterType, const DirectX::XMFLOAT2& firstPos);
	void Initialize(MonsterType monsterType, const DirectX::XMFLOAT2& firstPos);
	~Star();
	void Update() override;
	void Render() override;
	void OnShow(const Event&);
	void OnCollision(const CollisionEvent& event);
	//variable
private:
	std::vector<std::shared_ptr<GameObject> > m_vecParts;
};
