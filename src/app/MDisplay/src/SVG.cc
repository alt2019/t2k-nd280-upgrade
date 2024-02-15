
#include<algorithm>
#include "SVG.hh"

// void xmlns::XMLObject::_initialize()
// {
//   Type = XMLObjectType::Tag;
//   Name = "";
//   Content = "";
//   ParentXMLObj = nullptr;
//   Level = 0;
//   Indent = ' ';
// }

// void xmlns::XMLObject::_reset() // TODO: implement
// {
//   Name = "";
//   Content = "";
//   ParentXMLObj = nullptr;
//   Level = 0;
// }

/*
xmlns::XMLObject::XMLObject(std::string name, std::string contents)
{
  _initialize();
  Name = name;
  Content = contents;
}

xmlns::XMLObject::XMLObject(std::string name, std::string contents, XMLAttributes& attributes)
{
  _initialize();
  Name = name;
  Content = contents;
  Attributes = attributes;
}

xmlns::XMLObject::XMLObject(XMLObjectType type, std::string name, std::string contents, std::string indentation)
{
  _initialize();
  Type = type;
  Name = name;
  Content = contents;
  Indent = indentation;
}

xmlns::XMLObject::XMLObject(
  xmlns::XMLObject* parent,
  std::string name,
  std::string contents,
  [[maybe_unused]] std::string identination,
  XMLAttributes& attributes)
{
  _initialize();
  Name = name;
  Content = contents;
  Attributes = attributes;
  ParentXMLObj = parent;
  _setLevel();
}
// */


// xmlns::XMLObject::XMLObject(
//   XMLObject* parent,
//   XMLObjectType type,
//   std::string name,
//   std::string contents,
//   std::string indentation,
//   XMLAttributes& attributes)
// {
//   _initialize();
//   Type = type;
//   Name = name;
//   Content = contents;
//   Attributes = attributes;
//   ParentXMLObj = parent;
//   _setLevel();
//   Indent = indentation;
// }

// xmlns::XMLObject::XMLObject(
//   XMLObject* parent,
//   XMLObjectType type,
//   std::string name,
//   std::string contents,
//   const XMLAttributes& attributes)
// {
//   _initialize();
//   Type = type;
//   Name = name;
//   Content = contents;
//   Attributes = attributes;
//   ParentXMLObj = parent;
//   _setLevel();
//   // Indent = indentation;
// }


void xmlns::XMLObject::addChild(XMLObject* child, XMLObject* before_child, XMLObject* after_child)
{
  child->ParentXMLObj = this;

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

std::string xmlns::XMLObject::render(int tab_size)
{
  /// compose attributes
  std::string s_attrib = "";
  if (!Attributes.empty())
  {
    for (auto const& [attr, val] : Attributes)
    {
      s_attrib += " " + attr + "=\"" + val + "\"";
    }
  }



  return "";
}

std::string xmlns::XMLObject::render0(int tab_size) // tab symbol 
{
  // std::string ident = "";
  const char indentation_symbol = ' ';
  std::string ident = std::string(Level * tab_size, indentation_symbol);
  // if      (indentation_symbol == '\t') ident = std::string(Level    , indentation_symbol);
	// else if (indentation_symbol == ' ' ) ident = std::string(Level * 2, indentation_symbol);

  std::string s_attrib = "";
  if (!Attributes.empty())
  {
    for (auto const& [attr, val] : Attributes)
    {
      s_attrib += " " + attr + "=\"" + val + "\"";
    }
  }

  std::string xml_tag = "";

  switch (Type)
  {
    case XMLObjectType::Header:
      xml_tag += "<?" + Name + s_attrib + "?>";
      break;
    case XMLObjectType::Tag:
      xml_tag = ident + "<" + Name + s_attrib;
      break;
    default:
    break;
  }

  // std::string xml_tag = ident + "<" + Name + s_attrib;











  // if (ChildXMLObjects.empty() && Content.empty())
  // {
  //   xml_tag += "/>";
  //   return xml_tag;
  // }

  // if (!Content.empty()) xml_tag += ">" + Content;

  // if (ChildXMLObjects.empty() && Content.empty())
  // {
  //   xml_tag += "/>";
  // }
  // else if (!Content.empty())
  // {
  //   xml_tag += ">" + Content;
  // }

  // if (ChildXMLObjects.empty())
  // {
  //   if (Content.empty())
  //   {
  //     xml_tag += "/>";
  //   } else {
  //     xml_tag += ">" + Content + "</" + Name + ">";
  //   }
  // } else {
  // }

  // if (ChildXMLObjects.empty() && Content.empty())
  // {
  //   xml_tag += "/>";
  // } else if (ChildXMLObjects.empty() && !Content.empty())
  // {
  //   xml_tag += ">" + Content;
  // }

  // if (ChildXMLObjects.empty() && Content.empty())
  // {
  //   xml_tag += "/>";
  // } else {
  //   xml_tag += ">" + Content;

  //   for (auto& child: ChildXMLObjects)
  //   {
  //     xml_tag += "\n" + child->render(2);
  //   }
  //   xml_tag += "\n" + ident + "</" + Name + ">";
  // }

  // std::string xml_tag = ident + "<" + Name + s_attrib + ">" + Content;

  // if (!ChildXMLObjects.empty())
  // {
  //   // xml_tag += "\n"; // move to new lane
  //   for (auto& child: ChildXMLObjects)
  //   {
  //     // std::string child_representation = child->render(2);
  //     xml_tag += "\n" + child->render(2);
  //   }
  //   xml_tag += "\n";
  // }

  // /// close tag
  // if (ChildXMLObjects.empty() && (Content.empty()))
  // {
  //   // xml_tag += "/>"; // not correct
  // } else {
  //   xml_tag += ident + "</" + Name + ">";
  // }
  
  return xml_tag;
}

xmlns::XMLObject xmlns::XMLObject::parse(std::string xml)
{
  XMLObject o("", "");
  return o;
}


// xmlns::XMLComment::XMLComment(XMLObject* parent, std::string contents)
// {
//   _initialize();
//   Name = name;
//   Content = contents;
// }

// xmlns::XMLComment::XMLComment(std::string name, std::string contents, XMLAttributes& attributes)
// {
//   _initialize();
//   Name = name;
//   Content = contents;
//   Attributes = attributes;
// }


// svgns::SVGElement::SVGElement()
// {

// }