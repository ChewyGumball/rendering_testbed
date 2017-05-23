#pragma once
#include <Renderer/IRenderer.h>

namespace Renderer {	
	enum struct ShaderSourceType;

	namespace OpenGL {
		class OpenGLRenderer : public IRenderer
		{
		public:
			static void checkGLError();

			OpenGLRenderer();
			~OpenGLRenderer();

			void createPendingResources() override;
			void destroyPendingResources() override;

			void processRenderingOptions(RenderOptions& options) override;
			void updateConstantBuffers(std::unordered_set<std::shared_ptr<ShaderConstantBuffer>>& constantBuffers) override;
			void draw(const std::vector<std::shared_ptr<const ModelInstance>>& modelInstances, SortType sortType, const std::unordered_map<std::string, std::shared_ptr<ShaderConstantBuffer>>& globalShaderConstantBuffers) override;

		};
	}
}
