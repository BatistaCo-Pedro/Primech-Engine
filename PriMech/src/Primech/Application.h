/*
 * This file is part of PriMech.
 *
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Apache License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#pragma once
#include "Core.h"
#include "PriMech/Events/Event.h"
#include "PriMech/LayerStack/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "ImGui/ImGuiLayer.h"

#include "Primech/Core/Timestep.h"

//General Namespace for everything included in the Engine itself 
namespace PriMech {
	//PriMech_API is defined in Core.h
	class Application
	{
	public:
		//Methods for Applications using PriMech
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& GetApplication() { return *instance_; }
		inline Window& GetWindow() { return *windowPtr_; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		Scope<Window> windowPtr_;
		ImGuiLayer* imGuiLayer_;
		bool running_ = true;
		LayerStack layerStack_;

		//Pointer to the App instance
		//accessible throught the whole code base to access important information such as the App window
		static Application* instance_; 

		float lastFrameTime_ = 0.0f;
	};

	//defined in client e.g Sandbox
	Application* CreateApplication();
}