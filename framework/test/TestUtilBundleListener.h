/*=============================================================================

  Library: CppMicroServices

  Copyright (c) The CppMicroServices developers. See the COPYRIGHT
  file at the top-level directory of this distribution and at
  https://github.com/CppMicroServices/CppMicroServices/COPYRIGHT .

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

#ifndef CPPMICROSERVICES_TESTUTILBUNDLELISTENER_H
#define CPPMICROSERVICES_TESTUTILBUNDLELISTENER_H

#include "cppmicroservices/BundleEvent.h"
#include "cppmicroservices/ServiceEvent.h"
#include "cppmicroservices/BundleContext.h"

#include "TestingMacros.h"

namespace cppmicroservices {

class TestBundleListener {

public:

  TestBundleListener();

  void BundleChanged(const BundleEvent& event);

  void ServiceChanged(const ServiceEvent& event);

  BundleEvent GetBundleEvent() const;

  ServiceEvent GetServiceEvent() const;

  bool CheckListenerEvents(
      bool pexp, BundleEvent::Type ptype,
      bool sexp, ServiceEvent::Type stype,
      const Bundle& bundleX, ServiceReferenceU* servX);

  bool CheckListenerEvents(const std::vector<BundleEvent>& pEvts, bool relaxed = false);

  bool CheckListenerEvents(const std::vector<ServiceEvent>& seEvts);

  bool CheckListenerEvents(const std::vector<BundleEvent>& pEvts,
                           const std::vector<ServiceEvent>& seEvts,
                           bool relaxed = false);

private:

  std::vector<ServiceEvent> serviceEvents;
  std::vector<BundleEvent> bundleEvents;
};

template<class Receiver>
class BundleListenerRegistrationHelper
{

public:

  typedef void(Receiver::*CallbackType)(const BundleEvent&);

  BundleListenerRegistrationHelper(const BundleContext& context, Receiver* receiver, CallbackType callback)
    : context(context)
    , receiver(receiver)
    , callback(callback)
  {
    try
    {
      this->context.AddBundleListener(receiver, callback);
    }
    catch (const std::logic_error& ise)
    {
      US_TEST_OUTPUT( << "bundle listener registration failed " << ise.what() );
      throw;
    }
  }

  ~BundleListenerRegistrationHelper()
  {
    context.RemoveBundleListener(receiver, callback);
  }

private:

  BundleContext context;
  Receiver* receiver;
  CallbackType callback;
};

template<class Receiver>
class ServiceListenerRegistrationHelper
{

public:

  typedef void(Receiver::*CallbackType)(const ServiceEvent&);

  ServiceListenerRegistrationHelper(const BundleContext& context, Receiver* receiver, CallbackType callback)
    : context(context)
    , receiver(receiver)
    , callback(callback)
  {
    try
    {
      this->context.AddServiceListener(receiver, callback);
    }
    catch (const std::logic_error& ise)
    {
      US_TEST_OUTPUT( << "service listener registration failed " << ise.what() );
      throw;
    }
  }

  ~ServiceListenerRegistrationHelper()
  {
    context.RemoveServiceListener(receiver, callback);
  }

private:

  BundleContext context;
  Receiver* receiver;
  CallbackType callback;
};

}

#endif // CPPMICROSERVICES_TESTUTILBUNDLELISTENER_H
