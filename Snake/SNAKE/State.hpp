#pragma once

namespace SNAKY {
	class State {
	public:
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
}