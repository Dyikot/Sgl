#pragma once

#include "DataTemplate.h"
#include "../../Base/Delegate.h"

namespace Sgl
{
	template<typename T>
	class FuncDataTemplate: public IDataTemplate
	{
	private:
		Func<std::shared_ptr<UIElement>, T> _templateFactory;
	public:
		static std::shared_ptr<FuncDataTemplate<T>> New(Func<std::shared_ptr<UIElement>, T> templateFactory)
		{
			return std::make_shared<FuncDataTemplate<T>>(std::move(templateFactory));
		}

		FuncDataTemplate(Func<std::shared_ptr<UIElement>, T> templateFactory):
			_templateFactory(std::move(templateFactory))
		{}

		std::shared_ptr<UIElement> Build(const Any& data) const override
		{
			return _templateFactory(data.As<T>());
		}
	};
}