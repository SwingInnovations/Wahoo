#include "STOBJLoader.h"
#include "../../../Graphics/GL/GLShader.h"

STOBJLoader::STOBJLoader() {

}

/**
 * @author Loads the OBJ Mesh(s) from File.
 * @param fileName
 * @param errFlag
 * @param tags
 * @param dataMesh
 * @return True if more than one mesh was loaded, otherwise false.
 */
bool STOBJLoader::Validate(const std::string &fileName, bool *errFlag, std::vector<std::string> *tags,
                           std::vector<STMesh_Structure> *dataMesh) {
    stUint vertCount, texCount, normCount, objCount, lastVertCount, lastTexCount, lastNormCount;
    vertCount = texCount = normCount = objCount = lastVertCount = lastNormCount = lastTexCount = 0;
    std::string lastTag = "";
    std::vector<Vector3<stUint>> vertCounts;
    std::vector<Vector3<stReal>> _vertex;
    std::vector<Vector2<stReal>> _texCoord;
    std::vector<Vector3<stReal>> _normal;
    std::vector<int> _index;

    stInt counter = 0;

    auto extractVector3 = [](std::string str){
        stReal arr[3];
        stInt counter = 0;
        stReal temp;
        std::stringstream buff(str);
        while(buff >> temp){
            arr[counter] = temp;
            counter++;
        }
        return Vector3<stReal>(arr[0], arr[1], arr[2]);
    };

    auto extractVector2 = [](std::string str){
        stReal arr[2];
        stInt counter = 0;
        stReal temp;
        std::stringstream buff(str);
        while(buff >> temp){
            arr[counter] = temp;
            counter++;
        }

        return Vector2<stReal>(arr[0], arr[1]);
    };

    //Internal function to extract face and store in Vector3
    auto extractFace = [](std::string &str) {
        std::vector<stInt> ext_ind;
        stInt temp;
        for (stUint i = 0, L = (stUint) str.length(); i < L; i++) {
            if (str[i] == '/') str[i] = ' ';
        }
        std::stringstream buff(str);
        while (buff >> temp) ext_ind.push_back(temp);

        return Vector3<stInt>(ext_ind[0], ext_ind[1], ext_ind[2]);
    };

    auto splitFace = [](std::string str) {
        std::vector<std::string> ret;
        std::string temp;
        std::stringstream buff(str);
        while (buff >> temp) ret.push_back(temp);

        return ret;
    };

    std::ifstream in(fileName.c_str(), std::ios_base::in);
    if (!in) {
        std::cerr << "Invalid file! Could not load: " << fileName << std::endl;
        *errFlag = false;
        return false;
    }
    std::string line;
    if (in.good()) {
        while (std::getline(in, line)) {
            //Vertex Extraction
            if (line[0] == 'g') {
                (*tags).push_back(line.substr(2));
            }

            if (line[0] == 'v' && line[1] == ' ') {
                if (lastTag == "f") {
                    if(_vertex.empty()){
                        *errFlag = false;
                        return false;
                    }
                    if(_texCoord.empty()){
                        *errFlag = false;
                        return false;
                    }
                    if(_normal.empty()){
                        *errFlag = false;
                        return false;
                    }
                    if(_index.size() % 3 != 0){
                        *errFlag = false;
                        return false;
                    }
                    std::vector<int> adjustedIndicies;
                    adjustedIndicies.reserve(_index.size());
                    std::vector<Vertex> vertexList;
                    std::vector<int> indexList;
                    stUint ind = 0;

                    lastVertCount++;
                    lastTexCount++;
                    lastNormCount++;

                    if ((*dataMesh).size() > 0) {
                        for (stUint i = 0, S = (stUint) _index.size(); i < S; i += 3) {
                            adjustedIndicies.push_back(_index.at(i) - lastVertCount);
                            adjustedIndicies.push_back(_index.at(i + 1) - lastTexCount);
                            adjustedIndicies.push_back(_index.at(i + 2) - lastNormCount);
                        }
                    } else adjustedIndicies = _index;
                    for (stUint i = 0, S = (stUint) adjustedIndicies.size(); i < S; i += 3) {
                        Vertex vert(_vertex[adjustedIndicies.at(i)], _texCoord[adjustedIndicies.at(i + 1)],
                                    _normal[_index.at(i + 2)]);
                        vertexList.push_back(vert);
                        indexList.push_back(ind);
                        ind++;
                    }
                    STMesh_Structure mesh;
                    mesh.m_vertices = vertexList;
                    mesh.m_indices = indexList;
                    (*dataMesh).push_back(mesh);//Mesh has been added to the list.

                    lastVertCount = vertCount - 1;
                    lastTexCount = texCount - 1;
                    lastNormCount = normCount - 1;

                    _vertex.clear();
                    _texCoord.clear();
                    _normal.clear();
                    _index.clear();
                    adjustedIndicies.clear();
                }

                _vertex.push_back(extractVector3(line.substr(2)));
                vertCount++;
                lastTag = "v";
            }
            //Texcoord Extraction
            if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
                _texCoord.push_back(extractVector2(line.substr(3)));
                texCount++;
            }
            //Normal Extraction
            if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
                _normal.push_back(extractVector3(line.substr(3)));
                normCount++;
            }

            //Index Extraction
            if (line[0] == 'f' && line[1] == ' ') {
                auto faceStrings = splitFace(line.substr(2));
                Vector3<stInt> values;
                Vector3<stInt> cached[3];
                stUint cachedCounter = 0;
                for(stUint i = 0; i < 3; i++){
                    values = extractFace(faceStrings.at(i));
                    if(i == 1 || i == 2){
                        cached[cachedCounter++] = values;
                    }
                    _index.push_back(values.getX() - 1);
                    _index.push_back(values.getY() - 1);
                    _index.push_back(values.getZ() - 1);
                }
                if(faceStrings.size() > 3){
                    cached[cachedCounter] = extractFace(faceStrings.at(3));
                    for(stUint i = 0; i < 3; i++){
                        _index.push_back(cached[i].getX() - 1);
                        _index.push_back(cached[i].getY() - 1);
                        _index.push_back(cached[i].getZ() - 1);
                    }
                }
                lastTag = "f";
            }
            counter++;
        }
        in.close();

        if (lastTag == "f") {
            std::vector<int> adjustedIndicies;
            auto check = adjustedIndicies.empty();
            std::vector<Vertex> vertexList;
            std::vector<int> indexList;
            stUint ind = 0;
            lastVertCount++;
            lastTexCount++;
            lastNormCount++;
            if ((*dataMesh).size() > 0) {
                for (stUint i = 0, S = (stUint) _index.size(); i < S; i += 3) {
                    adjustedIndicies.push_back(_index.at(i) - lastVertCount);
                    adjustedIndicies.push_back(_index.at(i + 1) - lastTexCount);
                    adjustedIndicies.push_back(_index.at(i + 2) - lastNormCount);
                }
            } else {
                adjustedIndicies = _index;
            }
            auto adSize = adjustedIndicies.size();
            for (stUint i = 0, S = (stUint) adjustedIndicies.size(); i < S; i += 3) {
                Vertex vert(_vertex[adjustedIndicies.at(i)], _texCoord[adjustedIndicies.at(i + 1)],
                            _normal[_index.at(i + 2)]);
                vertexList.push_back(vert);
                indexList.push_back(ind);
                ind++;
            }
            STMesh_Structure mesh;
            mesh.m_vertices = vertexList;
            mesh.m_indices = indexList;
            (*dataMesh).push_back(mesh);//Mesh has been added to the list.

            _vertex.clear();
            _texCoord.clear();
            _normal.clear();
            _index.clear();
        }
    }
    return (*dataMesh).size() > 1;
}

