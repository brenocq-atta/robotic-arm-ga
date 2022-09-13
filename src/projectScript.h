//--------------------------------------------------
// Game of life
// projectScript.h
// Date: 2022-09-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/pch.h>
#include <atta/script/projectScript.h>
#include "common.h"

class Project : public atta::script::ProjectScript {
  public:
    void onLoad() override;

    void onStart() override;
    void onStop() override;

    void onUpdateBefore(float dt) override;
    void onUIRender() override;

  private:
    void init();
    void clear();
    void createArm(cmp::Entity arm);
    void updateArm(cmp::Entity arm);

    bool _running;
    std::vector<cmp::Entity> _toDelete;
};

ATTA_REGISTER_PROJECT_SCRIPT(Project)

#endif // PROJECT_SCRIPT_H
