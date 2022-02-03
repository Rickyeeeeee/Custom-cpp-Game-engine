#include "core/pch.h"
#include "Renderer/RenderCommand.h"

#include "OpenGl/OpenGLRendererAPI.h"

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;