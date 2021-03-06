#ifndef SWINGTECH1_ST3DANIMATIONCOMPONENT_H
#define SWINGTECH1_ST3DANIMATIONCOMPONENT_H

#include <sol.hpp>


#include "STComponent.h"
#include "STRendererComponent.h"
#include "../STEntity.h"

class STComponent;

/**
 * @brief Handle Animation of Character(supplied by file)
 */
class ST3DAnimationComponent : public STComponent {
public:
    explicit ST3DAnimationComponent();
    ST3DAnimationComponent(STMesh_Structure& meshStructure);
    ST3DAnimationComponent(const ST3DAnimationComponent& copy);
    ~ST3DAnimationComponent();
    void initScriptingFunctions(sol::state& script) override ;
    void init(std::shared_ptr<STEntity>& entity) override ;
    void update() override ;
    void setCurrentAnimation(const std::string& anim);
    void save(std::ofstream& out) override;
    void load(std::ifstream& in) override;
protected:
    bool                                m_isRoot;
    std::string                         m_currentAnimation;
    STRendererComponent*                m_gfxComponent;
    std::map<std::string, std::shared_ptr<STAnimation>> m_animationMap;
    STList<std::shared_ptr<STBoneData>> m_boneData;
    std::shared_ptr<STMeshNode>         m_nodeData;
    std::map<std::string, stUint>       m_boneMap;
    Matrix4f                            m_globalInverseMat;
private:
    STNodeAnim* FindAnimMode(STAnimation*, std::string nodeName);
    void MoveBones(stReal animationTime);
    void ReadNodeHeirarchy(float AnimationTime, STMeshNode* node, Matrix4f& parentTransform);

    stUint FindPosition(stReal animationTime, STNodeAnim* node);
    stUint FindRotation(stReal animationTime, STNodeAnim* node);
    stUint FindScale(stReal animation, STNodeAnim* node);

    void CalcInterpolatedPosition(Vector3D& out, float animTime, STNodeAnim* node);
    void CalcInterpolatedRotation(Quaternion& out, float animTime, STNodeAnim* node);
    void CalcInterpolatedScaling(Vector3D& out, float animTime, STNodeAnim* node);
};;


#endif //SWINGTECH1_ST3DANIMATIONCOMPONENT_H
