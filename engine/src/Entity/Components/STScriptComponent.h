#ifndef WAHOO_STSCRIPTCOMPONENT_H
#define WAHOO_STSCRIPTCOMPONENT_H

#include "STComponent.h"
#include "../STEntity.h"

#if __MINGW32__
#include <sol.hpp>
#elif __linux__
#include <lua5.1/sol.hpp>
#else
#include <sol.hpp>
#endif

class STEntity;

class STScriptComponent : public STComponent{
public:
    STScriptComponent();
    STScriptComponent(STEntity* entity, const std::string& fileName);

    explicit STScriptComponent(const std::string& fileName);
    void init(std::shared_ptr<STEntity>& parent) override;
    void registerFunction(const std::string& functionName, std::function<void()> newFunction);
    void registerEvent(STEntity* self, const std::string& eventName);
    ~STScriptComponent() override;

    void update() override;

    void save(std::ofstream& out) override;
    void load(std::ifstream& in) override;
    sol::state m_script;
private:
    void initScript(const std::string& fileName);
    std::string scriptName;
};

#endif //WAHOO_STSCRIPTCOMPONENT_H