/**
 * Loads a singular OBJ File
 * @param fileName
 * @return STMeshStructure.
 */
STMesh_Structure STOBJLoader::Load(const std::string fileName) {
    STMesh_Structure ret;

    std::vector<Vector3<stReal>> _vertex;
    std::vector<Vector2<stReal>> _texCoords;
    std::vector<Vector3<stReal>> _normal;
    std::vector<stInt> _index;
    std::vector<Vertex> vertList;
    std::vector<int> indList;
    stInt counter = 0;

    //Loading the file
    std::ifstream in(fileName.c_str(), std::ios_base::in);
    if(!in){
        std::cerr << "Invalid File! Could not load: " << fileName << std::endl;
        STMesh_Structure ret2;
        return ret2;
    }

    std::string line;
    std::string lastTag;
    if(in.good()){
        while(std::getline(in, line)){
            counter++;
            if(line[0] == 'v' && line[1] == ' '){
                _vertex.push_back(ExtractVector3(line.substr(2)));
            }
            if(line[0] == 'v' && line[1] == 't' && line[2] == ' '){
                _texCoords.push_back(ExtractVector2(line.substr(3)));
            }
            if(line[0] == 'f' && line[1] == 'n' && line[2] == ' '){
                _normal.push_back(ExtractVector3(line.substr(3)));
            }
            if(line[0] == 'f' && line[1] == ' '){
                auto faceStrings = SplitFace(line.substr(2));
                Vector3<stInt> values;
                Vector3<stInt> cached[3];
                stUint cachedCounter = 0;
                for(stUint i = 0; i < 3; i++){
                    values = ExtractFace(faceStrings.at(i));
                    if(i == 1 || i == 2){
                        cached[cachedCounter++] = values;
                    }
                    _index.push_back(values.getX() - 1);
                    _index.push_back(values.getY() - 1);
                    _index.push_back(values.getZ() - 1);
                }
                if(faceStrings.size() > 3){
                    cached[cachedCounter] = ExtractFace(faceStrings.at(3));
                    for(stUint i = 0; i < 3; i++){
                        _index.push_back(cached[i].getX() - 1);
                        _index.push_back(cached[i].getY() - 1);
                        _index.push_back(cached[i].getZ() - 1);
                    }
                }
            }
        }
    }
    in.close();
    stInt ind = 0;
    for(stUint i = 0, S = (stUint)_index.size(); i < S; i+= 3){
        Vertex vert(_vertex[_index.at(i)], _texCoords[_index.at(i+1)], _normal[_index.at(i+2)]);
        vertList.push_back(vert);
        indList.push_back(ind);
        ind++;
    }
    ret.m_vertices = vertList;
    ret.m_indices = indList;

    return ret;
}

