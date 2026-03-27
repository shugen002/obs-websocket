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

#include <string>
#include <vector>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "Platform.h"
#include "plugin-macros.generated.h"

std::string Utils::Platform::GetLocalAddress()
{
#ifdef _WIN32
	ULONG bufLen = 15000;
	std::vector<uint8_t> buf(bufLen);
	PIP_ADAPTER_ADDRESSES pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf.data());
	ULONG ret = GetAdaptersAddresses(AF_INET,
					 GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
					 NULL, pAddresses, &bufLen);
	if (ret == ERROR_BUFFER_OVERFLOW) {
		buf.resize(bufLen);
		pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf.data());
		ret = GetAdaptersAddresses(AF_INET,
					   GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
					   NULL, pAddresses, &bufLen);
	}
	if (ret != NO_ERROR)
		return "0.0.0.0";

	std::vector<std::pair<std::string, uint8_t>> preferredAddresses;
	for (PIP_ADAPTER_ADDRESSES pCurrent = pAddresses; pCurrent != nullptr; pCurrent = pCurrent->Next) {
		if (pCurrent->OperStatus != IfOperStatusUp)
			continue;
		for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrent->FirstUnicastAddress; pUnicast != nullptr;
		     pUnicast = pUnicast->Next) {
			if (pUnicast->Address.lpSockaddr->sa_family != AF_INET)
				continue;
			char addrBuf[INET_ADDRSTRLEN];
			sockaddr_in *sa = reinterpret_cast<sockaddr_in *>(pUnicast->Address.lpSockaddr);
			if (!inet_ntop(AF_INET, &sa->sin_addr, addrBuf, sizeof(addrBuf)))
				continue;
			std::string addr(addrBuf);
			if (addr == "127.0.0.1")
				continue;
			uint8_t priority = 255;
			if (addr.substr(0, 10) == "192.168.1." || addr.substr(0, 10) == "192.168.0.") {
				if (addr.substr(0, 11) == "192.168.56.")
					continue; // Ignore VirtualBox default network
				priority = 0;
			} else if (addr.substr(0, 7) == "172.16.") {
				priority = 1;
			} else if (addr.substr(0, 3) == "10.") {
				priority = 2;
			}
			preferredAddresses.emplace_back(addr, priority);
		}
	}
	if (preferredAddresses.empty())
		return "0.0.0.0";
	std::sort(preferredAddresses.begin(), preferredAddresses.end(),
		  [](const std::pair<std::string, uint8_t> &a, const std::pair<std::string, uint8_t> &b) {
			  return a.second < b.second;
		  });
	return preferredAddresses[0].first;
#else
	struct ifaddrs *ifaddr;
	if (getifaddrs(&ifaddr) == -1)
		return "0.0.0.0";

	std::vector<std::pair<std::string, uint8_t>> preferredAddresses;
	for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
		if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET)
			continue;
		char addrBuf[INET_ADDRSTRLEN];
		sockaddr_in *sa = reinterpret_cast<sockaddr_in *>(ifa->ifa_addr);
		if (!inet_ntop(AF_INET, &sa->sin_addr, addrBuf, sizeof(addrBuf)))
			continue;
		std::string addr(addrBuf);
		if (addr == "127.0.0.1")
			continue;
		uint8_t priority = 255;
		if (addr.substr(0, 10) == "192.168.1." || addr.substr(0, 10) == "192.168.0.") {
			if (addr.substr(0, 11) == "192.168.56.")
				continue; // Ignore VirtualBox default network
			priority = 0;
		} else if (addr.substr(0, 7) == "172.16.") {
			priority = 1;
		} else if (addr.substr(0, 3) == "10.") {
			priority = 2;
		}
		preferredAddresses.emplace_back(addr, priority);
	}
	freeifaddrs(ifaddr);

	if (preferredAddresses.empty())
		return "0.0.0.0";
	std::sort(preferredAddresses.begin(), preferredAddresses.end(),
		  [](const std::pair<std::string, uint8_t> &a, const std::pair<std::string, uint8_t> &b) {
			  return a.second < b.second;
		  });
	return preferredAddresses[0].first;
#endif
}

std::string Utils::Platform::GetCommandLineArgument(std::string arg)
{
	(void)arg;
	return "";
}

bool Utils::Platform::GetCommandLineFlagSet(std::string arg)
{
	(void)arg;
	return false;
}
