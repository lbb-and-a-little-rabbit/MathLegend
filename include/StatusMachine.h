#pragma once
#include <memory>
#include "Status.h"

class StatusMachine{
public:

    void changeState(std::unique_ptr<Status> newStatus){
        current = std::move(newStatus);
    }

    Status* get(){
        return current.get();
    }

private:
    std::unique_ptr<Status> current;
};