Vector3<stReal> STOBJLoader::ExtractVector3(std::string str) {
    stReal arr[3];
    stInt c = 0;
    stReal tmp;
    std::stringstream buff(str);
    while(buff >> tmp){
        arr[c++] = tmp;
    }
    return Vector3<stReal>(arr[0], arr[1], arr[2]);
}

Vector2<stReal> STOBJLoader::ExtractVector2(std::string str) {
    stReal arr[2];
    stInt c = 0;
    stReal tmp;
    std::stringstream buff(str);
    while(buff >> tmp){
        arr[c++] = tmp;
    }
    return Vector2<stReal>(arr[0], arr[1]);
}

Vector3<stInt> STOBJLoader::ExtractFace(std::string str) {
    std::vector<stInt> ext_ind;
    stInt tmp;
    for(stUint i = 0, L = str.length(); i < L; i++){
        if(str[i] == '/') str[i] = ' ';
    }
    std::stringstream buff(str);
    while(buff >> tmp) ext_ind.push_back(tmp);
    return Vector3<stInt>(ext_ind[0], ext_ind[1], ext_ind[2]);
}

std::vector<std::string> STOBJLoader::SplitFace(std::string str) {
    std::vector<std::string> ret;
    std::string tmp;
    std::stringstream buff(str);
    while(buff >> tmp) ret.push_back(tmp);
    return ret;
}

