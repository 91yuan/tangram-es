#include "ambientLight.h"

#include "glm/gtx/string_cast.hpp"
#include "platform.h"

namespace Tangram {

std::string AmbientLight::s_classBlock;
std::string AmbientLight::s_typeName = "AmbientLight";

AmbientLight::AmbientLight(const std::string& name, bool dynamic) :
    Light(name, dynamic) {

    m_type = LightType::ambient;

}

AmbientLight::~AmbientLight() {}

std::unique_ptr<LightUniforms> AmbientLight::injectOnProgram(ShaderProgram& shader) {
    injectSourceBlocks(shader);

    if (!m_dynamic) { return nullptr; }

    return std::make_unique<LightUniforms>(shader, getUniformName());
}

void AmbientLight::setupProgram(const View& view, LightUniforms& uniforms) {
    Light::setupProgram(view, uniforms);
}

std::string AmbientLight::getClassBlock() {
    if (s_classBlock.empty()) {
        s_classBlock = stringFromFile("shaders/ambientLight.glsl", PathType::internal)+"\n";
    }
    return s_classBlock;
}

std::string AmbientLight::getInstanceDefinesBlock() {
    //  Ambient lights don't have defines.... yet.
    return "\n";
}

std::string AmbientLight::getInstanceAssignBlock() {
    std::string block = Light::getInstanceAssignBlock();
    if (!m_dynamic) {
        block += ")";
    }
    return block;
}

const std::string& AmbientLight::getTypeName() {

    return s_typeName;

}

}
