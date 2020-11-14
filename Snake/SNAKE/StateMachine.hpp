#pragma once

#include <memory>
#include <stack>
#include "State.hpp"
#include "DEFINITIONS.hpp"

namespace SNAKY {
	class StateMachine {
	public:
		void ReplaceState(std::unique_ptr<State> newState);
		std::unique_ptr<State>& GetActiveState();

	private:
		std::unique_ptr<State> _newState;
	};
}