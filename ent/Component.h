#pragma once

namespace Hx
{
	struct ComponentBase
	{
		static unsigned int idCounter;
	};

	template<class T>
	struct Component : protected ComponentBase
	{
		static unsigned int id;
	};

	template<class T>
	unsigned int Component<T>::id = Component::idCounter++;
}