bool STOBJLoader::Validate(const std::string &fileName, bool *errFlag, std::vector<STMesh_Structure> *meshes,
                           std::map<std::string, STMaterial *> *materials) {
    stUint vertCount, texCount, normCount, objCount, lastVertCount, lastTexCount, lastNormCount;
    vertCount = texCount = normCount = objCount = lastVertCount = lastNormCount = lastTexCount = 0;
    std::string lastTag;    //Keeps a cache of the last code.
    std::vector<Vector3<stUint>> vertCounts;
    std::vector<Vector3<stReal>> _vertex;
    std::vector<Vector2<stReal>> _texCoord;
    std::vector<Vector3<stReal>> _normal;
    std::string name;           //save the name of the current mesh
    std::string materialName;
    std::vector<int> _index;
    stUint counter = 0;
    std::string filePath = "";
    if(fileName.find("/")!=0){
        filePath = fileName.substr(0, fileName.find_last_of("/"));
    }else if(fileName.find("\\") != 0){
        filePath = fileName.substr(0, fileName.find_last_of('\\'));
    }

    std::ifstream in(fileName.c_str(), std::ios_base::in);
    if(!in){
        std::cerr << "Invalid File! Could not load: " << fileName << std::endl;
        *errFlag = false;
        return false;
    }
    std::string line;
    if(in.good()){
        while(std::getline(in, line)){
            if(line[0] == 'm'){
                //Do some material processing here.
                std::string matName = line.substr(7);
                STOBJLoader::PopulateMaterial(matName, materials);
                //if(materials->count(matName) == 0) materials->insert(std::pair<std::string, STMaterial*>(matName, new STMaterial));
            }
            if(line[0] == 'o'){
                name = line.substr(line.find(' '), line.length()-1);
            }

            if(line[0] == 'u'){
                //Assume this means "Use Material"
                materialName = line.substr(line.find(' ')+1);
            }

            if(line[0] == 'v'){
                if(lastTag == "f"){
                    if(_vertex.empty()){
                        std::cerr << "Error! Mesh must have Verticies!" << std::endl;
                        *errFlag = false;
                        return false;
                    }
                    if(_texCoord.empty()){
                        std::cerr << "Error! Mesh must have TexCoords!" << std::endl;
                        *errFlag = false;
                        return false;
                    }
                    if(_normal.empty()){
                        std::cerr << "Error! Mesh must have Normals!" << std::endl;
                        *errFlag = false;
                        return false;
                    }
                    if(_index.empty()){
                        std::cerr << "Error! Mesh must have Indicies!" << std::endl;
                        *errFlag = false;
                        return false;
                    }
                    //Assuming everything goes well.
                    std::vector<int> adjustedIndicies;
                    adjustedIndicies.reserve(_index.size());
                    stInt ind = 0;
                    lastVertCount++;
                    lastTexCount++;
                    lastNormCount++;
                    if((*meshes).size() > 0){
                        for(stUint i = 0, S = (stUint)_index.size(); i < S; i+=3){
                            adjustedIndicies.push_back(_index.at(i) - lastVertCount);
                            adjustedIndicies.push_back(_index.at(i+1) - lastTexCount);
                            adjustedIndicies.push_back(_index.at(i+2) - lastNormCount);
                        }
                    }else adjustedIndicies = _index;
                    STMesh_Structure mesh;
                    mesh.name = name;
                    mesh.materialKey = materialName;
                    //Now we are ready to begin processing;
                    for(stUint i = 0, S = (stUint)adjustedIndicies.size(); i < S; i+=3){
                        mesh.m_vertices.push_back(Vertex(_vertex[adjustedIndicies[i]],
                                                         _texCoord[adjustedIndicies[i+1]],
                                                         _normal[adjustedIndicies[i+2]]));
                        mesh.m_indices.push_back(ind);
                        ind++;
                    }
                    (*meshes).push_back(mesh);
                    lastVertCount = vertCount - 1;
                    lastTexCount = texCount - 1;
                    lastNormCount = normCount - 1;
                    _vertex.clear();
                    _texCoord.clear();
                    _normal.clear();
                    _index.clear();
                    adjustedIndicies.clear();
                }
                if(line[1] == ' '){
                    _vertex.push_back(STOBJLoader::ExtractVector3(line.substr(2)));
                    vertCount++;
                    lastTag = "v";
                }

                if(line[1] == 't' && line[2] == ' '){
                    _texCoord.push_back(STOBJLoader::ExtractVector2(line.substr(3)));
                    texCount++;
                    lastTag = "vt";
                }

                if(line[1] == 'n' && line[2] == ' '){
                    _normal.push_back(STOBJLoader::ExtractVector3(line.substr(3)));
                    normCount++;
                    lastTag = "vt";
                }

            }

            if(line[0] == 'f' && line[1] == ' '){
                auto faceStrings = STOBJLoader::SplitFace(line.substr(2));
                Vector3<stInt> values;
                Vector3<stInt> cached[3];
                stUint cachedCounter = 0;
                for(stUint i = 0; i < 3; i++){
                    values = STOBJLoader::ExtractFace(faceStrings.at(i));
                    if(i == 1 || i == 2){
                        cached[cachedCounter++] = values;
                    }
                    _index.push_back(values.getX() - 1);
                    _index.push_back(values.getY() - 1);
                    _index.push_back(values.getZ() - 1);
                }
                if(faceStrings.size() > 3){
                    cached[cachedCounter] = STOBJLoader::ExtractFace(faceStrings.at(3));
                    for(stUint j = 0; j < 3; j++){
                        _index.push_back(cached[j].getX() - 1);
                        _index.push_back(cached[j].getY() - 1);
                        _index.push_back(cached[j].getZ() - 1);
                    }
                }
                lastTag = "f";
            }
            counter++;
        }
        in.close();
        if(lastTag == "f"){
            std::vector<int> adjustedIndices;
            stInt ind = 0;
            lastVertCount++;
            lastTexCount++;
            lastNormCount++;
            STMesh_Structure mesh;
            mesh.name = name;
            mesh.materialKey = materialName;
            if((*meshes).size() > 0){
                for(stUint i = 0, S = (stUint)_index.size(); i < S; i+=3){
                    adjustedIndices.push_back(_index.at(i) - lastVertCount);
                    adjustedIndices.push_back(_index.at(i+1) - lastTexCount);
                    adjustedIndices.push_back(_index.at(i+2) - lastNormCount);
                }
            }else adjustedIndices = _index;
            for(stUint i = 0, S = (stUint)adjustedIndices.size(); i < S; i+=3){
                mesh.m_vertices.push_back(Vertex(_vertex[adjustedIndices[i]],
                                                _texCoord[adjustedIndices[i+1]],
                                                _normal[adjustedIndices[i+2]]));
                mesh.m_indices.push_back(ind);
                ind++;
            }
            (*meshes).push_back(mesh);
            _vertex.clear();
            _texCoord.clear();
            _normal.clear();
            _index.clear();
        }
    }

    return (*meshes).size() > 1;
}

