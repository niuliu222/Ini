#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
namespace LLD {
	namespace Ini {

		class IniProperty {
		public:
			std::string key;
			std::string value;
			static std::shared_ptr<IniProperty> ParseIniProperty(const std::string & str);

			bool   ValueToBool()   { return value == "true"; }
			double ValueToDouble() { return atof(value.c_str()); }
			int    ValueToInt()    { return atoi(value.c_str()); }

			static bool CheckIniPropertSyntax(const std::string & str);

			friend std::ostream & operator<<(std::ostream & os, const IniProperty & property);
		};

		class IniGroup {
			std::string GroupName;
			std::vector<std::shared_ptr<IniProperty>> Properties;
		public:
			void SetGroupName(const std::string &name) { this->GroupName = name; }
			const std::string& GetGroupName()const { return GroupName; }

			void AddProperty(std::shared_ptr<IniProperty> & property) { Properties.push_back(property); }

			std::shared_ptr<IniProperty> GetPropertyByName(const std::string &name);

			bool                GetBool  (const std::string & key) { return GetPropertyByName(key)->ValueToBool(); }
			int                 GetInt   (const std::string & key) { return GetPropertyByName(key)->ValueToInt(); }
			double              GetDouble(const std::string & key) { return GetPropertyByName(key)->ValueToDouble(); }
			const std::string & GetString(const std::string & key) { return GetPropertyByName(key)->value; }

			void SetBool  (const std::string & key, bool b)               { GetPropertyByName(key)->value = b ? "true" : "false"; }
			void SetInt   (const std::string & key, int b)                { GetPropertyByName(key)->value = b; }
			void SetFloat (const std::string & key, double b)             { GetPropertyByName(key)->value = b; }
			void SetString(const std::string & key, const std::string& b) { GetPropertyByName(key)->value = b; }
			
			static std::shared_ptr<IniGroup> ParseIniGroup(const std::string & str);

			static bool CheckIniGroupSyntax(const std::string & str) {return str.at(0) == '[' && str.at(str.length() - 1) == ']';}

			friend std::ostream & operator<<(std::ostream & os, const IniGroup & group);
		};
		class IniFile {
		private:
			std::string url;
			std::vector<std::shared_ptr<IniGroup>> Groups;
			std::string defaultGroupName = "default";
		public:
			IniFile(){}

			IniFile(const std::string& url) {Open(url);}

			void Open(const std::string& url);

			std::string ReadAllData(const std::string& url);

			void ParseLine(const std::string str);

			std::string&   replace_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value);

			void MinTrim(std::string&  str);

			void ParseAllData(const std::string data);

			std::shared_ptr<IniGroup> GetGroupByName(const std::string &name) {
				auto it = Groups.begin();
				for (; it != Groups.end(); ++it) {
					if ((*it)->GetGroupName() == name) {
						return (*it);
					}
				}
			}

			bool               GetBool  (const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				return GetGroupByName(group)->GetBool(key);
			}
			int                GetInt   (const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				return GetGroupByName(group)->GetInt(key);
			}
			bool               GetDouble(const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				return GetGroupByName(group)->GetDouble(key);
			}
			const std::string& GetString(const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				return GetGroupByName(group)->GetString(key);
			}

			void SetBool  (bool                value, const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				GetGroupByName(group)->SetBool(key, value);
			}
			void SetInt   (int                 value, const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				GetGroupByName(group)->SetInt(key, value);
			}
			void SetFloat (double              value, const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				GetGroupByName(group)->SetFloat(key, value);
			}
			void SetString(const std::string & value, const std::string& key = "", std::string group = "") {
				if (group.length() == 0) {
					group = this->defaultGroupName;
				}
				GetGroupByName(group)->SetString(key, value);
			}

			friend std::ostream & operator<<(std::ostream & os, const IniFile & inifile) {
				auto it = inifile.Groups.cbegin();
				for (; it != inifile.Groups.cend(); ++it) {
					os << *(*it);
				}
				return os;
			}

		};


