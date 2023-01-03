#pragma once

// FOR USE BY PRIMECH APPS e.g. Sandbox

#include "Primech/Core/Application.h"
#include "Primech/Core/Logging/Log.h"
#include "Primech/Core/Input.h"
#include "Primech/Core/Codes/KeyCodes.h"
#include "Primech/Core/Codes/MouseButtonCodes.h"
#include "Primech/Core/LayerStack/Layer.h"
#include "Primech/Core/Timestep.h"

#include "Primech/ImGui/ImGuiLayer.h"



//-----------------Renderer---------------------
#include "Primech/Renderer/camera/OrthographicCamera.h"
#include "Primech/Renderer/camera/OrthographicCameraController.h"
#include "Primech/Renderer/RendererCommand.h"
#include "Primech/Renderer/Renderer.h"
#include "Primech/Renderer/Shader.h"
#include "Primech/Renderer/Texture.h"
#include "Primech/Renderer/Buffer.h"
#include "Primech/Renderer/VertexArray.h"
//----------------------------------------------

//---------------Entry Point--------------------

#include "Primech/Core/Entrypoint.h"

//----------------------------------------------