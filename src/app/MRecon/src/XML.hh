/**
 * @file XML.hh
 *
 * @brief Implementation of timer
 *
 * @creation_date     { --.--.2021 }
 * @modification_date ( 25.05.2021 )
 * @modification_date ( 14.02.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */


#ifndef XML_H
#define XML_H


#include <memory>  // test features

// /*
struct XMLObject
{
	std::string Name;
	std::string Content;
	// std::unordered_map<std::string, std::string> Attributes;
	std::vector<std::pair<std::string, std::string>> Attributes;
	XMLObject* ParentXMLObj;
	std::vector<XMLObject*> ChildXMLObjects;
	int Level;

	/// with map of attributes ///
	// XMLObject(std::string name, std::string content, std::unordered_map<std::string, std::string>& attributes, XMLObject* parent)
	// : Name(name), Content(content), Attributes(attributes), ParentXMLObj(parent) {}

	// XMLObject(std::string name, std::string content, std::unordered_map<std::string, std::string>& attributes)
	// : Name(name), Content(content), Attributes(attributes), ParentXMLObj(nullptr) {}

	/// with vector of attributes pairs ///
	XMLObject(std::string name, std::string content, std::vector<std::pair<std::string, std::string>>& attributes, XMLObject* parent)
	: Name(name), Content(content), Attributes(attributes), ParentXMLObj(parent), Level(parent->Level + 1) { parent->appendChild(this); }

	XMLObject(std::string name, std::string content, std::vector<std::pair<std::string, std::string>>& attributes)
	: Name(name), Content(content), Attributes(attributes), ParentXMLObj(nullptr), Level(0) {}

	/// --- copy vector of attributes, not pass it by reference ///
	XMLObject(std::string name, std::string content, std::vector<std::pair<std::string, std::string>> attributes, XMLObject* parent)
	: Name(name), Content(content), Attributes(attributes), ParentXMLObj(parent), Level(parent->Level + 1) { parent->appendChild(this); }

	XMLObject(std::string name, std::string content, std::vector<std::pair<std::string, std::string>> attributes)
	: Name(name), Content(content), Attributes(attributes), ParentXMLObj(nullptr), Level(0) {}
	/// --- copy vector of attributes, not pass it by reference ///

	XMLObject(std::string name, std::string content, XMLObject* parent)
	: Name(name), Content(content), ParentXMLObj(parent), Level(parent->Level + 1) { parent->appendChild(this); }

	XMLObject(std::string name, std::string content)
	: Name(name), Content(content), ParentXMLObj(nullptr), Level(0) {}

	std::string to_str([[maybe_unused]] int with_new_line = 0, [[maybe_unused]] int depth_level = 0, const char* identination_symbol = "  ")
	{
		// const char identination_symbol = '\t';
		// const char identination_symbol = '  ';
		// const char* identination_symbol = "  ";
		std::string xml_obj_str = "\n" + Indentination(*identination_symbol) + "<" + Name;

		if (!Attributes.empty()) 
			for (auto& attr: Attributes)
				xml_obj_str += " " + attr.first + "=\"" + attr.second + "\"";

		// if (Name == "g")
		// 	xml_obj_str += ">\n" + Content;
		// else 
		// 	xml_obj_str += ">" + Content;
		// xml_obj_str = (Name == "g") ? xml_obj_str + ">\n" + Content : xml_obj_str + ">" + Content;
		// xml_obj_str += (Name == "g") ? ">\n" + Content : ">" + Content; /// default variant
		xml_obj_str += ">" + Content;

		if (!ChildXMLObjects.empty()) 
			for (auto& child: ChildXMLObjects)				
				// if (child and depth_level > 0) xml_obj_str += child->to_str(1, depth_level - 1);
				// else xml_obj_str += child->to_str();
				xml_obj_str += (child and depth_level > 0) ? child->to_str(1, depth_level - 1) : child->to_str();

		// if (Content.length() >= 20 || ChildXMLObjects.size())
		
		// if (ChildXMLObjects.size())
		// 	xml_obj_str += "\n" + Indentination(identination_symbol) + "</" + Name + ">";
		// else 
		// 	xml_obj_str += "</" + Name + ">";
		xml_obj_str += (ChildXMLObjects.size()) ?
							"\n" + Indentination(*identination_symbol) + "</" + Name + ">" :
							"</" + Name + ">";

		// if (with_new_line == 1) return xml_obj_str + "\n";
		return xml_obj_str;
	}

