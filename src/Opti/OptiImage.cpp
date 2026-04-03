//
// Created by joshb on 2026/4/3.
//

#include "Opti/OptiImage.hpp"

#include <memory>
#include <SDL_surface.h>
#include <string>
#include <vector>

#include "Core/Drawable.hpp"
#include "Core/IndexBuffer.hpp"
#include "Core/Program.hpp"
#include "Core/Texture.hpp"
#include "Core/TextureUtils.hpp"
#include "Core/UniformBuffer.hpp"
#include "Core/VertexArray.hpp"
#include "Core/VertexBuffer.hpp"
#include "Opti/OptiRenderer.hpp"
#include "Util/AssetStore.hpp"
#include "Util/Image.hpp"
#include "Opti/OptiImage.hpp"

#include "Util/MissingTexture.hpp"

static std::shared_ptr<SDL_Surface> LoadSurface(const std::string &filepath) {
    auto surface = std::shared_ptr<SDL_Surface>(IMG_Load(filepath.c_str()),
                                                SDL_FreeSurface);

    if (surface == nullptr) {
        surface = {GetMissingTextureSDLSurface(), SDL_FreeSurface};
        LOG_ERROR("Failed to load image: '{}'", filepath);
        LOG_ERROR("{}", IMG_GetError());
    }

    return surface;
}

OptiImage::OptiImage(const std::string &filepath)
    : m_Path(filepath) {
    if (s_Program == nullptr) {
        InitProgram();
    }
    if (s_VertexArray == nullptr) {
        InitVertexArray();
    }

    m_UniformBuffer = std::make_unique<Core::UniformBuffer<Core::Matrices>>(
        *s_Program, "Matrices", 0);

    auto surface = s_Store.Get(filepath);

    m_Texture = std::make_unique<Core::Texture>(
        Core::SdlFormatToGlFormat(surface->format->format), surface->w,
        surface->h, surface->pixels);
    m_Size = {surface->w, surface->h};
}

void OptiImage::SetImage(const std::string &filepath) {
    auto surface = s_Store.Get(filepath);

    m_Texture->UpdateData(Core::SdlFormatToGlFormat(surface->format->format),
                          surface->w, surface->h, surface->pixels);
    m_Size = {surface->w, surface->h};
}

void OptiImage::Draw(const Core::Matrices &data) {
    OptiRenderer::Submit(this, data);
}

void OptiImage::InitProgram() {
    // TODO: Create `BaseProgram` from `Program` and pass it into `Drawable`
    s_Program =
        std::make_unique<Core::Program>("../PTSD/assets/shaders/Base.vert",
                                        "../PTSD/assets/shaders/Base.frag");
    s_Program->Bind();

    GLint location = glGetUniformLocation(s_Program->GetId(), "surface");
    glUniform1i(location, UNIFORM_SURFACE_LOCATION);
}

void OptiImage::InitVertexArray() {
    s_VertexArray = std::make_unique<Core::VertexArray>();

    // NOLINTBEGIN
    // These are vertex data for the rectangle but clang-tidy has magic
    // number warnings

    // Vertex
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            -0.5F, 0.5F,  //
            -0.5F, -0.5F, //
            0.5F, -0.5F,  //
            0.5F, 0.5F,   //
        },
        2));

    // UV
    s_VertexArray->AddVertexBuffer(std::make_unique<Core::VertexBuffer>(
        std::vector<float>{
            0.0F, 0.0F, //
            0.0F, 1.0F, //
            1.0F, 1.0F, //
            1.0F, 0.0F, //
        },
        2));

    // Index
    s_VertexArray->SetIndexBuffer(
        std::make_unique<Core::IndexBuffer>(std::vector<unsigned int>{
            0, 1, 2, //
            0, 2, 3, //
        }));
    // NOLINTEND
}

std::unique_ptr<Core::Program> OptiImage::s_Program = nullptr;
std::unique_ptr<Core::VertexArray> OptiImage::s_VertexArray = nullptr;

Util::AssetStore<std::shared_ptr<SDL_Surface>> OptiImage::s_Store(LoadSurface);