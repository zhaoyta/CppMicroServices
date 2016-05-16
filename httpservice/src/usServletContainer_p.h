/*=============================================================================

  Library: CppMicroServices

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

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

#ifndef USSERVLETCONTAINER_P_H
#define USSERVLETCONTAINER_P_H

#include <usServiceTracker.h>
#include <usServiceTrackerCustomizer.h>

class CivetServer;

namespace us {

class BundleContext;

class HttpServlet;
class ServletContainer;
class ServletContext;
class ServletHandler;

struct ServletContainerPrivate : private ServiceTrackerCustomizer<HttpServlet, ServletHandler>
{
  ServletContainerPrivate(ServletContainer* q);

  void Start();
  void Stop();

  std::string GetMimeType(const ServletContext* context, const std::string& file) const;

  BundleContext* m_Context;
  CivetServer* m_Server;
  ServiceTracker<HttpServlet, ServletHandler> m_ServletTracker;

  std::map<std::string, std::shared_ptr<ServletContext>> m_ServletContextMap;
  std::string m_ContextPath;

private:

  ServletContainer* const q;
  std::list<std::shared_ptr<ServletHandler>> m_Handler;

  virtual std::shared_ptr<ServletHandler> AddingService(const ServiceReference<HttpServlet>& reference);
  virtual void ModifiedService(const ServiceReference<HttpServlet>& /*reference*/, const std::shared_ptr<ServletHandler>& /*service*/);
  virtual void RemovedService(const ServiceReference<HttpServlet>& reference, const std::shared_ptr<ServletHandler>& handler);
};

}

#endif // USSERVLETCONTAINER_P_H
