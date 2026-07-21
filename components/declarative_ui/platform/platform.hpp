#pragma once

namespace DeclarativeUI::Platform {

using LockFunction = bool (*)(int timeoutMilliseconds);
using UnlockFunction = void (*)();

void configure(
    LockFunction lockFunction,
    UnlockFunction unlockFunction
);

bool lock(int timeoutMilliseconds);
void unlock();

}