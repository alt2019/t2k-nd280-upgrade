
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
    Header = 0,
    Doctype,
    Tag,
    Data,
    Comment,
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
      int Level;
      // std::string Indent;

      // void _initialize();
      // void _reset();
      // void _setLevel() { if (ParentXMLObj) { Level = ParentXMLObj->getLevel() + 1; } };

    public:
      // XMLObject(std::string name, std::string contents);
      // XMLObject(std::string name, std::string contents) :
      //   ParentXMLObj(nullptr),
      //   Type(XMLObjectType::Tag),
      //   Name(name),
      //   Content(contents),
      //   Indent(" "),
      //   Level(0)
      //   {};
      // XMLObject(std::string name, std::string contents, XMLAttributes& attributes);
      // XMLObject(XMLObjectType type, std::string name, std::string contents, std::string indentation = " ");
      // XMLObject(XMLObject* parent, std::string name, std::string contents, std::string indentation, XMLAttributes& attributes);
      
      // XMLObject(XMLObject* parent, XMLObjectType type, std::string name, std::string contents, std::string indentation, XMLAttributes& attributes);
      // XMLObject(XMLObjectType type, std::string name, std::string contents, std::string indentation, XMLAttributes& attributes) : 
      //   XMLObject(nullptr, type, name, contents, indentation, attributes){};
      // XMLObject(std::string name, std::string contents, std::string indentation, XMLAttributes& attributes) : 
      //   XMLObject(nullptr, XMLObjectType::Tag, name, contents, indentation, attributes){};
      
      XMLObject(
        XMLObject* parent, XMLObjectType type,
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          ParentXMLObj(parent), Type(type),
          Name(name), Content(contents), Attributes(attributes),
          Level( (ParentXMLObj)? ParentXMLObj->getLevel() + 1: 0)
          {};
      XMLObject(
        XMLObjectType type,
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          XMLObject(nullptr, type, name, contents, attributes){};
      XMLObject(
        std::string name, std::string contents, const XMLAttributes& attributes = XMLAttributes()) : 
          XMLObject(nullptr, XMLObjectType::Tag, name, contents, attributes){};

      ~XMLObject();

      std::string             getName() { return Name; };
      std::string             getContent() { return Content; }
      XMLAttributes           getAttributes() { return Attributes; }
      XMLObject*              getParent() { return ParentXMLObj; }
      std::vector<XMLObject*> getChilds() { return ChildXMLObjects; }
      int                     getLevel() { return Level; };

      void setContent(std::string content) { Content = content; };
      void setAttributes(XMLAttributes& attributes) { Attributes = attributes; };
      // void setAttribute(std::tuple<std::string, std::string>& attribute) {  };
      void setAttribute(std::string attribute, std::string value) { Attributes.insert_or_assign(attribute, value); };
      void setParent(XMLObject& parent) { ParentXMLObj = &parent; };
      void setParent(XMLObject* parent) { ParentXMLObj = parent; };

      void addChild(XMLObject* child, XMLObject* before_child = nullptr, XMLObject* after_child = nullptr);
      void removeChild(XMLObject* child);

      std::string render0(int tab_size);
      std::string render(int tab_size);
      // std::string render(int tab_size, std::string indentation);

      XMLObject parse(std::string xml);
  };

  /*
  class XMLComment : public XMLObject
  {
    XMLComment(XMLObject* parent, std::string contents);
    ~XMLComment();
    std::string render(int tab_size);
  };

  class XMLHeader : public XMLObject
  {
    XMLHeader(std::string contents, XMLAttributes& attributes);
    ~XMLHeader();
    std::string render(int tab_size);
  };

  class XMLTag : public XMLObject
  {
    XMLTag(XMLObject* parent, std::string name, std::string contents, std::string indentation, XMLAttributes& attributes);
    ~XMLTag();
    std::string render(int tab_size);
  };
  // */
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
      
  };
}


#endif
