//
// Created by NDanq on 11/2/2015.
//

#include "Shader.h"

void Shader::updateUniforms(std::vector<STShader::ShaderAttrib> _uniforms) {
    for(uint32_t i = 0, S = (uint32_t)_uniforms.size(); i < S; i++){
        if(_uniforms[i].type == STShader::INT){
            update(_uniforms[i].name, STShader::toInt(_uniforms[i].value));
        }else if(_uniforms[i].type == STShader::FLOAT){
            update(_uniforms[i].name, STShader::toFloat(_uniforms[i].value));
        } else if(_uniforms[i].type == STShader::VEC3){
            update(_uniforms[i].name, STShader::toVector3(_uniforms[i].value));
        }else{
            update(_uniforms[i].name, STShader::toVector4(_uniforms[i].value));
        }
    }
}