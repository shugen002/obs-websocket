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

RequestResult RequestHandler::GetVirtualCamStatus(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::ToggleVirtualCam(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StartVirtualCam(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StopVirtualCam(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::GetReplayBufferStatus(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::ToggleReplayBuffer(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StartReplayBuffer(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StopReplayBuffer(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SaveReplayBuffer(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::GetLastReplayBufferReplay(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::GetOutputList(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::GetOutputStatus(const Request &request)
{
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!request.ValidateString("outputName", statusCode, comment))
return RequestResult::Error(statusCode, comment);

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::ToggleOutput(const Request &request)
{
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!request.ValidateString("outputName", statusCode, comment))
return RequestResult::Error(statusCode, comment);

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StartOutput(const Request &request)
{
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!request.ValidateString("outputName", statusCode, comment))
return RequestResult::Error(statusCode, comment);

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StopOutput(const Request &request)
{
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!request.ValidateString("outputName", statusCode, comment))
return RequestResult::Error(statusCode, comment);

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::GetOutputSettings(const Request &request)
{
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!request.ValidateString("outputName", statusCode, comment))
return RequestResult::Error(statusCode, comment);

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SetOutputSettings(const Request &request)
{
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!(request.ValidateString("outputName", statusCode, comment) &&
      request.ValidateObject("outputSettings", statusCode, comment, true)))
return RequestResult::Error(statusCode, comment);

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}
