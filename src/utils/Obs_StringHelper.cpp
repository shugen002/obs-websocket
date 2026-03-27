/*
obs-websocket
Copyright (C) 2016-2021 Stephane Lepin <stephane.lepin@gmail.com>
Copyright (C) 2020-2021 Kyle Manning <tt2468@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program. If not, see <https://www.gnu.org/licenses/>
*/

#include <inttypes.h>
#include <stdio.h>

#include <obs-module.h>

#include <util/util.hpp>

#include "Obs.h"
#include "plugin-macros.generated.h"

std::string Utils::Obs::StringHelper::GetObsVersion()
{
	uint32_t version = obs_get_version();

	uint8_t major, minor, patch;
	major = (version >> 24) & 0xFF;
	minor = (version >> 16) & 0xFF;
	patch = version & 0xFF;

	char buf[32];
	snprintf(buf, sizeof(buf), "%u.%u.%u", (unsigned)major, (unsigned)minor, (unsigned)patch);
	return std::string(buf);
}

std::string Utils::Obs::StringHelper::GetModuleConfigPath(std::string fileName)
{
	BPtr<char> configPath = obs_module_config_path(fileName.c_str());
	return std::string(configPath.Get());
}

std::string Utils::Obs::StringHelper::GetCurrentSceneCollection()
{
	return "";
}

std::string Utils::Obs::StringHelper::GetCurrentProfile()
{
	return "";
}

std::string Utils::Obs::StringHelper::GetCurrentProfilePath()
{
	return "";
}

std::string Utils::Obs::StringHelper::GetCurrentRecordOutputPath()
{
	return "";
}

std::string Utils::Obs::StringHelper::GetLastRecordFileName()
{
	return "";
}

std::string Utils::Obs::StringHelper::GetLastReplayBufferFileName()
{
	return "";
}

std::string Utils::Obs::StringHelper::GetLastScreenshotFileName()
{
	return "";
}

std::string Utils::Obs::StringHelper::DurationToTimecode(uint64_t ms)
{
	uint64_t secs = ms / 1000ULL;
	uint64_t minutes = secs / 60ULL;

	uint64_t hoursPart = minutes / 60ULL;
	uint64_t minutesPart = minutes % 60ULL;
	uint64_t secsPart = secs % 60ULL;
	uint64_t msPart = ms % 1000ULL;

	char buf[32];
	snprintf(buf, sizeof(buf), "%02" PRIu64 ":%02" PRIu64 ":%02" PRIu64 ".%03" PRIu64,
		 hoursPart, minutesPart, secsPart, msPart);
	return std::string(buf);
}
