#include "platform.hpp"

namespace DeclarativeUI::Platform {

namespace {

LockFunction configuredLock = nullptr;
UnlockFunction configuredUnlock = nullptr;

}

void configure(
    LockFunction lockFunction,
    UnlockFunction unlockFunction
)
{
    configuredLock = lockFunction;
    configuredUnlock = unlockFunction;
}

bool lock(int timeoutMilliseconds)
{
    if (configuredLock == nullptr) {
        return false;
    }

    return configuredLock(timeoutMilliseconds);
}

void unlock()
{
    if (configuredUnlock != nullptr) {
        configuredUnlock();
    }
}

}