// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <string>
#include <sstream>
#include <string>
#include <iomanip>

#include "../../lsMisc/GetVolumeInfo.h"
#include "../../lsMisc/stdosd/stdosd.h"
#include "../../lsMisc/FormatSizeof.h"
#include "../../lsMisc/CResizableDialog/ResizableDialog.h"
#include "../../profile/cpp/Profile/include/ambiesoft.profile.h"
#include "../../lsMisc/stdosd/stdosd.h"
#include "../../lsMisc/SaveWindowLocation.h"
#include "../../lsMisc/IsDuplicateInstance.h"

#define I18N(s) s
#endif //PCH_H
