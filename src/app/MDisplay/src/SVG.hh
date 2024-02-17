
#include<iostream>
#include<string>
#include<vector>
#include<map>

#ifndef SVG_H
#define SVG_H

namespace xmlns
{
  typedef std::map<std::string, std::string> XMLAttributes;

  enum XMLObjectType
  {
    File = 0, // top level xml object
    Header,
    Doctype,
    Tag,
    Data,
    Comment,
    Text, // text between tags. i.e. <path>suyfdu <bd>sdfbkj</bd> skdj</path>
    Css,
    Js
  };

  class XMLObject
  {
    private:
      XMLObject* ParentXMLObj = nullptr;
      XMLObjectType Type;
      std::string Name;
      std::string Content;
      XMLAttributes Attributes;
      std::vector<XMLObject*> ChildXMLObjects;
      int Level; // TODO: add update of Level for childs
      bool _debug = false;

    public:     
      XMLObject(
        XMLObject* parent, XMLObjectType type,
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          ParentXMLObj(parent), Type(type),
          Name(name), Content(contents), Attributes(attributes),
          Level( (ParentXMLObj)? ParentXMLObj->getLevel() + 1: 0)
          { if (parent) parent->addChild(this); };
      XMLObject(
        XMLObjectType type,
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          XMLObject(nullptr, type, name, contents, attributes){};
      XMLObject(
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          XMLObject(nullptr, XMLObjectType::Tag, name, contents, attributes){};
      XMLObject(
        XMLObject* parent,
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          XMLObject(parent, XMLObjectType::Tag, name, contents, attributes) {};
      XMLObject() :
          XMLObject(nullptr, XMLObjectType::File, "", "") {};
      XMLObject(std::string xml);

      ~XMLObject() { ParentXMLObj = nullptr; ChildXMLObjects.clear(); Attributes.clear(); };

      std::string             getName      () { return Name; };
      std::string             getContent   () { return Content; }
      XMLAttributes           getAttributes() { return Attributes; }
      XMLObject*              getParent    () { return ParentXMLObj; }
      std::vector<XMLObject*> getChilds    () { return ChildXMLObjects; }
      int                     getLevel     () { return Level; };
      XMLObjectType           getType      () { return Type; }

      void setContent   (std::string content) { Content = content; };
      void setAttributes(XMLAttributes& attributes) { Attributes = attributes; };
      void setAttribute (std::string attribute, std::string value) { Attributes.insert_or_assign(attribute, value); };
      void setParent    (XMLObject& parent) { ParentXMLObj = &parent; };
      void setParent    (XMLObject* parent) { ParentXMLObj = parent; };
      void setLevel     (int level) { Level = level; };
      void setType      (XMLObjectType type) { Type = type; }

      void addChild   (XMLObject* child, XMLObject* before_child = nullptr, XMLObject* after_child = nullptr);
      // void addText(std::string text, XMLObject* before_child = nullptr, XMLObject* after_child = nullptr);
      void removeChild(XMLObject* child);

      // std::string render(int tab_size);
      std::string render(int tab_size, std::string indentation = " ");

      struct tagParse
      {
        std::string Name;
        XMLAttributes attributes;
      };
      XMLObject* parse(std::string xml);
      tagParse _parse_tag_definition(std::string tag_definition);

      
  };

}

namespace svgns
{
  class SVGElement : public xmlns::XMLObject
  {
    public:
      SVGElement(std::map<std::string, std::string>& attributes);
      ~SVGElement();

      void        render(std::string out_file_path);
      std::string render();

      SVGElement parse(std::string svg);

      void addGroup();
      void addPath();
      
  };
}


#endif
