#include "ppch.h"
#include "LayerStack.h"

namespace PriMech {
	LayerStack::LayerStack() {}

	LayerStack::~LayerStack() {
		for (Layer* layer : layers_) {
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		layers_.emplace(layers_.begin() + layerInsertIndex_, layer);
		layerInsertIndex_++;
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		layers_.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto iterator = std::find(layers_.begin(), layers_.end(), layer);
		if (iterator != layers_.begin() + layerInsertIndex_) {
			layer->OnDetach();
			layers_.erase(iterator); 
			layerInsertIndex_--;
		}
	}
	
	void LayerStack::PopOverlay(Layer* overlay) {
		auto iterator = std::find(layers_.begin(), layers_.end(), overlay);
		if (iterator != layers_.end()) { 
			overlay->OnDetach();
			layers_.erase(iterator); 
		}
	}
}