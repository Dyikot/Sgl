#include "RadioButton.h"

namespace Sgl::UIElements
{
	class RadioButtonGroup
	{
	public:
		RadioButtonGroup() = default;

		void Add(RadioButton* button)
		{
			_buttons.push_back(button);
		}

		void Remove(RadioButton* button)
		{
			std::erase(_buttons, button);
		}
		
		void NotifyCheckedChanged(RadioButton* changedButton)
		{
			for(auto button : _buttons)
			{
				if(button != changedButton)
				{
					button->Uncheck();
				}
			}
		}

		bool Empty() const noexcept
		{
			return _buttons.empty();
		}
	private:
		std::vector<RadioButton*> _buttons;
	};

	class RadioButtonGroupRegistry
	{
	public:
		static RadioButtonGroupRegistry& Instance()
		{
			static RadioButtonGroupRegistry registry;
			return registry;
		}

		void Add(RadioButton* button)
		{
			auto& groupName = button->GetGroupName();

			if(groupName.empty())
			{
				return;
			}

			_groups[groupName].Add(button);
		}

		void Remove(RadioButton* button)
		{
			auto& groupName = button->GetGroupName();

			if(auto it = _groups.find(groupName); it != _groups.end())
			{
				auto& group = it->second;
				group.Remove(button);

				if(group.Empty())
				{
					_groups.erase(groupName);
				}
			}
		}

		void NotifyCheckedChanged(RadioButton* button)
		{
			auto& groupName = button->GetGroupName();
			_groups[groupName].NotifyCheckedChanged(button);
		}
	private:
		std::unordered_map<std::string, RadioButtonGroup> _groups;
	};

	RadioButton::RadioButton()
	{
		static ImageSource source(AssetId::RadioButtonOutline);

		Name = "RadioButton";
		SetWidth(24, ValueSource::Default);
		SetHeight(24, ValueSource::Default);
		SetBackground(source, ValueSource::Default);
	}

	RadioButton::RadioButton(RadioButton&& other) noexcept:
		_groupName(std::move(other._groupName))
	{}

	void RadioButton::SetGroupName(const std::string& value, ValueSource source)
	{
		std::string oldValue = _groupName;

		if(SetProperty(GroupNameProperty, _groupName, value, _groupNameValueSource, source))
		{
			auto& registry = RadioButtonGroupRegistry::Instance();

			if(!oldValue.empty() && IsAttachedToLogicalTree())
			{
				registry.Remove(this);
			}

			if(!_groupName.empty() && IsAttachedToLogicalTree())
			{
				registry.Add(this);
			}
		}
	}

	void RadioButton::OnCheckedChanged()
	{
		if(IsChecked())
		{
			RadioButtonGroupRegistry::Instance().NotifyCheckedChanged(this);
		}
	}

	void RadioButton::OnClick(MouseClickEventArgs& e)
	{
		Button::OnClick(e);

		if(!IsChecked())
		{
			Check();
		}
	}

	void RadioButton::OnAttachedToLogicalTree()
	{
		ToggleButton::OnAttachedToLogicalTree();
		RadioButtonGroupRegistry::Instance().Add(this);
	}

	void RadioButton::OnDetachedFromLogicalTree()
	{
		ToggleButton::OnDetachedFromLogicalTree();
		RadioButtonGroupRegistry::Instance().Remove(this);
	}
}