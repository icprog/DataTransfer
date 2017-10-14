﻿/*=============================================================================

  Library: CTK

  Copyright (c) DKFZ

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


#include "ServiceEventListenerActivator_p.h"
#include <QtPlugin>
#include "ServiceImp.h"
ServiceEventListenerActivator* ServiceEventListenerActivator::instance = 0;

ServiceEventListenerActivator::ServiceEventListenerActivator()
    : context(0)
{
}

ServiceEventListenerActivator::~ServiceEventListenerActivator()
{

}

void ServiceEventListenerActivator::start(ctkPluginContext* context)
{
    pInterface = new ServiceImp();
    context->registerService(QStringList("ServiceInterface"), pInterface);
    instance = this;
    this->context = context;
}

void ServiceEventListenerActivator::stop(ctkPluginContext* context)
{
    if (NULL != pInterface) {
        delete pInterface;
        pInterface = NULL;
    }
    Q_UNUSED(context)
}

ServiceEventListenerActivator* ServiceEventListenerActivator::getInstance()
{
    return instance;
}

ctkPluginContext* ServiceEventListenerActivator::getPluginContext() const
{
    return context;
}

Q_EXPORT_PLUGIN2(org_mydomain_serviceeventlistener, ServiceEventListenerActivator)


