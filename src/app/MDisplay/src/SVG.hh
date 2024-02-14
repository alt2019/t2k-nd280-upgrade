
#include<string>
#include<vector>
#include<map>

#ifndef SVG_H
#define SVG_H

namespace xmlns
{
  typedef std::map<std::string, std::string> XMLAttributes;

  class XMLObject
  {
    private:
      std::string Name;
      std::string Content;
      XMLAttributes Attributes;
      XMLObject* ParentXMLObj;
      std::vector<XMLObject*> ChildXMLObjects;
      // const char _identination_symbol;
      int Level;

      void _initialize();
      void _reset();
      void _setLevel() { if (ParentXMLObj) { Level = ParentXMLObj->getLevel() + 1; } };
      // void _setIdentination();

    public:
      XMLObject(std::string name, std::string contents);
      XMLObject(std::string name, std::string contents, XMLAttributes& attributes);
      XMLObject(XMLObject* parent, std::string name, std::string contents, std::string identination, XMLAttributes& attributes);
      // XMLObject(XMLObject* parent, std::string name, std::string contents, std::string identination, XMLAttributes& attributes, std::string type);
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
      // void setIdentination();

      void addChild(XMLObject* child, XMLObject* before_child = nullptr, XMLObject* after_child = nullptr);
      void removeChild(XMLObject* child);

      std::string render(int tab_size);

      XMLObject parse(std::string xml);
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
      
  };
}


#endif
