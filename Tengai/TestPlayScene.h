#pragma once
#include"SceneManager.h"
#include<vector>
#include"GameObject.h"
#include"UI.h"
class PlayScene : public IScene
{
public:
    PlayScene();
    void OnLoaded() override;
    void OnUnloaded() override;
    void Update() override;
    virtual bool ShowBox(std::shared_ptr<UI>&) override;
    virtual bool HideBox() override;
    std::vector<std::pair<float,std::weak_ptr<GameObject> > > timeline;
    std::vector<std::pair<float,std::weak_ptr<GameObject> > >::const_iterator iter;
    std::shared_ptr<UI> pCurrentShowBox = nullptr;
    float time;
    float oldTime;
};