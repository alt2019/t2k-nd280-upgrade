
#include<algorithm>
#include "SVG.hh"

void xmlns::XMLObject::_initialize()
{
  Name = "";
  Content = "";
  ParentXMLObj = nullptr;
  Level = 0;
}

void xmlns::XMLObject::_reset() // TODO: implement
{
  Name = "";
  Content = "";
  ParentXMLObj = nullptr;
  Level = 0;
}

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

xmlns::XMLObject::XMLObject(
  xmlns::XMLObject* parent,
  std::string name,
  std::string contents,
  std::string identination,
  XMLAttributes& attributes)
{
  _initialize();
  Name = name;
  Content = contents;
  Attributes = attributes;
  ParentXMLObj = parent;

  // if (ParentXMLObj)
  // {
  //   Level = ParentXMLObj->getLevel() + 1;
  // }
  _setLevel();
  
}

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

std::string xmlns::XMLObject::render(int tab_size) // tab symbol 
{
  // std::string ident = "";
  const char identination_symbol = ' ';
  std::string ident = std::string(Level * tab_size, identination_symbol);
  // if      (identination_symbol == '\t') ident = std::string(Level    , identination_symbol);
	// else if (identination_symbol == ' ' ) ident = std::string(Level * 2, identination_symbol);

  std::string s_attrib = "";
  if (!Attributes.empty())
  {
    for (auto const& [attr, val] : Attributes)
    {
      s_attrib += attr + "=\"" + val + "\"";
    }
  }

  std::string xml_tag = ident + "<" + Name + " " + s_attrib + ">" + Content;

  if (!ChildXMLObjects.empty())
  {
    // xml_tag += "\n"; // move to new lane
    for (auto& child: ChildXMLObjects)
    {
      // std::string child_representation = child->render(2);
      xml_tag += "\n" + child->render(2);
    }
    xml_tag += "\n";
  }

  /// close tag
  if (ChildXMLObjects.empty() && (Content.empty()))
  {
    // xml_tag += "/>"; // not correct
  } else {
    xml_tag += ident + "</" + Name + ">";
  }
  
  return xml_tag;
}

xmlns::XMLObject xmlns::XMLObject::parse(std::string xml)
{
  XMLObject o("", "");
  return o;
}




// svgns::SVGElement::SVGElement()
// {

// }