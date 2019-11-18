#include "StateMachine.hpp"

namespace SNAKY {
	void StateMachine::ReplaceState(StateRef newState) {
		this->_newState = std::move(newState);
	}

	StateRef& StateMachine::GetActiveState() {
		return this->_newState;
	}
}