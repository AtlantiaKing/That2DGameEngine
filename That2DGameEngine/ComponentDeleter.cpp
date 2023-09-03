#include "ComponentDeleter.h"
#include "Component.h"

void that::ComponentDeleter::operator()(that::Component* p) const
{
    delete p;
}
