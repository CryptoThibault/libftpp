template <typename TState>
void StateMachine<TState>::addState(const TState& state)
{
    states.insert(state);
}

template <typename TState>
void StateMachine<TState>::addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda)
{
    if (states.find(startState) == states.end() || states.find(finalState) == states.end())
        throw std::invalid_argument("State not registered");

    transitions[startState] = {finalState, lambda};
}

template <typename TState>
void StateMachine<TState>::addAction(const TState& state, const std::function<void()>& lambda)
{
    if (states.find(state) == states.end())
        throw std::invalid_argument("State not registered");

    actions[state] = lambda;
}

template <typename TState>
void StateMachine<TState>::transitionTo(const TState& state)
{
    auto it = transitions.find(currentState);
    if (it == transitions.end() || it->second.first != state)
        throw std::invalid_argument("Transition not allowed");

    if (it->second.second)
        it->second.second();

    currentState = state;
}

template <typename TState>
void StateMachine<TState>::update()
{
    auto it = actions.find(currentState);
    if (it != actions.end())
        it->second();
}