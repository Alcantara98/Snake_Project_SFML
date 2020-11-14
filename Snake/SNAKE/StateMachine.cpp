#include "StateMachine.hpp"

namespace SNAKY {
	void StateMachine::ReplaceState(std::unique_ptr<State> newState) {
		this->_newState = std::move(newState);
	}

	std::unique_ptr<State>& StateMachine::GetActiveState() {
		return this->_newState;
	}
}