#include "StyleContext.h"

namespace Sgl
{
	Styleable::Styleable()
	{
		States.Changed += [this](VisualStateSet& sender, EventArgs e)
		{
			ApplyStateStyle();
		};
	}

	void Styleable::SetName(const std::string& value)
	{
		if(IsAttachedToLogicalTree())
		{
			throw Exception("Cannot change element name after it has been attached to the tree");
		}

		_name = value;
	}

	void Styleable::SetStyle(Ref<Style> style)
	{
		_style = std::move(style);
	}

	void Styleable::SetParent(Styleable* parent)
	{
		_stylingParent = parent;
	}

	void Styleable::OnAttachedToLogicalTree()
	{
		_isAttachedToLogicalTree = true;
		
		if(!_style)
		{
			_style = CreateDefaultStyle();
		}

		ApplyStyle();
		ApplyStateStyle();
		AttachedToLogicalTree.Invoke(*this);
	}

	void Styleable::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;
		DetachedFromLogicalTree.Invoke(*this);
	}

	Ref<Style> Styleable::CreateDefaultStyle() const
	{
		return nullptr;
	}

	void Styleable::ApplyStyle()
	{
		if(_style)
		{
			ValueSourceScope scope(*this, ValueSource::Style);
			StyleContext context(*this);
			_style->OnBase(context);
		}
	}

	void Styleable::ApplyStateStyle()
	{
		if(_style)
		{
			ClearAllValues();

			ValueSourceScope scope(*this, ValueSource::VisualState);
			StyleContext context(*this);
			_style->OnState(context, States);
		}
	}
}