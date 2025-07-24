#pragma once

#include "DataTemplate.h"
#include "../../Base/Delegate.h"

namespace Sgl
{
	template<typename T>
	class FuncDataTemplate: public IDataTemplate
	{
	private:
		Func<Shared<UIElement>, T> _templateFactory;
	public:
		FuncDataTemplate(Func<Shared<UIElement>, T> templateFactory):
			_templateFactory(std::move(templateFactory))
		{}

		Shared<UIElement> Build(const Any& data) const override
		{
			return _templateFactory(data.As<T>());
		}
	};
}