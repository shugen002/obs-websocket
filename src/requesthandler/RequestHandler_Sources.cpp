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

RequestResult RequestHandler::GetSourceActive(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease source = request.AcquireSource("canvasUuid", "sourceName", "sourceUuid", statusCode, comment);
	if (!source)
		return RequestResult::Error(statusCode, comment);

	if (obs_source_get_type(source) != OBS_SOURCE_TYPE_INPUT && obs_source_get_type(source) != OBS_SOURCE_TYPE_SCENE)
		return RequestResult::Error(RequestStatus::InvalidResourceType, "The specified source is not an input or a scene.");

	json responseData;
	responseData["videoActive"] = obs_source_active(source);
	responseData["videoShowing"] = obs_source_showing(source);
	return RequestResult::Success(responseData);
}

/**
 * Gets a Base64-encoded screenshot of a source.
 *
 * The `imageWidth` and `imageHeight` parameters are treated as "scale to inner", meaning the smallest ratio will be used and the aspect ratio of the original resolution is kept.
 * If `imageWidth` and `imageHeight` are not specified, the compressed image will use the full resolution of the source.
 *
 * **Compatible with inputs and scenes.**
 *
 * @requestField ?canvasUuid              | String | UUID of the canvas the source is in, if using sourceName field
 * @requestField ?sourceName              | String | Name of the source to take a screenshot of
 * @requestField ?sourceUuid              | String | UUID of the source to take a screenshot of
 * @requestField imageFormat              | String | Image compression format to use. Use `GetVersion` to get compatible image formats
 * @requestField ?imageWidth              | Number | Width to scale the screenshot to                                                                                         | >= 8, <= 4096 | Source value is used
 * @requestField ?imageHeight             | Number | Height to scale the screenshot to                                                                                        | >= 8, <= 4096 | Source value is used
 * @requestField ?imageCompressionQuality | Number | Compression quality to use. 0 for high compression, 100 for uncompressed. -1 to use "default" (whatever that means, idk) | >= -1, <= 100 | -1
 *
 * @responseField imageData | String | Base64-encoded screenshot
 *
 * @requestType GetSourceScreenshot
 * @complexity 4
 * @rpcVersion -1
 * @initialVersion 5.0.0
 * @api requests
 * @category sources
 */
RequestResult RequestHandler::GetSourceScreenshot(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease source = request.AcquireSource("canvasUuid", "sourceName", "sourceUuid", statusCode, comment);
	if (!(source && request.ValidateString("imageFormat", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SaveSourceScreenshot(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease source = request.AcquireSource("canvasUuid", "sourceName", "sourceUuid", statusCode, comment);
	if (!(source && request.ValidateString("imageFormat", statusCode, comment) &&
	      request.ValidateString("imageFilePath", statusCode, comment)))
		return RequestResult::Error(statusCode, comment);

	return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

// Intentionally undocumented
RequestResult RequestHandler::GetSourcePrivateSettings(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease source = request.AcquireSource("canvasUuid", "sourceName", "sourceUuid", statusCode, comment);
	if (!source)
		return RequestResult::Error(statusCode, comment);

	OBSDataAutoRelease privateSettings = obs_source_get_private_settings(source);

	json responseData;
	responseData["sourceSettings"] = Utils::Json::ObsDataToJson(privateSettings);

	return RequestResult::Success(responseData);
}

// Intentionally undocumented
RequestResult RequestHandler::SetSourcePrivateSettings(const Request &request)
{
	RequestStatus::RequestStatus statusCode;
	std::string comment;
	OBSSourceAutoRelease source = request.AcquireSource("canvasUuid", "sourceName", "sourceUuid", statusCode, comment);
	if (!source || !request.ValidateObject("sourceSettings", statusCode, comment, true))
		return RequestResult::Error(statusCode, comment);

	OBSDataAutoRelease privateSettings = obs_source_get_private_settings(source);

	OBSDataAutoRelease newSettings = Utils::Json::JsonToObsData(request.RequestData["sourceSettings"]);

	// Always overlays to prevent destroying internal source data unintentionally
	obs_data_apply(privateSettings, newSettings);

	return RequestResult::Success();
}
