#pragma once
#include "ScreenState.h"

DECLARE_SMART(LoginScreenState, spLoginScreenState)
class LoginScreenState : public ScreenState {
public:
    void enter(ScreenActor& screen) override;
};