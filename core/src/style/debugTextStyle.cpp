#include "debugTextStyle.h"

#include "text/fontContext.h"
#include "tile/mapTile.h"
#include "util/vboMesh.h"

DebugTextStyle::DebugTextStyle(const std::string& _fontName, std::string _name, float _fontSize, unsigned int _color, bool _sdf, GLenum _drawMode)
: TextStyle(_fontName, _name, _fontSize, _color, _sdf, false, _drawMode) {
}

void DebugTextStyle::addData(TileData& _data, MapTile& _tile) {

    if (Tangram::getDebugFlag(Tangram::DebugFlags::tile_infos)) {
        std::shared_ptr<VboMesh> mesh(newMesh());
        auto& buffer = static_cast<TextBuffer&>(*mesh);
        
        onBeginBuildTile(*mesh);
        
        auto ftContext = m_labels->getFontContext();
        
        ftContext->setFont(m_fontName, m_fontSize * m_pixelScale);

        if (m_sdf) {
            float blurSpread = 2.5;
            ftContext->setSignedDistanceField(blurSpread);
        }

        std::string tileID = std::to_string(_tile.getID().x) + "/" + std::to_string(_tile.getID().y) + "/" + std::to_string(_tile.getID().z);
        m_labels->addTextLabel(_tile, buffer, m_name, { glm::vec2(0), glm::vec2(0) }, tileID, Label::Type::debug);

        onEndBuildTile(*mesh);

        mesh->compileVertexBuffer();
        _tile.addGeometry(*this, mesh);
    }

}
