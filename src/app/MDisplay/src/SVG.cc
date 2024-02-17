
#include<iostream>
#include<algorithm>
#include<queue>
#include<stack>
#include<tuple>
#include "SVG.hh"


xmlns::XMLObject::XMLObject(std::string xml)
{
  parse(xml);
}

void xmlns::XMLObject::addChild(XMLObject* child, XMLObject* before_child, XMLObject* after_child)
{
  child->ParentXMLObj = this;
  child->setLevel(this->Level + 1);

  if (before_child)
  {
    auto it = std::find(ChildXMLObjects.begin(), ChildXMLObjects.end(), before_child);
    if (it == ChildXMLObjects.end()) { throw; }
    // auto index = std::distance(ChildXMLObjects.begin(), it);
    ChildXMLObjects.insert(it, child);
    return;
  }

  if (after_child)
  {
    auto it = std::find(ChildXMLObjects.begin(), ChildXMLObjects.end(), after_child);
    if (it == ChildXMLObjects.end()) { throw; }
    // auto index = std::distance(ChildXMLObjects.begin(), it);
    ChildXMLObjects.insert(it+1, child);
    return;
  }

  ChildXMLObjects.push_back(child);
}

void xmlns::XMLObject::removeChild(XMLObject* child)
{
  ChildXMLObjects.erase(std::remove(ChildXMLObjects.begin(), ChildXMLObjects.end(), child), ChildXMLObjects.end());
}

std::string xmlns::XMLObject::render(int tab_size, [[maybe_unused]] std::string indentation)
{
  std::string xml_tag = "";

  const char indentation_symbol = ' ';
  std::string ident = std::string(Level * tab_size, indentation_symbol);

  /// compose attributes
  std::string s_attrib = "";
  if (!Attributes.empty())
  {
    for (auto const& [attr, val] : Attributes)
    {
      s_attrib += " " + attr + "=\"" + val + "\"";
    }
  }

  switch (Type)
  {
    case XMLObjectType::File:
    {
      bool has_childs = !ChildXMLObjects.empty();
      for (auto& child: ChildXMLObjects)
      {
        xml_tag += "\n" + child->render(2);
      }
      break;
    }
    case XMLObjectType::Text:
    {
      xml_tag += Content;
      break;
    }
    case XMLObjectType::Header:
      xml_tag += "<?" + Name + s_attrib + "?>";
      break;
    case XMLObjectType::Doctype:
      xml_tag += "<!DOCTYPE " + Content + ">";
      break;
    case XMLObjectType::Tag:
    {
      // xml_tag += "\n" + ident + "<" + Name + s_attrib;
      xml_tag += ident + "<" + Name + s_attrib;

      bool has_childs = !ChildXMLObjects.empty();
      bool has_content = !Content.empty();
      // std::cout << has_childs << " " << has_content << std::endl;

      if ( !has_childs && !has_content )
      {
        xml_tag += "/>";
      }
      else if ( !has_childs && has_content )
      {
        xml_tag += ">" + Content + "</" + Name + ">";
      }
      else // if (has_childs)
      {
        xml_tag += ">" + Content;

        for (auto& child: ChildXMLObjects)
        {
          xml_tag += "\n" + child->render(2);
        }
        // xml_tag += "\n";
        xml_tag += "\n" + ident + "</" + Name + ">";
      }
      break;
    }
    default:
      break;
  }

  return xml_tag;
}