	void appendChild(XMLObject* child)
	{
		child->ParentXMLObj = this;
		ChildXMLObjects.push_back(child);
		child->Level += Level;
	}

	void addAttribute(std::string attrName, std::string attrValue)
	{
		// Attributes.insert({attrName, "\"" + attrValue + "\""});
		// Attributes.push_back({attrName, "\"" + attrValue + "\""});
		Attributes.push_back({attrName, attrValue});
	}

	void setContent(std::string content)
	{
		Content = content;
	}

	void setDeclaration()
	{

	}

	void addContent(std::string content)
	{
		Content += content;
	}

	XMLObject* GetParentObject()
	{
		return ParentXMLObj;
	}

	std::string Indentination(const char symb)
	{
		if      (symb == '\t') return std::string(Level    , symb);
		else if (symb == ' ' ) return std::string(Level * 2, symb);
		// else if (symb == ""  ) return "";
	}

	void clear()
	{
		for (auto& ptr: ChildXMLObjects)
		{
			// if (ptr != nullptr) // if xml object points to other xml object that may by deleted yet
				ptr->clear();
		}
		delete this;
	}

	/*
	void parseXMLstring(std::string xml_string)
	{
		bool isTagOpen = false;
		bool isTagContentOpen = false;
		std::string tag_definition = "";
		std::string tag_content = "";
		char prev_char = "";
		for (char& c : xml_string)
		{
			if (isTagOpen) tag_definition += c;
			if (isTagContentOpen && !isTagOpen) tag_content += c;

			if      (isTagOpen && c != "/" && prev_char == "<") isTagContentOpen = true;
			else if (isTagOpen && c == "/" && prev_char == "<") isTagContentOpen = false;

			if      (c == "<") isTagOpen = true;
			else if (c == ">") isTagOpen = false;

			prev_char = c;
		}
	}

	void parseXMLstringMin(std::string xml_string)
	{
		bool isTagOpen = false;
		char prev_char = "";
		std::string tag_string = "";
		for (char& c : xml_string)
		{
			/// fill tag content
			if (isTagOpen) tag_string += c;

			/// stop filling tag content
			if      (c == "<") isTagOpen = true;
			else if ((c == "/" && prev_char == "<") || (c == ">" && prev_char == "/"))
			{
				isTagOpen = false;
				parseXMLstringMin(tag_string);
				tag_string = "";
			}

			prev_char = c;
		}
	}

	void parseXMLstring2(std::string xml_string)
	{
		bool isTagDefinitionOpen = false;
		std::string tag_string = "";
		std::string tag_name = "";
		std::string tag_definition = "";
		char prev_char = "";
		for (char& c : xml_string)
		{
			if (isTagDefinitionOpen)
			{
				tag_definition += c;

				if (c != " ") tag_name += c;

				if (c == ">" && prev_char != "/") isTagDefinitionOpen = false;
			}

			if (c == "<") isTagDefinitionOpen = true;

			prev_char = c;
		}
	}
	// */

	// void find(std::string tag)
	// {

	// }
	

	/*
	XMLObject * parseXMLString(std::string xml_string, XMLObject * parent = nullptr)
	{
		bool isTagOpen = false;
		bool isName = false;
		char prev_char = "";
		std::string tag_name = "";
		XMLObject * xml_obj;
		for (char& c : xml_string)
		{
			if (isName && c == " ")
			{
				isName = false;
				xml_obj = new XMLObject(name, "");
			}

			if (isName) tag_name += c;

			if (c == "<") isTagOpen = true;
			if (prev_char == "<") isName = true;

			prev_char = c;
		}
	}
	// */
};

// */

#endif