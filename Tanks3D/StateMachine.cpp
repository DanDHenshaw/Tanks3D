#include "StateMachine.h"

void StateMachine::AddState(StateRef newState, bool isReplacing, bool isReplacingAll)
{
	// If state machine is closing then dont add state
	if (_isClosing) return;

	_isAdding = true;
	_isReplacing = isReplacing;
  _isReplacingAll = isReplacingAll;

	_newState = std::move(newState);
}

void StateMachine::RemoveState()
{
	_isRemoving = true;
}

void StateMachine::ProcessStateChanges()
{
	// If state machine is closing then dont process state changes
	if (_isClosing) return;

	if (_isRemoving && !_states.empty())
	{
		// Removes current state from stack
    _states.top()->Release();
		_states.pop();

		if (!_states.empty())
		{
			// Resumes the current state
			_states.top()->Resume();
		}

		_isRemoving = false;
	}

	if (_isAdding)
	{
		if (!_states.empty())
		{
			if (_isReplacing)
			{
        if (_isReplacingAll) 
        {
          _isReplacingAll = false;
          while (!_states.empty())
          {
            _states.top()->Release();
            _states.pop();
          }
        }
        else 
        {
          // if state is added and states isnt empty and current state is getting replaced then remove the current state
          _isReplacing = false;
          _states.top()->Release();
          _states.pop();
        }
			}
			else
			{
				// if state is added and states isnt empty then pause current state
				_states.top()->Pause();
			}
		}

		// Adds new state to the to of the stack
		_states.push(std::move(_newState));
		_states.top()->Initialise();
		_isAdding = false;
	}
}

StateRef& StateMachine::GetActiveState()
{
	// returns active state if not closing
	if (!_isClosing)
		return _states.top();
}

void StateMachine::Release()
{
	_isClosing = true;

	// loops through the stack and release and removes all states
  while (!_states.empty()) 
  {
    _states.top()->Release();
    _states.pop();
  }
}
