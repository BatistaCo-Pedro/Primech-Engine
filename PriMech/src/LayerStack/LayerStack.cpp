#include "ppch.h"
#include "LayerStack.h"

namespace PriMech {
	LayerStack::LayerStack() {
		layerInsertIt = layers_.begin();
	}

	LayerStack::~LayerStack() {
		for (Layer* layer : layers_) {
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer) {
		layerInsertIt = layers_.emplace(layerInsertIt, layer);
	}

	void LayerStack::PushOverlay(Layer* overlay) {
		layers_.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer) {
		auto iterator = std::find(layers_.begin(), layers_.end(), layer);
		if (iterator != layers_.end()) { 
			layers_.erase(iterator); 
			layerInsertIt--;
		}
	}
	
	void LayerStack::PopLayer(Layer* layer) {
		auto iterator = std::find(layers_.begin(), layers_.end(), layer);
		if (iterator != layers_.end()) { layers_.erase(iterator); }
	}
}