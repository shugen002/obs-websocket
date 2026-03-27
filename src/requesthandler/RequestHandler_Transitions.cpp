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

#include <math.h>

#include "RequestHandler.h"

/**
 * Gets an array of all available transition kinds.
 *
 * Similar to `GetInputKindList`
 *
 * @responseField transitionKinds | Array<String> | Array of transition kinds
 *
 * @requestType GetTransitionKindList
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::GetTransitionKindList(const Request &)
{
	json responseData;
	responseData["transitionKinds"] = Utils::Obs::ArrayHelper::GetTransitionKindList();
	return RequestResult::Success(responseData);
}

/**
 * Gets an array of all scene transitions in OBS.
 *
 * @responseField currentSceneTransitionName | String         | Name of the current scene transition. Can be null
 * @responseField currentSceneTransitionUuid | String         | UUID of the current scene transition. Can be null
 * @responseField currentSceneTransitionKind | String         | Kind of the current scene transition. Can be null
 * @responseField transitions                | Array<Object> | Array of transitions
 *
 * @requestType GetSceneTransitionList
 * @complexity 3
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::GetSceneTransitionList(const Request &)
{
	json responseData;
	responseData["currentSceneTransitionName"] = nullptr;
	responseData["currentSceneTransitionUuid"] = nullptr;
	responseData["currentSceneTransitionKind"] = nullptr;
	responseData["transitions"] = Utils::Obs::ArrayHelper::GetSceneTransitionList();
	return RequestResult::Success(responseData);
}

/**
 * Gets information about the current scene transition.
 *
 * @responseField transitionName         | String  | Name of the transition
 * @responseField transitionUuid         | String  | UUID of the transition
 * @responseField transitionKind         | String  | Kind of the transition
 * @responseField transitionFixed        | Boolean | Whether the transition uses a fixed (unconfigurable) duration
 * @responseField transitionDuration     | Number  | Configured transition duration in milliseconds. `null` if transition is fixed
 * @responseField transitionConfigurable | Boolean | Whether the transition supports being configured
 * @responseField transitionSettings     | Object  | Object of settings for the transition. `null` if transition is not configurable
 *
 * @requestType GetCurrentSceneTransition
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::GetCurrentSceneTransition(const Request &)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Sets the current scene transition.
 *
 * Small note: While the namespace of scene transitions is generally unique, that uniqueness is not a guarantee as it is with other resources like inputs.
 *
 * @requestField transitionName | String | Name of the transition to make active
 *
 * @requestType SetCurrentSceneTransition
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::SetCurrentSceneTransition(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("transitionName", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Sets the duration of the current scene transition, if it is not fixed.
 *
 * @requestField transitionDuration | Number | Duration in milliseconds | >= 50, <= 20000
 *
 * @requestType SetCurrentSceneTransitionDuration
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::SetCurrentSceneTransitionDuration(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateNumber("transitionDuration", statusCode, comment, 50, 20000))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Sets the settings of the current scene transition.
 *
 * @requestField transitionSettings | Object  | Settings object to apply to the transition. Can be `{}`
 * @requestField ?overlay           | Boolean | Whether to overlay over the current settings or replace them | true
 *
 * @requestType SetCurrentSceneTransitionSettings
 * @complexity 3
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::SetCurrentSceneTransitionSettings(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateObject("transitionSettings", statusCode, comment, true))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Gets the cursor position of the current scene transition.
 *
 * Note: `transitionCursor` will return 1.0 when the transition is inactive.
 *
 * @responseField transitionCursor | Number | Cursor position, between 0.0 and 1.0
 *
 * @requestType GetCurrentSceneTransitionCursor
 * @complexity 2
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category transitions
 */
RequestResult RequestHandler::GetCurrentSceneTransitionCursor(const Request &)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::TriggerStudioModeTransition(const Request &)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SetTBarPosition(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateNumber("position", statusCode, comment, 0.0, 1.0))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}
