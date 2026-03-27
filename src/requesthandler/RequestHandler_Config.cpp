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

#include <util/config-file.h>

#include "RequestHandler.h"

#define GLOBAL_PERSISTENT_DATA_FILE_NAME "persistent_data.json"

/**
 * Gets the value of a "slot" from the selected persistent data realm.
 *
 * @requestField realm    | String | The data realm to select. `OBS_WEBSOCKET_DATA_REALM_GLOBAL` or `OBS_WEBSOCKET_DATA_REALM_PROFILE`
 * @requestField slotName | String | The name of the slot to retrieve data from
 *
 * @responseField slotValue | Any | Value associated with the slot. `null` if not set
 *
 * @requestType GetPersistentData
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::GetPersistentData(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!(request.ValidateString("realm", statusCode, comment) && request.ValidateString("slotName", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	std::string realm = request.RequestData["realm"];
	std::string slotName = request.RequestData["slotName"];

	std::string persistentDataPath;
	if (realm == "OBS_WEBSOCKET_DATA_REALM_GLOBAL")
		persistentDataPath = Utils::Obs::StringHelper::GetModuleConfigPath(GLOBAL_PERSISTENT_DATA_FILE_NAME);
	else if (realm == "OBS_WEBSOCKET_DATA_REALM_PROFILE")
		persistentDataPath = Utils::Obs::StringHelper::GetCurrentProfilePath() + "/obsWebSocketPersistentData.json";
	else
		return RequestResult::Error(RequestStatus::ResourceNotFound,
					    "You have specified an invalid persistent data realm.");

	json responseData;
	json persistentData;
	if (Utils::Json::GetJsonFileContent(persistentDataPath, persistentData) && persistentData.contains(slotName))
		responseData["slotValue"] = persistentData[slotName];
	else
		responseData["slotValue"] = nullptr;

	return RequestResult::Success(responseData);
}

/**
 * Sets the value of a "slot" from the selected persistent data realm.
 *
 * @requestField realm     | String | The data realm to select. `OBS_WEBSOCKET_DATA_REALM_GLOBAL` or `OBS_WEBSOCKET_DATA_REALM_PROFILE`
 * @requestField slotName  | String | The name of the slot to retrieve data from
 * @requestField slotValue | Any    | The value to apply to the slot
 *
 * @requestType SetPersistentData
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::SetPersistentData(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!(request.ValidateString("realm", statusCode, comment) && request.ValidateString("slotName", statusCode, comment) &&
	      request.ValidateBasic("slotValue", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	std::string realm = request.RequestData["realm"];
	std::string slotName = request.RequestData["slotName"];
	json slotValue = request.RequestData["slotValue"];

	std::string persistentDataPath;
	if (realm == "OBS_WEBSOCKET_DATA_REALM_GLOBAL")
		persistentDataPath = Utils::Obs::StringHelper::GetModuleConfigPath(GLOBAL_PERSISTENT_DATA_FILE_NAME);
	else if (realm == "OBS_WEBSOCKET_DATA_REALM_PROFILE")
		persistentDataPath = Utils::Obs::StringHelper::GetCurrentProfilePath() + "/obsWebSocketPersistentData.json";
	else
		return RequestResult::Error(RequestStatus::ResourceNotFound,
					    "You have specified an invalid persistent data realm.");

	json persistentData;
	Utils::Json::GetJsonFileContent(persistentDataPath, persistentData);
	persistentData[slotName] = slotValue;
	if (!Utils::Json::SetJsonFileContent(persistentDataPath, persistentData))
		return RequestResult::Error(RequestStatus::RequestProcessingFailed,
					    "Unable to write persistent data. No permissions?");

	return RequestResult::Success();
}

/**
 * Gets an array of all scene collections
 *
 * @responseField currentSceneCollectionName | String        | The name of the current scene collection
 * @responseField sceneCollections           | Array<String> | Array of all available scene collections
 *
 * @requestType GetSceneCollectionList
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::GetSceneCollectionList(const Request &)
{
	json responseData;
	responseData["currentSceneCollectionName"] = Utils::Obs::StringHelper::GetCurrentSceneCollection();
	responseData["sceneCollections"] = Utils::Obs::ArrayHelper::GetSceneCollectionList();
	return RequestResult::Success(responseData);
}

/**
 * Switches to a scene collection.
 *
 * Note: This will block until the collection has finished changing.
 *
 * @requestField sceneCollectionName | String | Name of the scene collection to switch to
 *
 * @requestType SetCurrentSceneCollection
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::SetCurrentSceneCollection(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("sceneCollectionName", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Creates a new scene collection, switching to it in the process.
 *
 * Note: This will block until the collection has finished changing.
 *
 * @requestField sceneCollectionName | String | Name for the new scene collection
 *
 * @requestType CreateSceneCollection
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::CreateSceneCollection(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("sceneCollectionName", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Gets an array of all profiles
 *
 * @responseField currentProfileName | String        | The name of the current profile
 * @responseField profiles           | Array<String> | Array of all available profiles
 *
 * @requestType GetProfileList
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::GetProfileList(const Request &)
{
	json responseData;
	responseData["currentProfileName"] = Utils::Obs::StringHelper::GetCurrentProfile();
	responseData["profiles"] = Utils::Obs::ArrayHelper::GetProfileList();
	return RequestResult::Success(responseData);
}

/**
 * Switches to a profile.
 *
 * @requestField profileName | String | Name of the profile to switch to
 *
 * @requestType SetCurrentProfile
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::SetCurrentProfile(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("profileName", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Creates a new profile, switching to it in the process
 *
 * @requestField profileName | String | Name for the new profile
 *
 * @requestType CreateProfile
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::CreateProfile(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("profileName", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Removes a profile. If the current profile is chosen, it will change to a different profile first.
 *
 * @requestField profileName | String | Name of the profile to remove
 *
 * @requestType RemoveProfile
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::RemoveProfile(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("profileName", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Gets a parameter from the current profile's configuration.
 *
 * @requestField parameterCategory | String | Category of the parameter to get
 * @requestField parameterName     | String | Name of the parameter to get
 *
 * @responseField parameterValue        | String | Value associated with the parameter. `null` if not set and no default
 * @responseField defaultParameterValue | String | Default value associated with the parameter. `null` if no default
 *
 * @requestType GetProfileParameter
 * @complexity 4
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::GetProfileParameter(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!(request.ValidateString("parameterCategory", statusCode, comment) &&
	      request.ValidateString("parameterName", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Sets the value of a parameter in the current profile's configuration.
 *
 * @requestField parameterCategory | String | Category of the parameter to set
 * @requestField parameterName     | String | Name of the parameter to set
 * @requestField parameterValue    | String | Value of the parameter to set. Use `null` to delete
 *
 * @requestType SetProfileParameter
 * @complexity 4
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::SetProfileParameter(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!(request.ValidateString("parameterCategory", statusCode, comment) &&
	      request.ValidateString("parameterName", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Gets the current video settings.
 *
 * Note: To get the true FPS value, divide the FPS numerator by the FPS denominator. Example: `60000/1001`
 *
 * @responseField fpsNumerator   | Number | Numerator of the fractional FPS value
 * @responseField fpsDenominator | Number | Denominator of the fractional FPS value
 * @responseField baseWidth      | Number | Width of the base (canvas) resolution in pixels
 * @responseField baseHeight     | Number | Height of the base (canvas) resolution in pixels
 * @responseField outputWidth    | Number | Width of the output resolution in pixels
 * @responseField outputHeight   | Number | Height of the output resolution in pixels
 *
 * @requestType GetVideoSettings
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::GetVideoSettings(const Request &)
{
	OBSCanvasAutoRelease mainCanvas = obs_get_main_canvas();
	if (!mainCanvas)
		return RequestResult::Error(RequestStatus::RequestProcessingFailed, "Unable to get internal main OBS canvas.");

	json responseData = Utils::Obs::ObjectHelper::GetCanvasVideoSettings(mainCanvas);

	return RequestResult::Success(responseData);
}

/**
 * Sets the current video settings.
 *
 * Note: Fields must be specified in pairs. For example, you cannot set only `baseWidth` without needing to specify `baseHeight`.
 *
 * @requestField ?fpsNumerator   | Number | Numerator of the fractional FPS value            | >= 1          | Not changed
 * @requestField ?fpsDenominator | Number | Denominator of the fractional FPS value          | >= 1          | Not changed
 * @requestField ?baseWidth      | Number | Width of the base (canvas) resolution in pixels  | >= 1, <= 4096 | Not changed
 * @requestField ?baseHeight     | Number | Height of the base (canvas) resolution in pixels | >= 1, <= 4096 | Not changed
 * @requestField ?outputWidth    | Number | Width of the output resolution in pixels         | >= 1, <= 4096 | Not changed
 * @requestField ?outputHeight   | Number | Height of the output resolution in pixels        | >= 1, <= 4096 | Not changed
 *
 * @requestType SetVideoSettings
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::SetVideoSettings(const Request &request)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Gets the current stream service settings (stream destination).
 *
 * @responseField streamServiceType     | String | Stream service type, like `rtmp_custom` or `rtmp_common`
 * @responseField streamServiceSettings | Object | Stream service settings
 *
 * @requestType GetStreamServiceSettings
 * @complexity 4
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category config
 * @api requests
 */
RequestResult RequestHandler::GetStreamServiceSettings(const Request &)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SetStreamServiceSettings(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!(request.ValidateString("streamServiceType", statusCode, comment) &&
	      request.ValidateObject("streamServiceSettings", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Gets the current directory that the record output is set to.
 *
 * @responseField recordDirectory | String | Output directory
 *
 * @requestType GetRecordDirectory
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category config
 */
RequestResult RequestHandler::GetRecordDirectory(const Request &)
{
	json responseData;
	responseData["recordDirectory"] = Utils::Obs::StringHelper::GetCurrentRecordOutputPath();

	return RequestResult::Success(responseData);
}

/**
 * Sets the current directory that the record output writes files to.
 *
 * @requestField recordDirectory | String | Output directory
 *
 * @requestType SetRecordDirectory
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.3.0
 * @api requests
 * @category config
 */
RequestResult RequestHandler::SetRecordDirectory(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("recordDirectory", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}
