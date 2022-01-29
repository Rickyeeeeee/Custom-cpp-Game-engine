#include "pch.h"
#include "RenderCommand.h"

#include "OpenGl/OpenGLRendererAPI.h"

RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;