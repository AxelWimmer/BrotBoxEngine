#include "BBE/EngineSettings.h"
#include "BBE/Exceptions.h"

static bool started = false;
static int amountOfLightSources = 4;

void bbe::Settings::INTERNAL_start()
{
	started = true;
}

int bbe::Settings::getAmountOfLightSources()
{
	//UNTESTED
	return amountOfLightSources;
}

void bbe::Settings::setAmountOfLightSources(int amount)
{
	//UNTESTED
	if (started)
	{
		throw AlreadyStartedException();
	}

	amountOfLightSources = amount;
}

int bbe::Settings::getTerrainAdditionalTextures()
{
	//UNTESTED
	return 2;
}
