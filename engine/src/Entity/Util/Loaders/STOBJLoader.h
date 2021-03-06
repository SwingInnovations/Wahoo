#ifndef SWINGTECH1_STOBJLOADER_H
#define SWINGTECH1_STOBJLOADER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "STMeshCommon.h"
#include "../../../Graphics/STMaterial.h"

/**
 * @author Nathan Danque
 * @brief Serves as the OBJ Loader for SwingTech1
 */


class STOBJLoader{
public:
    STOBJLoader();
    static STMesh_Structure Load(const std::string fileName);
    static bool Validate(const std::string& fileName, bool* errFlag, std::vector<std::string> *tags, std::vector<STMesh_Structure>* dataMesh);
    static bool Validate(const std::string& fileName, bool* errFlag, std::vector<STMesh_Structure>* meshes, std::map<std::string, STMaterial*> *materials);
private:
    static Vector3<stReal> ExtractVector3(std::string str);
    static Vector2<stReal> ExtractVector2(std::string str);
    static Vector3<stInt> ExtractFace(std::string str);
    static std::vector<std::string> SplitFace(std::string str);
    static void PopulateMaterial(const std::string& fileName, std::map<std::string, STMaterial*>* materials);
};


#endif //SWINGTECH1_STOBJLOADER_H
