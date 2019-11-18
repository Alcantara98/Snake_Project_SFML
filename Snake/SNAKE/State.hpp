#pragma once

namespace SNAKY {
	class State {
	public:
		virtual void Update(float dt) = 0;
		virtual void Draw(float dt) = 0;
	};
}