xmlns::XMLObject::tagParse xmlns::XMLObject::_parse_tag_definition(std::string tag_definition)
{
  tagParse tp;

  if (tag_definition.find_first_of("\t\n ") == std::string::npos)
  {
    if (_debug) std::cout << "--- Tag name: " << tag_definition << std::endl;
    tp.Name = tag_definition;
    return tp;
  }

  int first_space_idx = tag_definition.find_first_of(' ');
  std::string tag_name = tag_definition.substr(0, first_space_idx);
  if (_debug) std::cout << "--- Tag name: " << tag_name << std::endl;
  tp.Name = tag_name;
  
  std::string attributes_str = tag_definition.substr(first_space_idx+1, tag_definition.size());
  if (_debug) std::cout << "--- Tag attributes: " << attributes_str << std::endl;

  bool continue_precessing = true;
  std::string attributes_str_copy = attributes_str;
  // while (continue_precessing)
  while (attributes_str.size())
  {
    /// attribute = "some text here \"myabe in quotes\" fas"

    if (attributes_str_copy.size() == 0) break;

    /// find "=" symbol
    int attr_idx = attributes_str_copy.find_first_of('=');
    /// get first part before "="
    std::string attr_name = attributes_str_copy.substr(0, attr_idx);
    // std::cout << "   ::attr_name " << attr_name << std::endl;
    /// remove spaces if exist
    attr_name.erase(remove_if(attr_name.begin(), attr_name.end(), ::isspace), attr_name.end());
    /// get second part after "="
    std::string remaining = attributes_str_copy.substr(attr_idx+1, attributes_str_copy.size());
    // std::cout << "   ::remaining " << remaining << std::endl;
    /// find first quote '"' symbol
    int first_quote_idx = remaining.find_first_of('"');
    if (first_quote_idx == -1) break; // break if not found
    // std::cout << "   ::first_quote_idx " << first_quote_idx << std::endl;
    /// loop over remaining to find another quote '"' symbol; store previous character for handling masked quotes \" cases
    // std::cout << "   ::first_quote_idx " << remaining[first_quote_idx] << " " << remaining.substr(first_quote_idx) << std::endl;
    char prev_symbol = remaining[first_quote_idx];
    int last_quote_idx = -1;
    for (int i = first_quote_idx+1; i < remaining.size(); ++i)
    {
      char s = remaining[i];
      if (s == '"' && prev_symbol != '\\')
      {
        last_quote_idx = i;
        break;
      }
    }
    if (_debug)
    {
      std::cout << "   ::last_quote_idx " << last_quote_idx << std::endl;
      std::cout << "   ::last_quote_idx " << remaining.substr(1, last_quote_idx-1) << std::endl;
      std::cout << "   ::last_quote_idx " << remaining.substr(last_quote_idx+1) << std::endl;
    }
    
    /// get attribute definition
    std::string attr_def = remaining.substr(1, last_quote_idx-1);
    tp.attributes.insert({attr_name, attr_def});
    if (_debug) std::cout << "Attribute: " << attr_name << " -- " << attr_def << std::endl;
    attributes_str_copy = attributes_str_copy.substr(attr_idx+1+last_quote_idx+1);
    if (_debug) std::cout << "   ::attributes_str_copy " << attributes_str_copy << std::endl;
  }

  return tp;
}

