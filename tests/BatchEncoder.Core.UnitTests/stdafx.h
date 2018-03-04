// Copyright (c) Wiesław Šoltés. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#pragma once

#include "targetver.h"
#include "CppUnitTest.h"

#include "utilities\StringHelper.h"
#include "utilities\Utilities.h"

#include "worker\WorkerContext.h"
#include "worker\CommandLine.h"
#include "worker\OutputPath.h"
#include "worker\FileToPipeWriter.h"
#include "worker\PipeToFileWriter.h"
#include "worker\PipeToStringWriter.h"
#include "worker\OutputParser.h"
#include "worker\LuaOutputParser.h"
#include "worker\DebugOutputParser.h"
#include "worker\ToolUtilities.h"
#include "worker\Worker.h"

#include "TestOutputParser.h"
#include "TestWorkerContext.h"
