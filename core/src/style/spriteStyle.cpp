#include "spriteStyle.h"

#include "platform.h"
#include "tile/mapTile.h"
#include "util/builders.h"
#include "util/shaderProgram.h"
#include "util/texture.h"
#include "util/vertexLayout.h"
#include "view/view.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

MapTile* SpriteStyle::s_processedTile = nullptr;

SpriteStyle::SpriteStyle(std::string _name, GLenum _drawMode) : Style(_name, _drawMode) {
    
    m_labels = Labels::GetInstance();
}

SpriteStyle::~SpriteStyle() {}

void SpriteStyle::constructVertexLayout() {

    m_vertexLayout = std::shared_ptr<VertexLayout>(new VertexLayout({
        {"a_position", 2, GL_FLOAT, false, 0},
        {"a_screenPosition", 2, GL_FLOAT, false, 0},
        {"a_uv", 2, GL_FLOAT, false, 0},
        {"a_alpha", 1, GL_FLOAT, false, 0},
        {"a_rotation", 1, GL_FLOAT, false, 0},
    }));
}

void SpriteStyle::constructShaderProgram() {

    std::string fragShaderSrcStr = stringFromResource("sprite.fs");
    std::string vertShaderSrcStr = stringFromResource("sprite.vs");

    m_shaderProgram->setSourceStrings(fragShaderSrcStr, vertShaderSrcStr);

    m_spriteAtlas = std::unique_ptr<SpriteAtlas>(new SpriteAtlas("poi_icons_32.png"));
    
    m_spriteAtlas->addSpriteNode("plane", {0, 0}, {32, 32});
    m_spriteAtlas->addSpriteNode("tree", {0, 185}, {32, 32});
    m_spriteAtlas->addSpriteNode("sunburst", {0, 629}, {32, 32});
}

void* SpriteStyle::parseStyleParams(const std::string& _layerNameID, const StyleParamMap& _styleParamMap) {
    return nullptr;
}

void SpriteStyle::buildPoint(Point& _point, void* _styleParam, Properties& _props, VboMesh& _mesh) const {
    // TODO : make this configurable
    SpriteNode planeSprite = m_spriteAtlas->getSpriteNode("plane");
    std::vector<PosUVVertex> vertices;
    
    SpriteBuilder builder = {
        [&](const glm::vec2& coord, const glm::vec2 screenPos, const glm::vec2& uv) {
            vertices.push_back({ coord, screenPos, uv, 0.5f, M_PI_2 });
        }
    };
    
    for (auto prop : _props.stringProps) {
        if (prop.first == "name") {
            float scale = .5f;
            glm::vec2 offset = {0.f, -10.f};
            
            auto label = m_labels->addSpriteLabel(*SpriteStyle::s_processedTile, m_name, { glm::vec2(_point), glm::vec2(_point) }, planeSprite.size * scale, offset);
            
            if (label) {
                Builders::buildQuadAtPoint(label->getTransform().m_screenPosition + offset, planeSprite.size * scale, planeSprite.uvBL, planeSprite.uvTR, builder);
            }
        }
    }
    
    auto& mesh = static_cast<SpriteStyle::Mesh&>(_mesh);
    mesh.addVertices(std::move(vertices), std::move(builder.indices));
}

void SpriteStyle::buildLine(Line& _line, void* _styleParam, Properties& _props, VboMesh& _mesh) const {
    // NO-OP
}

void SpriteStyle::buildPolygon(Polygon& _polygon, void* _styleParam, Properties& _props, VboMesh& _mesh) const {
    // NO-OP
}

void SpriteStyle::onBeginBuildTile(MapTile& _tile) const {

    // FIXME: concurrency makes this processed tile unreliable
    SpriteStyle::s_processedTile = &_tile;
}

void SpriteStyle::onEndBuildTile(MapTile &_tile, std::shared_ptr<VboMesh> _mesh) const {

    SpriteStyle::s_processedTile = nullptr;
}

void SpriteStyle::onBeginDrawFrame(const std::shared_ptr<View>& _view, const std::shared_ptr<Scene>& _scene) {
    m_spriteAtlas->bind();
    
    m_shaderProgram->setUniformi("u_tex", 0);
    // top-left screen axis, y pointing down
    m_shaderProgram->setUniformMatrix4f("u_proj", glm::value_ptr(glm::ortho(0.f, _view->getWidth(), _view->getHeight(), 0.f, -1.f, 1.f)));
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SpriteStyle::onEndDrawFrame() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
