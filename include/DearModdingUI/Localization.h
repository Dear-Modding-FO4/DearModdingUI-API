#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <format>
#include <unordered_map>
#include <filesystem>

namespace dmui
{
	namespace localize
	{
		namespace detail
		{
			using namespace std::literals;

			constexpr static std::string WHITESPACEA = " \n\r\t\f\v";

			// Trim from the start (left trim)
			static void LeftTrim(std::string& s) noexcept
			{
				size_t start = s.find_first_not_of(WHITESPACEA);
				s.erase(0, start);
			}

			// Trim from the end (right trim)
			static void RightTrim(std::string& s) noexcept
			{
				size_t end = s.find_last_not_of(WHITESPACEA);
				if (end != std::string::npos)
					s.erase(end + 1);
			}

			// Trim from both ends
			static void Trim(std::string& s) noexcept
			{
				RightTrim(s);
				LeftTrim(s);
			}

			enum class Encoding : int8_t
			{
				Unknown = 0,
				UTF8_BOM,
				UTF16_LE,
				UTF16_BE,
				UTF32_LE,
				UTF32_BE
			};

			static Encoding CheckBom(const std::string& filename) noexcept
			{
				std::ifstream file(filename, std::ios::binary);
				if (!file)
					return Encoding::Unknown;

				// Read the first 4 bytes
				unsigned char bytes[4] = { 0 };
				file.read(reinterpret_cast<char*>(bytes), 4);
				std::streamsize bytes_read = file.gcount();

				if (bytes_read >= 3 && bytes[0] == 0xEF && bytes[1] == 0xBB && bytes[2] == 0xBF)
					return Encoding::UTF8_BOM;

				if (bytes_read >= 2 && bytes[0] == 0xFF && bytes[1] == 0xFE) {
					// Could be UTF-32 LE if followed by 00 00, but usually UTF-16 LE
					if (bytes_read == 4 && bytes[2] == 0x00 && bytes[3] == 0x00)
						return Encoding::UTF32_LE;
					return Encoding::UTF16_LE;
				}

				if (bytes_read >= 2 && bytes[0] == 0xFE && bytes[1] == 0xFF)
					return Encoding::UTF16_BE;

				if (bytes_read == 4 && bytes[0] == 0x00 && bytes[1] == 0x00 && bytes[2] == 0xFE && bytes[3] == 0xFF)
					return Encoding::UTF32_BE;

				return Encoding::Unknown;  // No BOM found
			}

			struct ILocalizeString
			{
				virtual void Load(void* a_data) = 0;
			};

			struct ILocalizeStore
			{
				virtual bool        Init(const std::string& a_file) noexcept = 0;
				virtual bool        Exists() const noexcept = 0;
				virtual void        Add(ILocalizeString* a_setting) noexcept = 0;
				virtual void        Load() = 0;
				virtual std::string GetFileName() const noexcept = 0;
			};

			class BaseLocalizeString :
				public ILocalizeString
			{
			protected:
				std::string value;
				std::string valueDefault;
			public:
				BaseLocalizeString() = delete;
				BaseLocalizeString(const std::string& a_default) noexcept :
					value(a_default),
					valueDefault(a_default)
				{}

				inline std::string GetValue() const noexcept { return value; }
				inline std::string GetValueDefault() const noexcept { return valueDefault; }
				inline void        SetValue(const std::string& a_value) noexcept { value = a_value; }

				inline operator std::string&() noexcept { return value; }
				inline operator const std::string&() const noexcept { return value; }
				inline operator char*() noexcept { return value.data(); }
				inline operator const char*() const noexcept { return value.c_str(); }
			};

			class LocalizeStore :
				public ILocalizeStore
			{
			protected:
				std::string                   file;
				std::string                   lang;
				std::vector<ILocalizeString*> localizes;
			public:
				bool Init(const std::string& a_file) noexcept override
				{
					file = a_file;
					return true;
				}

				bool Exists() const noexcept override { return file.length() ? std::filesystem::exists(file) : false; }

				void Add(ILocalizeString* a_localize) noexcept override
				{
					if (a_localize)
						localizes.emplace_back(a_localize);
				}

				std::string GetFileName() const noexcept override { return file; }
			};

			// Same as MCM translation files.
			// String parsing: $Weather40	Сильная облачность с дождем
			class LocalizationFileLoader
			{
				std::unordered_map<std::string, std::string> translations{};

				bool UTF8_LoadLanguageFile(std::ifstream& a_stm, const std::string& a_filePath,
					uintmax_t a_size, bool a_isBom)
				{
					try {
						if (a_isBom)
							a_stm.seekg(3, std::ios::beg);

						std::string line;
						while (std::getline(a_stm, line)) {
							Trim(line);

							std::size_t delimiterPos = line.find_first_of(" \t");
							if (delimiterPos != std::string::npos) {
								std::string key = line.substr(0, delimiterPos);
								std::string value = line.substr(delimiterPos + 1);

								Trim(key);
								Trim(value);

								translations.try_emplace(key, value);
							}
						}

						return true;
					} catch (const std::exception& e) {
						LatestError = std::format("An exception occurred while reading the file: \"{}\" message: \"{}\" "sv,
							a_filePath, e.what());
						return false;
					}
				}

