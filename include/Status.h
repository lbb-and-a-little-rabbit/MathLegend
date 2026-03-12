#pragma once

#include "PublicResources.h"

enum class StatusAssemble{
    None,
    toGame,
    toMenu,
    Exit
};

class Status{
public:
    virtual ~Status() = default;

    virtual StatusAssemble run() {return StatusAssemble::Exit;};
};