#ifndef ROESYSTEM_H_
#define ROESYSTEM_H_

#include "RoeCommon.h"

#ifdef ROE_COMPILE_SYSTEM_SPECIFICS

#include "RoeSingleton.h"

namespace Roe {
namespace System {
	
	class DirParser {
	public:
		DirParser();
		~DirParser();
	};
	
} // namespace System
} // namespace Roe

#endif //ROE_COMPILE_SYSTEM_SPECIFICS
#endif /* ROESYSTEM_H_ */
