#pragma once
#include "Primech/Core.h"
#include "Layer.h"

#include <vector>
namespace PriMech {
	class PRIMECH_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return layers_.begin(); }
		std::vector<Layer*>::iterator end() { return layers_.end(); }
	private:
		std::vector<Layer*> layers_;
		unsigned int layerInsertIndex_ = 0;
	};
}
