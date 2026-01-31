#include "../common.h"

#include "Game.h"
#include "../ErectusProcess.h"
#include "../utils.h"
#include "Datatypes/TesWorldSpace.h"

LoadedAreaManager Game::GetLoadedAreaManager()
{
	LoadedAreaManager result = {};

	std::uintptr_t loadedAreaManagerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_ENTITY_LIST, &loadedAreaManagerPtr, sizeof loadedAreaManagerPtr))
		return result;
	if (!Utils::Valid(loadedAreaManagerPtr))
		return result;

	ErectusProcess::Rpm(loadedAreaManagerPtr, &result, sizeof result);

	return result;
}

LocalPlayer Game::GetLocalPlayer()
{
	LocalPlayer result = {};

	std::uintptr_t localPlayerPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_LOCAL_PLAYER, &localPlayerPtr, sizeof localPlayerPtr))
		return result;
	if (!Utils::Valid(localPlayerPtr))
		return result;

	ErectusProcess::Rpm(localPlayerPtr, &result, sizeof result);
	result.ptr = localPlayerPtr;

	return result;
}

std::vector<TesObjectCell> Game::GetLoadedCells()
{
	auto result = GetLoadedAreaManager().GetLoadedCells();
	result.push_back(GetSkyCell());

	return result;
}

Camera Game::GetPlayerCamera()
{
	Camera result = {};

	std::uintptr_t cameraPtr;
	if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_CAMERA, &cameraPtr, sizeof cameraPtr))
		return result;

	if (!Utils::Valid(cameraPtr))
		return result;

	if (!ErectusProcess::Rpm(cameraPtr, &result, sizeof result))
		return result;

	return result;
}

TesObjectCell Game::GetSkyCell()
{
    TesObjectCell result = {};

    // Pointer chain: [[[OFFSET_LOCAL_PLAYER]+0x0A8]+0x0100]
    std::uintptr_t localPlayerPtr = 0;
    if (!ErectusProcess::Rpm(ErectusProcess::exe + OFFSET_LOCAL_PLAYER, &localPlayerPtr, sizeof localPlayerPtr))
        return result;
    if (!Utils::Valid(localPlayerPtr))
        return result;

    std::uintptr_t intermediatePtr = 0;
    if (!ErectusProcess::Rpm(localPlayerPtr + 0x0A8, &intermediatePtr, sizeof intermediatePtr))
        return result;
    if (!Utils::Valid(intermediatePtr))
        return result;

    std::uintptr_t worldspacePtr = 0;
    if (!ErectusProcess::Rpm(intermediatePtr + 0x0100, &worldspacePtr, sizeof worldspacePtr))
        return result;
    if (!Utils::Valid(worldspacePtr))
        return result;

    TesWorldSpace worldspace = {};
    if (!ErectusProcess::Rpm(worldspacePtr, &worldspace, sizeof worldspace))
        return result;

    ErectusProcess::Rpm(worldspace.skyCellPtr, &result, sizeof result);

    return result;
}
