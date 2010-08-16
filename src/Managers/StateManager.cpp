#include "Managers/StateManager.hpp"

void StateManager::push(const StatePtr &state)
{
	mStates.push_back(state);
}

StateManager::~StateManager()
{
	while (!mStates.empty())
	{
		mStates.pop_back();
	}
}

State *StateManager::pop()
{
	mActiveState = mStates.back();	// Store pointer for next state to use
	mStates.pop_back();				// And remove it from list
}

State *StateManager::getActiveState()
{
	return mActiveState.get();
}