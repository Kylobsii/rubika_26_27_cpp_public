#include "Entity.h"

template<typename C>
requires std::is_base_of_v<IComponent, C>
inline C* Entity::AddComponent()
{
	C* comp = GetComponent<C>(false);

	if (comp)
	{
		Logger::Warning("Component of type: " + comp->GetName() + " was not added to: " + name + ", because this entity already has one!");
		return nullptr;
	}

	comp = new C(*this);
	components.push_back(comp);

	return comp;
}

template<typename C>
requires std::is_base_of_v<IComponent, C>
inline C* Entity::GetComponent(bool log) const
{
	for (auto component : components)
	{
		if (C* comp = dynamic_cast<C*>(component))
		{
			return comp;
		}
	}

	if (log)
	{
		Logger::Warning("Something tried to get the component of type: " + std::string(typeid(C).name()) + " of the Entity: " + name + ", but it doesn't not have one!");
	}

	return nullptr;
}