				bool UTF16LE_LoadLanguageFile(std::ifstream& a_stm, const std::string& a_filePath,
					uintmax_t a_size, bool a_isBom)
				{
					try {
						if (a_isBom)
							a_stm.seekg(2, std::ios::beg);

						std::string u16;
						u16.resize(a_size);
						a_stm.read(u16.data(), a_size - 2);

						std::string u8, line;
						REX::UTF16_TO_UTF8(reinterpret_cast<const wchar_t*>(u16.c_str()), u8);

						std::stringstream sstm(u8);

						while (std::getline(sstm, line)) {
							Trim(line);

							std::size_t delimiterPos = line.find_first_of(" \t");
							if (delimiterPos != std::string::npos) {
								std::string key = line.substr(0, delimiterPos);
								std::string value = line.substr(delimiterPos + 1);

								Trim(key);
								Trim(value);

								translations.try_emplace(key, value);
							}
						}

						return true;
					} catch (const std::exception& e) {
						LatestError = std::format("An exception occurred while reading the file: \"{}\" message: \"{}\" "sv,
							a_filePath, e.what());
						return false;
					}
				}

				bool UTF16BE_LoadLanguageFile(std::ifstream& a_stm, const std::string& a_filePath,
					uintmax_t a_size, bool a_isBom)
				{
					try {
						if (a_isBom)
							a_stm.seekg(2, std::ios::beg);

						std::string u16;
						u16.resize(a_size);
						a_stm.read(u16.data(), a_size - 2);

						auto swap_bytes = [&](char* data, size_t byte_length) {
							for (size_t i = 0; i + 1 < byte_length; i += 2)
								std::swap(data[i], data[i + 1]);
						};

						swap_bytes(u16.data(), u16.size());

						std::string u8, line;
						REX::UTF16_TO_UTF8(reinterpret_cast<const wchar_t*>(u16.c_str()), u8);

						std::stringstream sstm(u8);

						while (std::getline(sstm, line)) {
							Trim(line);

							std::size_t delimiterPos = line.find_first_of(" \t");
							if (delimiterPos != std::string::npos) {
								std::string key = line.substr(0, delimiterPos);
								std::string value = line.substr(delimiterPos + 1);

								Trim(key);
								Trim(value);

								translations.try_emplace(key, value);
							}
						}

						return true;
					} catch (const std::exception& e) {
						LatestError = std::format("An exception occurred while reading the file: \"{}\" message: \"{}\" "sv,
							a_filePath, e.what());
						return false;
					}
				}

				std::string LatestError{};
			public:
				constexpr LocalizationFileLoader() = default;

				constexpr std::string LatestErrorMsg() const noexcept { return LatestError; }

				// Reads and parses the localization file line-by-line
				bool LoadLanguageFile(const std::string& a_filePath)
				{
					if (!std::filesystem::exists(a_filePath))
					{
						LatestError = std::format("No found localization file: {}"sv, a_filePath);
						return false;
					}

					auto fileSize = std::filesystem::file_size(a_filePath);
					if (fileSize <= 4) {
						LatestError = std::format("Incorrect file, too small size: {}"sv, a_filePath);
						return false;
					}

					auto encoding = CheckBom(a_filePath);
					if ((encoding == Encoding::UTF32_BE) || (encoding == Encoding::UTF32_LE)) {
						LatestError = std::format("The file contains a bom and the file encoding is not supported: {}"sv, a_filePath);
						return false;
					}

					bool isBom = encoding != Encoding::Unknown;

					std::ifstream file(a_filePath, std::ios::binary);
					if (!file.is_open()) {
						LatestError = std::format("Failed to open localization file: {}"sv, a_filePath);
						return false;
					}

					switch (encoding) {
						case Encoding::UTF16_LE:
							return UTF16LE_LoadLanguageFile(file, a_filePath, fileSize, isBom);
						case Encoding::UTF16_BE:
							return UTF16BE_LoadLanguageFile(file, a_filePath, fileSize, isBom);
						default:
							return UTF8_LoadLanguageFile(file, a_filePath, fileSize, isBom);
					}

					file.close();
					return true;
				}

				// Fetches the localized text
				bool Get(const std::string& a_key, const std::string& a_defValue, std::string& a_value) const noexcept
				{
					auto it = translations.find(a_key);
					auto result = it != translations.end();
					if (result)
						a_value = it->second;
					else
						a_value = a_defValue;
					return result;
				}
			};

			static void LocalizeLoad(void* a_data, const std::string& a_key,
				std::string& a_value, const std::string& a_valueDefault) noexcept
			{
				const auto data = static_cast<LocalizationFileLoader*>(a_data);
				data->Get(a_key, a_valueDefault, a_value);
			}
		}

		class LocalizationManager :
			public detail::LocalizeStore
		{
		protected:
			LocalizationManager() = default;
			~LocalizationManager() = default;

			LocalizationManager(const LocalizationManager&) = delete;
			LocalizationManager(LocalizationManager&&) = delete;

			LocalizationManager& operator=(const LocalizationManager&) = delete;
			LocalizationManager& operator=(LocalizationManager&&) = delete;
		public:
			static LocalizationManager* GetSingleton()
			{
				static LocalizationManager singleton;
				return std::addressof(singleton);
			}

			void Load() override
			{
				detail::LocalizationFileLoader loader;

				loader.LoadLanguageFile(file);
				for (auto& localize : localizes)
					localize->Load(std::addressof(loader));
			}
		};

		class LocalizeString :
			public detail::BaseLocalizeString
		{
			std::string key;
		public:
			LocalizeString(const std::string& a_key, const std::string& a_default) noexcept :
				BaseLocalizeString(a_default),
				key(a_key)
			{
				LocalizationManager::GetSingleton()->Add(this);
			}

			void Load(void* a_data) noexcept override
			{
				detail::LocalizeLoad(a_data, key, this->value, this->valueDefault);
			}
		};
	}
}
