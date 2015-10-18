/*=============================================================================

  Library: CppMicroServices

  Copyright (c) The CppMicroServices developers. See the COPYRIGHT
  file at the top-level directory of this distribution and at
  https://github.com/saschazelzer/CppMicroServices/COPYRIGHT .

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#ifndef USLOG_P_H
#define USLOG_P_H

#include <usCoreConfig.h>
#include <usThreads_p.h>

#include <iostream>
#include <sstream>

namespace us {

US_Core_EXPORT void message_output(MsgType, const char* buf);

struct LogMsg {

  LogMsg(MsgType t, const char* file, int ln, const char* func)
    : type(t), enabled(true), buffer()
  { buffer << "In " << func << " at " << file << ":" << ln << " : "; }

  LogMsg()
    : type(DebugMsg), enabled(false), buffer()
  {}

  LogMsg(const LogMsg& other)
    : type(other.type), enabled(other.enabled), buffer()
  {}

  ~LogMsg() { if(enabled) message_output(type, buffer.str().c_str()); }

  template<typename T>
  LogMsg& operator<<(T t)
  {
    if (enabled) buffer << t;
    return *this;
  }

  LogMsg& operator()(bool flag)
  {
    this->enabled = flag;
    return *this;
  }

private:

  MsgType type;
  bool enabled;
  std::stringstream buffer;
};

class Logger : MultiThreaded<> {
public:
    static Logger& instance()
    { 
      /* 
      IMPORTANT: This is only thread safe if compiling with a C++11 compiler
        which implements C++11 standard section 6.7.4:
       "If control enters the declaration concurrently while the variable is being initialized, 
        the concurrent execution shall wait for completion of the initialization."

        However, even if a C++11 supported compiler isn't used, thread safe initialization
        of function local statics isn't a concern at the moment becasue this Logger
        is used internally by the Framework and not instantiated in a concurrent fashion.
       */
      static Logger inst;
      return inst;
    }

    /**
    * Set the logging level for log messages from CppMicroServices bundles.
    *
    * Higher logging levels will discard messages with lower priority.
    * E.g. a logging level of WarningMsg will discard all messages of
    * type DebugMsg and InfoMsg.
    *
    * @param level The new logging level.
    */
    void SetLogLevel(const MsgType level)
    {
      Lock l(this);
      logLevel = level;
    }

    /**
    * Get the current logging level.
    *
    * @return The currently used logging level.
    */
    MsgType GetLogLevel()
    {
      Lock l(this);
      return logLevel;
    }

protected:
    Logger(void) : logLevel(DebugMsg) {}
    ~Logger() {}

private:
    MsgType logLevel;

    // disable copy/assignment
    Logger(const Logger&);
    Logger& operator=(const Logger&);
};

}

#if defined(US_ENABLE_DEBUG_OUTPUT)
#define US_DEBUG (us::Logger::instance().GetLogLevel() > us::DebugMsg ? us::LogMsg() : us::LogMsg(us::DebugMsg, __FILE__, __LINE__, __FUNCTION__))
#else
  #define US_DEBUG us::LogMsg()
#endif

#if !defined(US_NO_INFO_OUTPUT)
#define US_INFO (us::Logger::instance().GetLogLevel() > us::InfoMsg ? us::LogMsg() : us::LogMsg(us::InfoMsg, __FILE__, __LINE__, __FUNCTION__))
#else
  #define US_INFO us::LogMsg()
#endif

#if !defined(US_NO_WARNING_OUTPUT)
#define US_WARN (us::Logger::instance().GetLogLevel() > us::WarningMsg ? us::LogMsg() : us::LogMsg(us::WarningMsg, __FILE__, __LINE__, __FUNCTION__))
#else
  #define US_WARN us::LogMsg()
#endif

#define US_ERROR us::LogMsg(us::ErrorMsg, __FILE__, __LINE__, __FUNCTION__)

#endif // USLOG_P_H