xmlns::XMLObject* xmlns::XMLObject::parse(std::string xml)
{ // TODO: add error check
  std::cout << "\n" << xml << std::endl;
  std::string prev_char = "";
  bool is_tag_open = false;
  bool is_tag_content_open = false;
  bool is_attribute_open = false;

  std::vector<std::tuple<std::string, int>> xmlContentSplitByTags;
  /// int -- type of item
  /// 0 -- other content (between tags)
  /// 1 -- opening tag <tag>
  /// 2 -- closing tag </tag>
  /// 3 -- open/close tag <tag />
  /// 4 -- header tag <? ?>
  /// 5 -- doctype <!DOCTYPE >

  std::string tag_content = "";
  std::string other_content = "";
  for (auto ch: xml)
  {
    if (ch == '<' && !is_tag_content_open)
    {
      if (!other_content.empty())
      {
        bool whiteSpacesOnly = std::all_of(other_content.begin(),other_content.end(),isspace);
        if (!whiteSpacesOnly)
          xmlContentSplitByTags.push_back({other_content, 0});
        other_content = "";
      }
      is_tag_content_open = true;
      continue;
    }
    // else if (ch == "<" && is_tag_content_open) throw;
    else if (ch == '>' && is_tag_content_open)
    {
      is_tag_content_open = false;

      int tag_type = -1;
      if (*tag_content.begin() == '?' && tag_content.back() == '?')
      {
        tag_type = 4;
      }
      else if (tag_content.back() == '/')
      {
        tag_type = 3;
      }
      else if (*tag_content.begin() == '/')
      {
        tag_type = 2;
      }
      else if (*tag_content.begin() == '!')
      {
        tag_type = 5;
      }
      else
      {
        tag_type = 1;
      }

      tag_content = '<' + tag_content + '>';
      xmlContentSplitByTags.push_back({tag_content, tag_type});
      tag_content = "";
      continue;
    }

    if (is_tag_content_open)
    {
      tag_content += ch;
    }
    else
    {
      other_content += ch;
    }

    prev_char = ch;
  }

  XMLObject* main_xml = new XMLObject();

  // std::queue<XMLObject*> open_tags; // fifo
  std::stack<XMLObject*> open_tags; // lifo
  open_tags.push(main_xml);
  std::vector<XMLObject*> open_tags_vs;
  open_tags_vs.push_back(main_xml);

  for (auto& item: xmlContentSplitByTags)
  {
    if (_debug) std::cout << std::get<0>(item) << " " << std::get<1>(item) << std::endl;

    int item_type = std::get<1>(item);

    std::string contents = std::get<0>(item);
    std::string contents_wo_tag_symbols = "";

    if (_debug){
      std::cout << "  ::Stack:top: " << open_tags.top()->getName() << " :size: " << open_tags.size() << std::endl;
      std::cout << "  ::Stack:top: " << open_tags_vs.back()->getName() << " :size: " << open_tags_vs.size() << " == [";// << std::endl;
      std::cout << "=== ";
      for (auto it: open_tags_vs)
      {
        std::cout << it->getName() << " ";
      }
      std::cout << "]" << std::endl;

      std::cout << "///============================================" << std::endl;
      std::cout << main_xml->getChilds().size() << std::endl;
      std::cout << main_xml->render(2) << std::endl;
      std::cout << "///============================================" << std::endl;
    }
    
    switch(item_type)
    {
      /// 0 -- other content (between tags)
      /// 1 -- opening tag <tag>
      /// 2 -- closing tag </tag>
      /// 3 -- open/close tag <tag />
      /// 4 -- header tag <? ?>
      /// 5 -- doctype <!DOCTYPE >
      case 0:
      {
        contents_wo_tag_symbols = contents;
        XMLObject* xml = new XMLObject(nullptr, XMLObjectType::Text, "", contents_wo_tag_symbols);
        XMLObject* last_open_xml = open_tags.top();
        // XMLObject* last_open_xml = open_tags_vs.back();
         if (_debug) std::cout << ">>> case 0: last_open_xml: " << last_open_xml->getName() << " " << last_open_xml->getLevel() << std::endl;
        last_open_xml->addChild(xml);
        break;
      }
      case 1: // open
      {
        contents_wo_tag_symbols = contents.substr(1, contents.size()-1-1);
        tagParse tp = _parse_tag_definition(contents_wo_tag_symbols);
        if (!open_tags.empty())
        // if (!open_tags_vs.empty())
        {
          XMLObject* last_open_xml = open_tags.top();
          // XMLObject* last_open_xml = open_tags_vs.back();
          XMLObject* xml = new XMLObject(last_open_xml, XMLObjectType::Tag, tp.Name, "", tp.attributes);
           if (_debug) std::cout << ">>> case 1: last_open_xml: " << last_open_xml->getName() << " " << last_open_xml->getLevel() << std::endl;
          open_tags.push(xml);
          // open_tags_vs.push_back(xml);
        } else {
          XMLObject* xml = new XMLObject(XMLObjectType::Tag, tp.Name, "", tp.attributes);
          open_tags.push(xml);
          // open_tags_vs.push_back(xml);
        }
        break;
      }
      case 2: // close
      {
        contents_wo_tag_symbols = contents.substr(2, contents.size()-1-2);
        tagParse tp = _parse_tag_definition(contents_wo_tag_symbols);
        XMLObject* last_open_xml = open_tags.top();
        // XMLObject* last_open_xml = open_tags_vs.back();
         if (_debug) std::cout << ">>> case 2: last_open_xml: " << last_open_xml->getName() << " " << last_open_xml->getLevel() << std::endl;
         if (_debug) std::cout << "===::Stack:close: " << last_open_xml->getName() << " " << tp.Name << std::endl;
        if (last_open_xml->getName() == tp.Name)
        {
          // open_tags_vs.pop_back();
          open_tags.pop();
        } else throw;
        break;
      }
      case 3: // open + close
      {
        contents_wo_tag_symbols = contents.substr(1, contents.size()-2-1);
        tagParse tp = _parse_tag_definition(contents_wo_tag_symbols);
        XMLObject* xml = new XMLObject(XMLObjectType::Tag, tp.Name, "", tp.attributes);
        XMLObject* last_open_xml = open_tags.top();
        // XMLObject* last_open_xml = open_tags_vs.back();
         if (_debug) std::cout << ">>> case 3: last_open_xml: " << last_open_xml->getName() << " " << last_open_xml->getLevel() << std::endl;
        last_open_xml->addChild(xml);
        break;
      }
      case 4: // open + close : header
      {
        contents_wo_tag_symbols = contents.substr(2, contents.size()-2-2);
        tagParse tp = _parse_tag_definition(contents_wo_tag_symbols);
        XMLObject* xml = new XMLObject(XMLObjectType::Header, tp.Name, "", tp.attributes);
        XMLObject* last_open_xml = open_tags.top();
        // XMLObject* last_open_xml = open_tags_vs.back();
         if (_debug) std::cout << ">>> case 4: last_open_xml: " << last_open_xml->getName() << " " << last_open_xml->getLevel() << std::endl;
        last_open_xml->addChild(xml);
        break;
      }
      case 5: // open + close : DOCTYPE // now it is incorrect
      {
        contents_wo_tag_symbols = contents.substr(2, contents.size()-1-2);
        tagParse tp = _parse_tag_definition(contents_wo_tag_symbols);
        XMLObject* xml = new XMLObject(XMLObjectType::Doctype, tp.Name, "");
        XMLObject* last_open_xml = open_tags.top();
        // XMLObject* last_open_xml = open_tags_vs.back();
         if (_debug) std::cout << ">>> case 5: last_open_xml: " << last_open_xml->getName() << " " << last_open_xml->getLevel() << std::endl;
        last_open_xml->addChild(xml);
        break;
      }
    }

    if (_debug) std::cout << std::get<0>(item) << " " << std::get<1>(item) << " " << contents_wo_tag_symbols << std::endl;
  }

  open_tags.pop();

  if (_debug){
    std::cout << "============================================" << std::endl;
    std::cout << main_xml->render(2) << std::endl;
    std::cout << "============================================" << std::endl;
   }

  return main_xml;
}