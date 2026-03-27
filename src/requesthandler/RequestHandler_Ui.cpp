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

#include "RequestHandler.h"

/**
 * Gets whether studio is enabled.
 *
 * @responseField studioModeEnabled | Boolean | Whether studio mode is enabled
 *
 * @requestType GetStudioModeEnabled
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category ui
 * @api requests
 */
RequestResult RequestHandler::GetStudioModeEnabled(const Request &)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SetStudioModeEnabled(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateBoolean("studioModeEnabled", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

/**
 * Opens the properties dialog of an input.
 *
 * @requestField ?inputName | String | Name of the input to open the dialog of
 * @requestField ?inputUuid | String | UUID of the input to open the dialog of
 *
 * @requestType OpenInputPropertiesDialog
 * @complexity 1
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @category ui
 * @api requests
 */
RequestResult RequestHandler::OpenInputPropertiesDialog(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease input = request.AcquireInput(statusCode, comment);
	if (!input)
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::OpenInputFiltersDialog(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease input = request.AcquireInput(statusCode, comment);
	if (!input)
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::OpenInputInteractDialog(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease input = request.AcquireInput(statusCode, comment);
	if (!input)
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::GetMonitorList(const Request &)
{
	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::OpenVideoMixProjector(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	if (!request.ValidateString("videoMixType", statusCode, comment))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::OpenSourceProjector(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease source = request.AcquireSource("canvasUuid", "sourceName", "sourceUuid", statusCode, comment);
	if (!source)
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}
