#include "ppch.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace PriMech {
	Scope<RendererAPI> RendererCommand::s_RendererAPI_ = CreateScope<OpenGLRendererAPI>();
}