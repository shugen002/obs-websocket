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

RequestResult RequestHandler::GetRecordStatus(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::ToggleRecord(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StartRecord(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::StopRecord(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::ToggleRecordPause(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::PauseRecord(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::ResumeRecord(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::SplitRecordFile(const Request &)
{
return RequestResult::Error(RequestStatus::UnsupportedFeature);
}

RequestResult RequestHandler::CreateRecordChapter(const Request &request)
{
if (request.Contains("chapterName")) {
RequestStatus::RequestStatus statusCode;
std::string comment;
if (!request.ValidateOptionalString("chapterName", statusCode, comment))
return RequestResult::Error(statusCode, comment);
}

return RequestResult::Error(RequestStatus::UnsupportedFeature);
}