		std::shared_ptr<IniProperty> IniProperty::ParseIniProperty(const std::string & str) {
			int index = str.find('=');
			std::shared_ptr<IniProperty> property(new IniProperty());
			property->key = str.substr(0, index);

			int lenth = str.length() - 1;
			if (str.at(str.length() - 1) == ';') {
				--lenth;
			}
			lenth -= index;
			property->value = str.substr(index + 1, lenth);

			return property;
		}

		bool IniProperty::CheckIniPropertSyntax(const std::string & str) {
			int index = str.find('=');
			if (index < 0) {
				return false;
			}
			else if (index == str.length()) {
				return false;
			}
			else if (index == 0) {
				return false;
			}
			else {
				return true;
			}
		}
		std::ostream & operator<<(std::ostream & os, const IniProperty & property) {
			os << property.key << "=" << property.value << ";" << std::endl;
			return os;
		}
		std::shared_ptr<IniProperty> IniGroup::GetPropertyByName(const std::string &name) {
			auto it = Properties.begin();
			for (; it != Properties.end(); ++it) {
				if ((*it)->key == name) {
					return (*it);
				}
			}
		}
		std::shared_ptr<IniGroup> IniGroup::ParseIniGroup(const std::string & str) {
			std::shared_ptr<IniGroup> group(new IniGroup());
			int lenth = str.length() - 2;
			group->GroupName = str.substr(1, lenth);
			return group;
		}

		std::ostream & operator<<(std::ostream & os, const IniGroup & group) {
			os << "[" << group.GetGroupName() << "]" << std::endl;
			auto it = group.Properties.cbegin();
			for (; it != group.Properties.cend(); ++it) {
				os << *(*it);
			}
			os << std::endl;
			return os;
		}

		void IniFile::Open(const std::string& url) {
			this->url = url;
			auto all_data = ReadAllData(url);
			ParseAllData(all_data);
		}

		std::string IniFile::ReadAllData(const std::string& url) {
			std::ifstream ifile(url);
			std::stringstream buf;
			buf << ifile.rdbuf();
			return buf.str();
		}

		void IniFile::ParseLine(const std::string str) {
			static std::shared_ptr<IniGroup> CurrentGroup;
			if (IniGroup::CheckIniGroupSyntax(str)) {
				CurrentGroup = IniGroup::ParseIniGroup(str);
				Groups.push_back(CurrentGroup);
			}
			else if (IniProperty::CheckIniPropertSyntax(str)) {
				if (Groups.size() == 0) {
					CurrentGroup = std::make_shared<IniGroup>();
					CurrentGroup->SetGroupName(defaultGroupName);
					Groups.push_back(CurrentGroup);
				}
				auto property = IniProperty::ParseIniProperty(str);
				CurrentGroup->AddProperty(property);
			}
		}

		std::string&   IniFile::replace_all(std::string&   str, const   std::string&   old_value, const   std::string&   new_value)
		{
			while (true) {
				std::string::size_type   pos(0);
				if ((pos = str.find(old_value)) != std::string::npos)
					str.replace(pos, old_value.length(), new_value);
				else   break;
			}
			return   str;
		}
		void IniFile::MinTrim(std::string&  str) {
			replace_all(str, " ", "");
			replace_all(str, "\n", "");
			replace_all(str, "\t", "");
		}

		void IniFile::ParseAllData(const std::string data) {
			int index = data.find('\n');
			int curr_index = 0;
			while (index > -1) {
				auto str = data.substr(curr_index, index - curr_index);

				MinTrim(str);
				if (str.length()) {
					ParseLine(str);
				}

				curr_index = index + 1;
				index = data.find('\n', curr_index);
			}

			if (curr_index < data.length()) {
				auto str = data.substr(curr_index);
				ParseLine(str);
			}

		}
	}
}
