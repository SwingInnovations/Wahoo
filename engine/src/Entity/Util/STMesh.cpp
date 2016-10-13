#include <c++/fstream>
#include "STMesh.h"
#include "../../Graphics/STGraphics.h"

/*-OBJ Mesh-*/
OBJMesh::OBJMesh() {

}

OBJMesh::OBJMesh(const std::string &filename) {
    std::ifstream in(filename.c_str(), std::ios_base::in);

    if(!in){
        std::cout << "Invalid File! Could not load: " << filename << std::endl;
        return;
    }

    unsigned int counter = 0;

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

    auto extractFace = [](std::string str){
        stInt arr[3];
        stInt counter = 0;
        stInt temp;
        for(stUint i = 0, L = (stUint)str.length(); i < L; i++){
            if(str[i] == '/') str[i] = ' ';
        }
        std::stringstream buff(str);
        while(buff >> temp){
            arr[counter] = temp;
            counter++;
        }

        return Vector3<stInt>(arr[0], arr[1], arr[2]);
    };

    auto splitFace = [](std::string str){
        std::vector<std::string> ret;
        std::string temp;
        std::stringstream buff(str);
        while(buff >> temp) ret.push_back(temp);
        return ret;
    };

    std::string line; //Store line from text file here.
    if(in.good()){
        while(std::getline(in, line)){
            counter++;
            //Vertex Extraction
            if(line[0] == 'v' && line[1] == ' '){

                _vertex.push_back(extractVector3(line.substr(2)));
            }
            //Texcoord Extraction
            if(line[0] == 'v' && line[1] == 't' && line[2] == ' '){
                _texCoord.push_back(extractVector2(line.substr(3)));
            }
            //Normal Extraction
            if(line[0] == 'v' && line[1] == 'n'&& line[2] == ' '){
                _normal.push_back(extractVector3(line.substr(3)));
            }

            //Index Extraction
            if(line[0] == 'f' && line[1] == ' '){
                auto faceStrings = splitFace(line.substr(2));
                Vector3<stInt> values;
                for(stUint i = 0; i < 3; i++){
                    values = extractFace(faceStrings.at(i));
                    _index.push_back(values.getX() - 1);
                    _index.push_back(values.getY() - 1);
                    _index.push_back(values.getZ() - 1);
                }
            }
        }
        in.close();
    }

    int inCount = 0;

    for(unsigned int i = 0, S = (int)_index.size(); i < S; i+=3){
        Vertex vert(_vertex.at((int)_index.at(i)), _texCoord.at((int)_index.at(i+1)), _normal.at((int)_index.at(i+2)));
        verticies.push_back(vert);
        indicies.push_back(inCount);
        inCount++;
    }
    std::cout << "Finished loading mesh. " << std::endl;
}

