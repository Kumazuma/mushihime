#pragma once
#include <list>
#include "GameObject.h"
#include "Bullet.h"

using namespace std;

class Player;
class PauseBox;
class BackGround;

class ObjectManager
{
private:
	ObjectManager();
	~ObjectManager();
	static void AddObject(ObjectType _type, std::shared_ptr<GameObject>);
public:
	static ObjectManager* GetInstance();
	template<typename ObjT, typename ...Arg>
	static std::shared_ptr<GameObject> CreateObject(ObjectType _type, Arg&& ...args);
	static std::shared_ptr<GameObject> CreateObject(ObjectType _type);
	static bool DeleteObject(const std::shared_ptr<GameObject>& _target);
	static void DestroyAll();
	static void DestroyAll(ObjectType _type);

	static void Release();
	static void Update();
	static void LateUpdate();
	static void Render();

	static void RenderBulletCount();
	static void RenderHP();
	
	std::weak_ptr<GameObject> pPlayer;
	PauseBox* pPauseUI = nullptr;
	BackGround* pBG = nullptr;

	list<std::shared_ptr<GameObject> >objectTable[ENUM_MAX<ObjectType>()];
	list<std::shared_ptr<GameObject> >objectList;
};

template<typename ObjT, typename ...Arg>
inline std::shared_ptr<GameObject> ObjectManager::CreateObject(ObjectType _type, Arg&& ...args)
{
	std::shared_ptr<GameObject> obj{ new ObjT{ std::forward<Arg>(args)... } };
	AddObject(_type, obj);
	return obj;
}
