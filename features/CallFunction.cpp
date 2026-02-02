#include "CallFunction.h"
#include <memory>
#include <cstdarg>
#include <cstring>
#include "../common.h"
#include "../ErectusProcess.h"
#include "../game/Game.h"
#include "../utils.h"
#include "../dependencies/fmt/fmt/format.h"

bool CallFunction::ApplyCombatSpell(std::uint32_t spellFormId)
{
    // Check if offsets are configured
    if (OFFSET_SCRIPT_CALLFUNCTION == 0 || OFFSET_SCRIPT_CALLFUNCTIONFORMAT == 0 || OFFSET_SCRIPT_INITIALIZESCRIPT == 0)
        return false;

    auto formIdStr = fmt::format("{:08X}", spellFormId);
    return Player(3, "Actor.DoCombatSpellApply", formIdStr.c_str(), "Player");
}

bool CallFunction::PlayIdleWithTarget(std::uint32_t idleFormId)
{
    // Check if offsets are configured
    if (OFFSET_SCRIPT_CALLFUNCTION == 0 || OFFSET_SCRIPT_CALLFUNCTIONFORMAT == 0 || OFFSET_SCRIPT_INITIALIZESCRIPT == 0)
        return false;

    auto idleStr = fmt::format("{:08X}", idleFormId);
    return Player(3, "Actor.PlayIdleWithTarget", idleStr.c_str(), "Player");
}

bool CallFunction::Player(short argc, ...)
{
    // Get local player reference
    auto localPlayer = Game::GetLocalPlayer();
    if (!Utils::Valid(localPlayer.ptr))
        return false;

    va_list args;
    va_start(args, argc);
    bool result = Execute(localPlayer.ptr, args, argc);
    va_end(args);

    return result;
}