OBJMesh::OBJMesh(const std::string &fileName, Vector2<stInt> lineBounds) {
    std::ifstream in(fileName.c_str(), std::ios_base::in);

    if(!in){
        std::cout << "Invalid File! Could not load: " << fileName << std::endl;
        return;
    }

    unsigned int counter = 0;

    std::string line; //Store line from text file here.
    if(in.good()){
        while(std::getline(in, line) && counter <= lineBounds.getY()+1){
            counter++;
            //Vertex Extraction
            if(counter > lineBounds.getX() - 1){
                if(line[0] == 'v' && line[1] == ' '){
                    std::string vals = line.substr(2);
                    stReal _x = 0.0f, _y = 0.0f, _z = 0.0f;

                    std::string vX = vals.substr(0, vals.find(' '));
                    _x = (stReal)atof(vX.c_str());

                    std::string vY = vals.substr(vX.length()+1, vals.find(' '));
                    _y = (stReal)atof(vY.c_str());

                    std::string vZ = vals.substr(vals.find_last_of(' ')+1);
                    _z = (stReal)atof(vZ.c_str());

                    _vertex.push_back(Vector3<stReal>(_x, _y, _z));
                }
                //Texcoord Extraction
                if(line[0] == 'v' && line[1] == 't' && line[2] == ' '){
                    std::string vals = line.substr(3);
                    stReal _u = 0.0f, _v = 0.0f;
                    std::string tU = vals.substr(0, vals.find(' '));
                    _u = (stReal)atof(tU.c_str());
                    std::string tV = vals.substr(tU.length()+1, vals.find(' '));
                    _v = (stReal)atof(tV.c_str());

                    _texCoord.push_back(Vector2<stReal>(_u, _v));
                }
                //Normal Extraction
                if(line[0] == 'v' && line[1] == 'n'&& line[2] == ' '){
                    std::string vals = line.substr(3);
                    stReal _x = 0.0f, _y = 0.0f, _z = 0.0f;
                    std::string nX = vals.substr(0, vals.find(' '));

                    _x = (stReal)atof(nX.c_str());
                    std::string nY = vals.substr(nX.length()+1, vals.find(' '));

                    _y = (stReal)atof(nY.c_str());
                    std::string nZ = vals.substr(vals.find_last_of(' ')+1);

                    _z = (stReal)atof(nZ.c_str());

                    _normal.push_back(Vector3<stReal>(_x, _y, _z));
                }

                //Index Extraction
                if(line[0] == 'f' && line[1] == ' '){
                    std::string hLine = line.substr(2);
                    int i = 0, j = 0, k = 0;
                    //first element
                    std::string face1 = hLine.substr(0, hLine.find(' '));
                    std::string v1 = face1.substr(0, face1.find('/'));
                    i = atoi(v1.c_str());
                    int midLen1 = face1.find_last_of('/') - face1.find('/')+1;
                    std::string t1 = face1.substr(face1.find('/')+1, midLen1);
                    j = atoi(t1.c_str());
                    std::string n1 = face1.substr(face1.find_last_of('/') + 1);
                    k = atoi(n1.c_str());
                    i--; j--; k--;
                    _index.push_back(i); _index.push_back(j); _index.push_back(k);
                    int midLen = hLine.find_last_of(' ') - hLine.find(' ') + 1;
                    std::string face2 = hLine.substr(hLine.find(' ') + 1, midLen);
                    std::string v2 = face2.substr(0, face2.find('/'));
                    i = atoi(v2.c_str());
                    int midLen2 = face2.find_last_of('/') - face2.find('/') + 1;
                    std::string t2 = face2.substr(v2.length()+1, midLen2);
                    j = atoi(t2.c_str());
                    std::string n2 = face2.substr(face2.find_last_of('/') + 1);
                    k = atoi(n2.c_str());
                    i--; j--; k--;
                    _index.push_back(i); _index.push_back(j); _index.push_back(k);
                    std::string face3 = hLine.substr(hLine.find_last_of(' ') + 1);
                    std::string v3 = face3.substr(0, face3.find('/'));
                    i = atoi(v3.c_str());
                    int midLen3 = face3.find_last_of('/')-face3.find('/') + 1;
                    std::string t3 = face3.substr(face3.find('/')+1, midLen3);
                    j = atoi(t3.c_str());
                    std::string n3 = face3.substr(face3.find_last_of('/')+1);
                    k = atoi(n3.c_str());
                    i--; j--; k--;
                    _index.push_back(i); _index.push_back(j); _index.push_back(k);
                }
            }
        }
        in.close();
    }

    int inCount = 0;
    auto vDisp = _index.at(0);
    auto tDisp = _index.at(1);
    auto nDisp = _index.at(2);

    for(unsigned int i = 0, S = (int)_index.size(); i < S; i+=3){
        auto vInd = _index[i] - vDisp;
        auto tInd = _index[i+1] - tDisp;
        auto nInd = _index[i+2] - nDisp;
        Vertex vert(_vertex.at(vInd), _texCoord.at(tInd), _normal.at(nInd));

        verticies.push_back(vert);
        indicies.push_back(inCount);
        inCount++;
    }
    std::cout << "Finished loading mesh. " << std::endl;
}