void STOBJLoader::PopulateMaterial(const std::string &fileName, std::map<std::string, STMaterial *> *materials) {
    //Open The file now.
    std::ifstream in(fileName.c_str());
    if(!in){
        std::cerr << "Error! Unable to load material file!" << std::endl;
        return;
    }
    std::string matName;
    std::string line;
    STMaterial* mat;
    bool beganMat = false;
    if(in.good()){
        while(getline(in, line)){
            //Do Populating of material information here.
            if(line[0] == 'n' && line[1] == 'e'){
                mat = new STMaterial(new GLShader("standard"));
                beganMat = true;
                matName = line.substr(line.find(' ')+1);
            }
            if(line[0] == 'K'){
                if(line[1] == 'd'){
                    auto diffVec = STOBJLoader::ExtractVector3(line.substr(line.find_first_of(' ')));
                    if(mat != nullptr) mat->setDiffuseColor(STColor(diffVec.getX(), diffVec.getY(), diffVec.getZ(), 1.f));
                }
                //TODO Implement Ambient
                //TODO Implement Specular
                //TODO Implement
            }
            if(line == "" && beganMat){
                beganMat = false;
                (*materials).insert(std::pair<std::string, STMaterial*>(matName, mat));
            }
        }
    }
    if(beganMat)(*materials).insert(std::pair<std::string, STMaterial*>(matName, mat));
    in.close();
}