bool CallFunction::Execute(std::uintptr_t reference, va_list args, int argc)
{
    constexpr short MaxParamSize = 4096;
    
    // Pack parameters into buffer
    // Format: [8 bytes padding][argc as short][for each param: length as short, string bytes...]
    BYTE params[MaxParamSize];
    memset(params, 0, sizeof(params));

    short offset = 8;  // Skip 8 bytes padding
    *reinterpret_cast<short*>(&params[offset]) = static_cast<short>(argc <= 11 ? argc : 11);
    offset += sizeof(short);

    short actualArgc = *reinterpret_cast<short*>(&params[8]);
    for (short i = 0; i < actualArgc; i++)
    {
        short remainingSpace = MaxParamSize - offset;
        if (remainingSpace <= 0 || remainingSpace >= MaxParamSize)
            return false;

        const char* param = va_arg(args, const char*);
        size_t length = strnlen(param, remainingSpace);

        *reinterpret_cast<short*>(&params[offset]) = static_cast<short>(length);
        offset += sizeof(short);

        remainingSpace = MaxParamSize - offset - static_cast<short>(length);
        if (remainingSpace <= 0 || remainingSpace >= MaxParamSize)
            return false;

        for (short c = 0; c <= static_cast<short>(length); c++)
        {
            params[offset] = param[c];
            offset += params[offset] == '\0' ? 0 : 1;
        }
    }

    // Allocate memory for parameters in target process
    auto paramsAlloc = ErectusProcess::AllocEx(offset);
    if (!paramsAlloc)
        return false;

    if (!ErectusProcess::Wpm(paramsAlloc, params, offset))
    {
        ErectusProcess::FreeEx(paramsAlloc);
        return false;
    }

    // Build shellcode data structure
    ShellcodeData shellcodeData = {};
    shellcodeData.callFunction = ErectusProcess::exe + OFFSET_SCRIPT_CALLFUNCTION;
    shellcodeData.initializeScript = ErectusProcess::exe + OFFSET_SCRIPT_INITIALIZESCRIPT;
    shellcodeData.format = ErectusProcess::exe + OFFSET_SCRIPT_CALLFUNCTIONFORMAT;
    shellcodeData.params = reinterpret_cast<void*>(paramsAlloc);
    shellcodeData.reference = reference;

    // The shellcode that calls the game's script function
    // This initializes a script context and calls CallFunction with the packed parameters
    BYTE shellcode[] = {
        0x48, 0x89, 0x5C, 0x24, 0x20, 0x55, 0x48, 0x8D, 0x6C, 0x24, 0xA9, 0x48, 0x81, 0xEC, 0xD0, 0x00,
        0x00, 0x00, 0x33, 0xC0, 0x48, 0x8B, 0xD9, 0x48, 0x89, 0x45, 0xC7, 0x48, 0x8D, 0x4D, 0xC7, 0x48,
        0x89, 0x45, 0xCF, 0x48, 0x89, 0x45, 0xD7, 0x48, 0x89, 0x45, 0xDF, 0x48, 0x89, 0x45, 0xE7, 0x48,
        0x89, 0x45, 0xEF, 0x48, 0x89, 0x45, 0xF7, 0x48, 0x89, 0x45, 0xFF, 0x48, 0x89, 0x45, 0x07, 0x48,
        0x89, 0x45, 0x0F, 0x48, 0x89, 0x45, 0x17, 0x48, 0x89, 0x45, 0x1F, 0x48, 0x89, 0x45, 0x27, 0x48,
        0x89, 0x45, 0x2F, 0x48, 0x89, 0x45, 0x37, 0x48, 0x89, 0x45, 0x3F, 0x48, 0x89, 0x45, 0x47, 0x48,
        0x8D, 0x45, 0xC7, 0x48, 0x89, 0x45, 0x67, 0xFF, 0x53, 0x08, 0x4C, 0x8B, 0x43, 0x20, 0x48, 0x8D,
        0x45, 0x6F, 0x48, 0x8B, 0x53, 0x18, 0x45, 0x33, 0xC9, 0x48, 0x8B, 0x4B, 0x10, 0x48, 0x89, 0x44,
        0x24, 0x38, 0x48, 0x8D, 0x45, 0x77, 0x48, 0x89, 0x44, 0x24, 0x30, 0x48, 0x8D, 0x45, 0x67, 0x48,
        0x89, 0x44, 0x24, 0x28, 0x48, 0x8B, 0x45, 0x67, 0x48, 0x89, 0x44, 0x24, 0x20, 0x48, 0xC7, 0x45,
        0x77, 0x08, 0x00, 0x00, 0x00, 0x48, 0xC7, 0x45, 0x6F, 0x08, 0x00, 0x00, 0x00, 0xFF, 0x13, 0x48,
        0x8B, 0x9C, 0x24, 0xF8, 0x00, 0x00, 0x00, 0x48, 0x81, 0xC4, 0xD0, 0x00, 0x00, 0x00, 0x5D, 0xC3,
    };

    // Calculate total allocation size
    const size_t totalSize = sizeof(shellcode) + sizeof(ShellcodeData);
    
    // Build the payload: shellcode followed by data
    auto payload = std::make_unique<BYTE[]>(totalSize);
    memcpy(payload.get(), shellcode, sizeof(shellcode));
    memcpy(payload.get() + sizeof(shellcode), &shellcodeData, sizeof(ShellcodeData));

    // Allocate executable memory
    auto codeAlloc = ErectusProcess::AllocEx(totalSize);
    if (!codeAlloc)
    {
        ErectusProcess::FreeEx(paramsAlloc);
        return false;
    }

    // Write payload
    if (!ErectusProcess::Wpm(codeAlloc, payload.get(), totalSize))
    {
        ErectusProcess::FreeEx(codeAlloc);
        ErectusProcess::FreeEx(paramsAlloc);
        return false;
    }

    // Make executable
    if (!ErectusProcess::ProtectEx(codeAlloc, totalSize))
    {
        ErectusProcess::FreeEx(codeAlloc);
        ErectusProcess::FreeEx(paramsAlloc);
        return false;
    }

    // Execute - pass pointer to ShellcodeData as parameter
    auto dataAddress = codeAlloc + sizeof(shellcode);
    auto* thread = CreateRemoteThread(
        ErectusProcess::handle,
        nullptr,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(codeAlloc),
        reinterpret_cast<LPVOID>(dataAddress),
        0,
        nullptr
    );

    if (!thread)
    {
        ErectusProcess::FreeEx(codeAlloc);
        ErectusProcess::FreeEx(paramsAlloc);
        return false;
    }

    // Wait for completion with timeout
    const auto threadResult = WaitForSingleObject(thread, 3000);
    CloseHandle(thread);

    // Cleanup
    ErectusProcess::FreeEx(codeAlloc);
    ErectusProcess::FreeEx(paramsAlloc);

    return threadResult == WAIT_OBJECT_0;
}
