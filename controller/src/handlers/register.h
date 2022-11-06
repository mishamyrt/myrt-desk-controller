// Copyright 2022, Mikhael Khrustik <misha@myrt.co>
//
// All components of Myrt Desk Firmware are licensed under the BSD 3-Clause
// License. See LICENSE.txt for details.

#pragma once

#include <mServer.h>
#include <Height.h>

void registerDescribeHandler(mServer *server, String mac_address);
void registerLegsHandlers(mServer *server, HeightController *height);
