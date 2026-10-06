#include "Styleable.h"
#include "../Base/Tools/StringUtils.h"
#include "../Base/Logging.h"

namespace Sgl
{
	Styleable::Styleable()
	{
		States.Changed += [this](VisualStateSet& sender, EventArgs e)
		{
			ApplyStateStyle();
		};
	}

	void Styleable::SetClasses(std::string_view classNames, char delimiter)
	{
		_classes = SplitString(classNames, delimiter);
		FetchAndApplyStyle();
	}

	void Styleable::SetClasses(std::vector<std::string> classList)
	{
		_classes = std::move(classList);
		FetchAndApplyStyle();
	}

	const std::vector<std::string>& Styleable::GetClasses() const
	{
		return _classes;
	}

	StyleCollection& Styleable::GetStyles()
	{
		return Styles;
	}

	void Styleable::SetParent(IStyleHost* parent)
	{
		_stylingParent = parent;
	}

	void Styleable::OnAttachedToLogicalTree()
	{
		_isAttachedToLogicalTree = true;
		FetchAndApplyStyle();
		AttachedToLogicalTree.Invoke(*this);
	}

	void Styleable::OnDetachedFromLogicalTree()
	{
		_isAttachedToLogicalTree = false;
		_style = {};
		DetachedFromLogicalTree.Invoke(*this);
	}

	void Styleable::FetchAndApplyStyle()
	{
		if(!IsAttachedToLogicalTree())
		{
			return;
		}

		_style = {};
				
		MergeStylesTo(*this, _style);
		ApplyStyle();
		ApplyStateStyle();
	}

	void Styleable::MergeStylesTo(Styleable& element, Style& target)
	{
		if(_stylingParent)
		{
			_stylingParent->MergeStylesTo(element, target);
		}

		Styles.MergeStylesTo(element, target);
	}

	void Styleable::ApplyStyle()
	{
		_style.Apply(*this);
	}

	void Styleable::ApplyStateStyle()
	{
		ClearAllValues();
		_style.ApplyStates(*this);
	}
}