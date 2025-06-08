#pragma once

#include <vector>

namespace Sgl
{		
	class IStyleable
	{
	public:
		virtual ~IStyleable() = default;

		virtual void ResetStyle() = 0;
	};

	class Trigger;

	class Style final
	{
	public:
		using StyleClass = void(*)(IStyleable&);
	private:
		IStyleable& _target;
		std::vector<StyleClass> _classes;
	public:
		Style(IStyleable& target):
			_target(target)
		{}

		Style(const Style& other):
			_target(other._target),
			_classes(other._classes)
		{}

		Style(Style&& other) noexcept:
			_target(other._target),
			_classes(std::move(other._classes))
		{}

		template<StyleClass... Classes>
		void Use()
		{
			_classes = { Classes... };
			Apply();
		}

		void Use(const Style& style)
		{
			_classes = style._classes;
			Apply();
		}
	private:
		void Apply()
		{
			for(StyleClass styleClass : _classes)
			{
				styleClass(_target);
			}
		}

		void ResetAndApply()
		{			
			_target.ResetStyle();
			Apply();
		}

		friend class Trigger;
	};
}