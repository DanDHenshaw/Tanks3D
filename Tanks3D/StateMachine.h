#pragma once

#include <memory>
#include <stack>

#include "State.h"

typedef std::unique_ptr<State> StateRef;

class StateMachine
{
public:
	StateMachine() {}
	~StateMachine() {}

	/*
	 Adds a new state to the stack.
		- newState is the state to add to the stack.
		- isReplacing is whether the new state should replace the current state.
	 */
	void AddState(StateRef newState, bool isReplacing = true, bool isReplacingAll = false);
	/*
	 Removes the last state in the stack.
	 */
	void RemoveState();

	/*
	 Updates the changes when a state is added or removed.
	 */
	void ProcessStateChanges();

	/*
	 A pointer to the current active state.
	 */
	StateRef& GetActiveState();

	void Release();

private:
	// Reference to the states on the stack.
	std::stack<StateRef> _states;
	// New state to be added to the stack.
	StateRef _newState;

	// If the program is removing the current state.
	bool _isRemoving;
	bool _isClosing = false;
	// If the program is adding a new state.
	bool _isAdding;
	// If the program is replacing the current state with a new state.
	bool _isReplacing;
  // If the program is replacing the current state with a new state.
  bool _isReplacingAll;
};