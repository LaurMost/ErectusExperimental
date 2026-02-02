#pragma once
#include <cstdint>
#include <string>

class CallFunction final {
public:
    // Apply a combat spell to the player (e.g., jetpack with no AP drain)
    static bool ApplyCombatSpell(std::uint32_t spellFormId);
    
    // Play an idle animation with a target (target is always "Player")
    static bool PlayIdleWithTarget(std::uint32_t idleFormId);

private:
    struct ShellcodeData {
        std::uintptr_t callFunction;
        std::uintptr_t initializeScript;
        std::uintptr_t format;
        void* params;
        std::uintptr_t reference;
    };

    // Internal function to call a script function on the local player
    static bool Player(short argc, ...);
    
    // Core implementation that handles shellcode execution
    static bool Execute(std::uintptr_t reference, va_list args, int argc);

    virtual void Dummy() = 0;
};
