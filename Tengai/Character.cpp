#include "stdafx.h"
#include "Character.h"

void Character::Update()
{
	
}

void Character::Render()
{
}

bool Character::Collision(const std::shared_ptr<GameObject>& _target)
{
	
	auto target{ std::dynamic_pointer_cast<const Character>(_target) };
	if (target == nullptr) return false;
	if (target->type == type) return false;

	RECT myBox = this->simpleCollider + this->position;
	RECT targetBox = target->simpleCollider + target->position;
	RECT outBox;

	if (!IntersectRect(&outBox, &myBox, &targetBox)) return false;

	for (const RECT& collider : colliders)
	{
		auto relativedCollider = collider + position;

		for (const RECT& targetCollider : target->colliders)
		{
			auto relativedTargetCollider = targetCollider + target->position;

			if (IntersectRect(&outBox, &relativedCollider, &relativedTargetCollider))
			{
				return true;
			}
		}
	}
	


	return false;
}



