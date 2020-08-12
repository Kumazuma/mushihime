#pragma once
#include<memory>
#include<list>
#include"GameObject.h"
class Event;
struct EventQueueItem
{
	Event* pEvent;
	int objectUid;
	ObjectType objectType;
};
class EventManager
{
private:
	EventManager();
	~EventManager() = default;
private:
	static void Broadcast(Event* _event, ObjectType _type, int _uid = 0);
	static void Broadcast(const std::shared_ptr<GameObject>& obj, Event* _event);
public:
	static EventManager* GetInstance();
	static void Initialize();
	static void Release();
	static void LateUpdate();
	static void RegisterObject(GameObject* const pObject);
	static void UnregisterObject(GameObject* const pObject);
	//set ��������Ʈ��.
	//Ư���� ��ü�� ��
	template<typename EventT, typename ...Args>
	static void Broadcast(const std::shared_ptr<GameObject>& _obj, Args&&...args);
	template<typename EventT, typename ...Args>
	static void Broadcast(const std::shared_ptr<GameObject>& _obj);
	//Ư�� ������Ʈ Ÿ���� ��ü�鿡��
	template<typename EventT, typename ...Args>
	static void Broadcast(ObjectType _type, Args&&...args);
	template<typename EventT, typename ...Args>
	static void Broadcast(ObjectType _type);
public:
	std::vector<EventQueueItem> eventQueue;
	std::vector<Event*> events;
	//�޽����� �ޱ�� �� ��ü��
	
};
template<typename EventT, typename ...Args>
inline static void EventManager::Broadcast(const std::shared_ptr<GameObject>& _obj, Args&&...args)
{
	Event* _pEvent = static_cast<Event*>(new EventT{ std::forward<Args>(args) ... });
	Broadcast(_obj, _pEvent);
}
template<typename EventT, typename ...Args>
inline static void EventManager::Broadcast(const std::shared_ptr<GameObject>& _obj)
{
	Event* _pEvent = static_cast<Event*>(new EventT{  });
	Broadcast(_obj, _pEvent);
}
template<typename EventT, typename ...Args>
inline static void EventManager::Broadcast(ObjectType _type, Args&&...args)
{
	Event* _pEvent = static_cast<Event*>(new EventT{ std::forward<Args>(args) ... });
	Broadcast(_pEvent, _type, 0);
}
template<typename EventT, typename ...Args>
inline static void EventManager::Broadcast(ObjectType _type)
{
	Event* _pEvent = static_cast<Event*>(new EventT{  });
	Broadcast(_pEvent, _type, 0);
}