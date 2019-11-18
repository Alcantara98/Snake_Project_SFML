#pragma once

#include <memory>
#include <stack>
#include "State.hpp"
#include "DEFINITIONS.hpp"

namespace SNAKY {
	typedef std::unique_ptr<State> StateRef;

	class StateMachine {
	public:
		void ReplaceState(StateRef newState);
		StateRef& GetActiveState();

	private:
		StateRef _newState;
	};
}