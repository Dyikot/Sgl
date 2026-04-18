#include "RadioButton.h"
#include <SDL3/SDL_filesystem.h>

namespace Sgl::UIElements
{
	class RadioButtonGrouopRegistry
	{
	public:
		void Add(const std::string& groupName, RadioButton* button)
		{
			if(groupName.empty())
			{
				return;
			}

			_groups[groupName].push_back(button);
		}

		void Remove(const std::string& groupName, RadioButton* button)
		{
			if(auto it = _groups.find(groupName); it != _groups.end())
			{
				auto& group = it->second;
				std::erase(group, button);

				if(group.size() == 0)
				{
					_groups.erase(groupName);
				}
			}
		}

		void NotifyGroup(const std::string& groupName, RadioButton* button)
		{
			auto& group = _groups[groupName];
			for(auto item : group)
			{
				if(item != button)
				{
					item->Uncheck();
				}
			}
		}
	private:
		std::unordered_map<std::string, std::vector<RadioButton*>> _groups;
	};

	static RadioButtonGrouopRegistry& GetRadioButtonRegistry()
	{
		static RadioButtonGrouopRegistry registry;
		return registry;
	}

	RadioButton::RadioButton()
	{
		static ImagePath imagePath = ImagePath(SDL_GetBasePath(), "Assets/Images/RadioButtonOutline.png");

		SetWidth(24, ValueSource::Default);
		SetHeight(24, ValueSource::Default);
		SetBackground(imagePath, ValueSource::Default);
	}

	RadioButton::RadioButton(RadioButton&& other) noexcept:
		_groupName(std::move(other._groupName))
	{}

	void RadioButton::SetGroupName(const std::string & value, ValueSource source)
	{
		std::string oldValue = _groupName;

		if(SetProperty(GroupNameProperty, _groupName, value, _groupNameValueSource, source))
		{
			auto& registry = GetRadioButtonRegistry();

			if(!oldValue.empty())
			{
				registry.Remove(_groupName, this);
			}

			if(!_groupName.empty())
			{
				registry.Add(_groupName, this);
			}
		}
	}

	void RadioButton::OnCheckedChanged()
	{
		if(IsChecked())
		{
			auto& registry = GetRadioButtonRegistry();
			registry.NotifyGroup(_groupName, this);			
		}
	}

	void RadioButton::OnClick()
	{
		Button::OnClick();

		if(!IsChecked())
		{
			Check();
		}
	}

	void RadioButton::OnDetachedFromLogicalTree()
	{
		ToggleButton::OnDetachedFromLogicalTree();

		auto& registry = GetRadioButtonRegistry();
		registry.Remove(_groupName, this);
	}
}