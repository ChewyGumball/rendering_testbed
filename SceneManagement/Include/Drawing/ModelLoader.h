#pragma once
#include <memory>
#include <string>

#include "Resources/Mesh.h"
#include <Renderer/IRenderer.h>

namespace ModelLoader
{
	std::unordered_map<Renderer::RenderResourceID, std::pair<std::string, std::shared_ptr<Renderer::Mesh>>> loadMultiPartOBJFile(std::string filename);
	std::shared_ptr<Renderer::Mesh> loadOBJFile(std::string filename, Renderer::VertexFormat loadedFormat = Renderer::VertexFormats::Unknown);
	std::shared_ptr<Renderer::Mesh> loadBinFile(std::string filename);

	//std::shared_ptr<Renderer::Mesh> loadFBXFile(std::string filename, Renderer::VertexFormat fileFormat = Renderer::VertexFormats::Unknown, Renderer::VertexFormat loadedFormat = Renderer::VertexFormats::Unknown);
}
