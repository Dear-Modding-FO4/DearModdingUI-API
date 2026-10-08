#pragma once

#include <DearModdingUI/Client.h>

#include <memory>
#include <string>
#include <vector>

namespace dmui::localize
{
	class LocalizeString;

	class LocalizationManager
	{
	public:
		LocalizationManager(const LocalizationManager&) = delete;
		LocalizationManager(LocalizationManager&&) = delete;
		LocalizationManager& operator=(const LocalizationManager&) = delete;
		LocalizationManager& operator=(LocalizationManager&&) = delete;

		[[nodiscard]] static LocalizationManager* GetSingleton() noexcept
		{
			static LocalizationManager singleton;
			return std::addressof(singleton);
		}

		void Add(LocalizeString* a_localize) noexcept
		{
			if (!a_localize)
				return;
			try
			{
				localizes.push_back(a_localize);
			}
			catch (...)
			{
			}
		}

		void Remove(LocalizeString* a_localize) noexcept
		{
			std::erase(localizes, a_localize);
		}

		// Reads a_owner's translation file; call from onHostReady or later.
		void Load(std::string_view a_owner) noexcept;

	private:
		LocalizationManager() = default;

		std::vector<LocalizeString*> localizes;
	};

	// Keys from Interface\Translations\<owner>_<language>.txt, the MCM file format.
	class LocalizeString
	{
	public:
		LocalizeString(const std::string& a_key, const std::string& a_default) noexcept :
			key(a_key),
			value(a_default),
			valueDefault(a_default)
		{
			LocalizationManager::GetSingleton()->Add(this);
		}

		~LocalizeString() noexcept
		{
			LocalizationManager::GetSingleton()->Remove(this);
		}

		LocalizeString(const LocalizeString&) = delete;
		LocalizeString(LocalizeString&&) = delete;
		LocalizeString& operator=(const LocalizeString&) = delete;
		LocalizeString& operator=(LocalizeString&&) = delete;

		[[nodiscard]] std::string GetValue() const noexcept { return value; }
		[[nodiscard]] std::string GetValueDefault() const noexcept { return valueDefault; }
		void SetValue(const std::string& a_value) noexcept { value = a_value; }

		operator std::string&() noexcept { return value; }
		operator const std::string&() const noexcept { return value; }
		operator char*() noexcept { return value.data(); }
		operator const char*() const noexcept { return value.c_str(); }

		void Load(std::string_view a_owner) noexcept
		{
			try
			{
				value = Client::Localize(a_owner, key, valueDefault);
			}
			catch (...)
			{
			}
		}

	private:
		std::string key;
		std::string value;
		std::string valueDefault;
	};

	inline void LocalizationManager::Load(std::string_view a_owner) noexcept
	{
		for (auto* localize : localizes)
			localize->Load(a_owner);
	}
}
