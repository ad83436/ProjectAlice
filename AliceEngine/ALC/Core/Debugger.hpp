#ifndef _CORE_DEBUGGER_HPP
#define _CORE_DEBUGGER_HPP
#include "../General.hpp"

namespace ALC {

	class Debugger {
		ALC_NON_CONSTRUCTABLE(Debugger);
	public:

		static void Log(const string& msg, const string& file, size_t line);
		static void Trace(const string& msg, const string& file, size_t line);
		static void Warning(const string& msg, const string& file, size_t line);
		static void Error(const string& msg, const string& file, size_t line);
		static void FatalError(const string& msg, const string& file, size_t line);

	};

}

#define ALC_DEBUG_LOG(msg) \
	::ALC::Debugger::Log(msg, __FILE__, __LINE__)
#define ALC_DEBUG_TRACE(msg) \
	::ALC::Debugger::Trace(msg, __FILE__, __LINE__)
#define ALC_DEBUG_WARNING(msg) \
	::ALC::Debugger::Warning(msg, __FILE__, __LINE__)
#define ALC_DEBUG_ERROR(msg) \
	::ALC::Debugger::Error(msg, __FILE__, __LINE__)
#define ALC_DEBUG_FATAL_ERROR(msg) \
	::ALC::Debugger::FatalError(msg, __FILE__, __LINE__)

#endif // !_CORE_DEBUGGER_HPP