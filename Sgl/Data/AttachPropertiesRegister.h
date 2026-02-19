#pragma once

#include <unordered_map>

namespace Sgl
{
	class BindableObject;

	template<typename T>
	class AttachPropertiesRegister
	{
	public:
		static AttachPropertiesRegister& Get()
		{
			static AttachPropertiesRegister instance;
			return instance;
		}

		T& GetAttachedProperty(const BindableObject* object)
		{
			return _register[object];
		}

		void ClearAttachedProperty(const BindableObject* object)
		{
			_register.erase(object);
		}
	private:
		AttachPropertiesRegister() = default;
	private:
		std::unordered_map<const BindableObject*, T> _register;
	};
}