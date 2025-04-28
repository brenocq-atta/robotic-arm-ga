//--------------------------------------------------
// Game of life
// projectScript.h
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include "common.h"
#include <atta/script/projectScript.h>

class Project : public scr::ProjectScript {
  public:
    void onLoad() override;
    void onUnload() override;

    void onStart() override;
    void onStop() override;

    void onUpdateBefore(float dt) override;
    void onUIRender() override;

  private:
    void init();
    void clear();
    cmp::Entity createArm();
    void updateArmJoints(cmp::Entity arm);
    void evolve();

    bool _running;
    std::vector<cmp::Entity> _arms;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
