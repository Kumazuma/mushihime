#include "stdafx.h"
#include "TestPlayScene.h"
#include "Monster.h"
#include "MainGame.h"
#include "PauseBox.h"
#include "GameOverBox.h"
#include "Star.h"
#include "StarPart.h"
PlayScene::PlayScene():
    pCurrentShowBox{nullptr}
{
    time = 0.f;
    oldTime = 0.0f;
}
void PlayScene::OnLoaded()
{
    time = 0.f;
    oldTime = 0.0f;
    ObjectManager::CreateObject(ObjectType::PLAYER);
    timeline = {
        /*{ 1  ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::BOSS, Transform{ WINDOW_WIDTH + 32, WINDOW_HEIGHT / 2 }) }*/
        { 0 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32 }) },

        { 1 ,Star::Initialize(ObjectManager::CreateObject<Star>(ObjectType::MONSTER), MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32 }) },
                
        { 2 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32 }) },
        { 4 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32 }) },
        { 4 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32 }) },
        { 6 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32  }) },
        { 8 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -32  }) },
        { 9 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02,  DirectX::XMFLOAT2{ WINDOW_WIDTH / 2 + 100 , -32 }) },
        { 10 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 11 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 16 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 20 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02, DirectX::XMFLOAT2{ WINDOW_WIDTH /2 - 10, -32  }) },
        { 24 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02, DirectX::XMFLOAT2{ WINDOW_WIDTH /2 + 10, -32  }) },
        { 28 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02, DirectX::XMFLOAT2{ WINDOW_WIDTH /2 - 20, -32  }) },
        { 32  ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02,DirectX::XMFLOAT2{ WINDOW_WIDTH /2 + 20, -32  }) },
        { 35 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 37 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 39 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 41 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 45 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32 }) },
        { 47 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02, DirectX::XMFLOAT2{ WINDOW_WIDTH / 2 + 100 , -32 }) },
        { 47 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32  }) },
        { 49 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32  }) },
        { 50 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB02, DirectX::XMFLOAT2{ WINDOW_WIDTH / 2 - 100, -32 }) },
        { 50 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32  }) },
        { 51 ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::MOB01, DirectX::XMFLOAT2{ WINDOW_WIDTH, -32  }) },
        { 60  ,Monster::Initialize(ObjectManager::CreateObject(ObjectType::MONSTER),MonsterType::BOSS, DirectX::XMFLOAT2{ WINDOW_WIDTH / 2, -40}) }
    };
    iter = timeline.begin();
    MainGame::Resume();
}

void PlayScene::OnUnloaded()
{
    ObjectManager::DestroyAll();
}
//#1 백그라운드가 플레이어나 몬스터나 유닛보다 아래에 있어야 한다.
//#2 백그라운드가 이벤트를 받아서
void PlayScene::Update()
{
    if (MainGame::GetInstance()->isPause == false)
    {
        float currentTime = time + TimeManager::DeltaTime();
        float prevTime = oldTime;
        time = currentTime;
        if (currentTime == prevTime)
        {
            return;
        }
        for (; iter != timeline.end(); ++iter)
        {
            if (prevTime <= iter->first && iter->first <= currentTime)
            {
                oldTime = time;
                EventManager::Broadcast< TimePassedEvent>(iter->second.lock(), currentTime);
            }
            else
            {
                break;
            }
        }
    }
    else if (pCurrentShowBox != nullptr)
    {
        pCurrentShowBox->Update();
    }
    if (InputManager::GetKeyDown(VK_ESCAPE))
    {
        if (MainGame::GetInstance()->isPause == false)
        {
            MainGame::Pause();
            auto box = std::static_pointer_cast<UI>(ObjectManager::CreateObject<PauseBox>(ObjectType::UI));
            if (ShowBox(box))
            {
                box->Show();
            }
            else
            {
                box->Die();
            }
        }
        else if(std::dynamic_pointer_cast<PauseBox>(pCurrentShowBox) != nullptr)
        {
            HideBox();
        }
    }
}

bool PlayScene::ShowBox(std::shared_ptr<UI>& ptr)
{
    if (pCurrentShowBox == nullptr)
    {
        pCurrentShowBox = ptr;
        return true;
    }
    return false;
}

bool PlayScene::HideBox()
{
    if (pCurrentShowBox != nullptr)
    {
        pCurrentShowBox->Hide();
    }
    pCurrentShowBox = nullptr;
    return true;
}