OBJMesh::OBJMesh(const std::string &fileName, Vector2<stInt> lineBounds, Vector3<stInt> maxSize) {
    std::ifstream in(fileName.c_str(), std::ios_base::in);

    if(!in){
        std::cout << "Invalid File! Could not load: " << fileName << std::endl;
        return;
    }

    unsigned int counter = 0;

    std::string line; //Store line from text file here.
    if(in.good()){
        while(std::getline(in, line) && counter <= lineBounds.getY()+1){
            counter++;
            //Vertex Extraction
            if(counter > lineBounds.getX() - 1){
                if(line[0] == 'v' && line[1] == ' '){
                    std::string vals = line.substr(2);
                    stReal _x = 0.0f, _y = 0.0f, _z = 0.0f;

                    std::string vX = vals.substr(0, vals.find(' '));
                    _x = (stReal)atof(vX.c_str());

                    std::string vY = vals.substr(vX.length()+1, vals.find(' '));
                    _y = (stReal)atof(vY.c_str());

                    std::string vZ = vals.substr(vals.find_last_of(' ')+1);
                    _z = (stReal)atof(vZ.c_str());

                    _vertex.push_back(Vector3<stReal>(_x, _y, _z));
                }
                //Texcoord Extraction
                if(line[0] == 'v' && line[1] == 't' && line[2] == ' '){
                    std::string vals = line.substr(3);
                    stReal _u = 0.0f, _v = 0.0f;
                    std::string tU = vals.substr(0, vals.find(' '));
                    _u = (stReal)atof(tU.c_str());
                    std::string tV = vals.substr(tU.length()+1, vals.find(' '));
                    _v = (stReal)atof(tV.c_str());

                    _texCoord.push_back(Vector2<stReal>(_u, _v));
                }
                //Normal Extraction
                if(line[0] == 'v' && line[1] == 'n'&& line[2] == ' '){
                    std::string vals = line.substr(3);
                    stReal _x = 0.0f, _y = 0.0f, _z = 0.0f;
                    std::string nX = vals.substr(0, vals.find(' '));

                    _x = (stReal)atof(nX.c_str());
                    std::string nY = vals.substr(nX.length()+1, vals.find(' '));

                    _y = (stReal)atof(nY.c_str());
                    std::string nZ = vals.substr(vals.find_last_of(' ')+1);

                    _z = (stReal)atof(nZ.c_str());

                    _normal.push_back(Vector3<stReal>(_x, _y, _z));
                }

                //Index Extraction
                if(line[0] == 'f' && line[1] == ' '){
                    std::string hLine = line.substr(2);
                    int i = 0, j = 0, k = 0;
                    //first element
                    std::string face1 = hLine.substr(0, hLine.find(' '));
                    std::string v1 = face1.substr(0, face1.find('/'));
                    i = atoi(v1.c_str());
                    int midLen1 = face1.find_last_of('/') - face1.find('/')+1;
                    std::string t1 = face1.substr(face1.find('/')+1, midLen1);
                    j = atoi(t1.c_str());
                    std::string n1 = face1.substr(face1.find_last_of('/') + 1);
                    k = atoi(n1.c_str());
                    i--; j--; k--;
                    _index.push_back(i); _index.push_back(j); _index.push_back(k);
                    int midLen = hLine.find_last_of(' ') - hLine.find(' ') + 1;
                    std::string face2 = hLine.substr(hLine.find(' ') + 1, midLen);
                    std::string v2 = face2.substr(0, face2.find('/'));
                    i = atoi(v2.c_str());
                    int midLen2 = face2.find_last_of('/') - face2.find('/') + 1;
                    std::string t2 = face2.substr(v2.length()+1, midLen2);
                    j = atoi(t2.c_str());
                    std::string n2 = face2.substr(face2.find_last_of('/') + 1);
                    k = atoi(n2.c_str());
                    i--; j--; k--;
                    _index.push_back(i); _index.push_back(j); _index.push_back(k);
                    std::string face3 = hLine.substr(hLine.find_last_of(' ') + 1);
                    std::string v3 = face3.substr(0, face3.find('/'));
                    i = atoi(v3.c_str());
                    int midLen3 = face3.find_last_of('/')-face3.find('/') + 1;
                    std::string t3 = face3.substr(face3.find('/')+1, midLen3);
                    j = atoi(t3.c_str());
                    std::string n3 = face3.substr(face3.find_last_of('/')+1);
                    k = atoi(n3.c_str());
                    i--; j--; k--;
                    _index.push_back(i); _index.push_back(j); _index.push_back(k);
                }
            }
        }
        in.close();
    }

    int inCount = 0;
    auto vDisp = maxSize.getX();
    auto tDisp = maxSize.getY();
    auto nDisp = maxSize.getZ();

    //TODO - Fix out of Range Index.

    for(unsigned int i = 0, S = (int)_index.size(); i < S; i+=3){
        auto vInd = _index[i] - vDisp;
        auto tInd = _index[i+1] - tDisp;
        auto nInd = _index[i+2] - nDisp;
        Vertex vert(_vertex.at(vInd), _texCoord.at(tInd), _normal.at(nInd));

        verticies.push_back(vert);
        indicies.push_back(inCount);
        inCount++;
    }
    std::cout << "Finished loading mesh. " << std::endl;
}

