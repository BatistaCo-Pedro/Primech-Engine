#include "ppch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PriMech {
	RendererAPI* RendererCommand::s_RendererAPI_ = new OpenGLRendererAPI;
}