#include "StateMachine.h"

void StateMachine::AddState(StateRef newState, bool isReplacing)
{
	if (_isClosing) return;

	_isAdding = true;
	_isReplacing = isReplacing;

	_newState = std::move(newState);
}

void StateMachine::RemoveState()
{
	_isRemoving = true;
}

void StateMachine::ProcessStateChanges()
{
	if (_isClosing) return;

	if (_isRemoving && !_states.empty())
	{
		StateRef oldState = (StateRef)_states.top().get();
		_states.pop();
		oldState.release();

		if (!_states.empty())
		{
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
				_states.pop();
			}
			else
			{
				_states.top()->Pause();
			}
		}

		_states.push(std::move(_newState));
		_states.top()->Initialise();
		_isAdding = false;
	}
}

StateRef& StateMachine::GetActiveState()
{
	if (!_isClosing)
		return _states.top();
}

void StateMachine::CloseStack()
{
	_isClosing = true;

	for (int i = 0; i < _states.size(); ++i)
	{
		_states.top()->Release();
		_states.pop();
	}
}