OBJMesh::~OBJMesh() {

}

bool OBJMesh::Validate(const std::string &fileName, std::vector<std::string> *tags,
                       std::vector<STMesh_Structure> *dataMesh) {
    //TODO - implement this.
    stUint vertCount, texCount, normCount, objCount, lastVertCount, lastTexCount, lastNormCount;
    vertCount = texCount = normCount = objCount = lastVertCount = lastNormCount = lastTexCount = 0;
    std::string lastTag = "";
    std::vector<Vector3<stUint>> vertCounts;
    std::vector<Vector3<stReal>> _vertex;
    std::vector<Vector2<stReal>> _texCoord;
    std::vector<Vector3<stReal>> _normal;
    std::vector<int> _index;

    stInt counter = 0;

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
        std::cout << "Invalid file! Could not load: " << fileName << std::endl;
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
                    std::vector<int> adjustedIndicies;
                    adjustedIndicies.reserve(_index.size());
                    std::vector<Vertex> vertexList;
                    std::vector<int> indexList;
                    stUint ind = 0;

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

                std::string vals = line.substr(2);
                stReal _x = 0.0f, _y = 0.0f, _z = 0.0f;

                std::string vX = vals.substr(0, vals.find(' '));
                _x = (stReal) atof(vX.c_str());

                std::string vY = vals.substr(vX.length() + 1, vals.find(' '));
                _y = (stReal) atof(vY.c_str());

                std::string vZ = vals.substr(vals.find_last_of(' ') + 1);
                _z = (stReal) atof(vZ.c_str());

                _vertex.push_back(Vector3<stReal>(_x, _y, _z));
                vertCount++;
                lastTag = "v";
            }
            //Texcoord Extraction
            if (line[0] == 'v' && line[1] == 't' && line[2] == ' ') {
                std::string vals = line.substr(3);
                stReal _u = 0.0f, _v = 0.0f;
                std::string tU = vals.substr(0, vals.find(' '));
                _u = (stReal) atof(tU.c_str());
                std::string tV = vals.substr(tU.length() + 1, vals.find(' '));
                _v = (stReal) atof(tV.c_str());

                _texCoord.push_back(Vector2<stReal>(_u, _v));
                texCount++;
            }
            //Normal Extraction
            if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ') {
                std::string vals = line.substr(3);
                stReal _x = 0.0f, _y = 0.0f, _z = 0.0f;
                std::string nX = vals.substr(0, vals.find(' '));

                _x = (stReal) atof(nX.c_str());
                std::string nY = vals.substr(nX.length() + 1, vals.find(' '));

                _y = (stReal) atof(nY.c_str());
                std::string nZ = vals.substr(vals.find_last_of(' ') + 1);

                _z = (stReal) atof(nZ.c_str());

                _normal.push_back(Vector3<stReal>(_x, _y, _z));
                normCount++;
            }

            //Index Extraction
            if (line[0] == 'f' && line[1] == ' ') {
                auto faceString = splitFace(line.substr(2));
                //first element
                auto vals = extractFace(faceString.at(0));
                _index.push_back(vals.getX() - 1);
                _index.push_back(vals.getY() - 1);
                _index.push_back(vals.getZ() - 1);
                vals = extractFace(faceString.at(1));
                _index.push_back(vals.getX() - 1);
                _index.push_back(vals.getY() - 1);
                _index.push_back(vals.getZ() - 1);
                vals = extractFace(faceString.at(2));
                _index.push_back(vals.getX() - 1);
                _index.push_back(vals.getY() - 1);
                _index.push_back(vals.getZ() - 1);
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

bool STMesh::Validate(const std::string &fileName, int *typeFlag, std::vector<std::string> *tags,
                      std::vector<STMesh_Structure> *meshes) {
    stUint extenPoint = (stUint)fileName.size() - 4;
    std::string fileExtension = fileName.substr(extenPoint);
    if(fileExtension == ".obj"){
        *typeFlag = STMesh::OBJ;
        return OBJMesh::Validate(fileName, tags, meshes);
    }
    else if(fileExtension == ".fbx"){
        *typeFlag = STMesh::FBX;
        //TODO FBX File Validation and Splitup.
        return true;
    }else if(fileExtension == ".gex"){
        *typeFlag = STMesh::GEX;
        //TODO GEX File Validation and Splitup.
        return true;
    }
    return false;
}
