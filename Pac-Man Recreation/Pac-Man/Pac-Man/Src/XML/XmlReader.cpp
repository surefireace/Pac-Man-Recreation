// Made by Donovan Colen
#include "XmlReader.h"

// attemps to load the file and returns the root element
tinyxml2::XMLElement* XmlReader::Load(string path)
{
    m_path = path;
    m_doc.LoadFile(path.c_str());    // load the xml file

    if (m_doc.Error())
    {
        cout << "error loading xml: " << m_doc.ErrorStr() << endl;
        return nullptr;
    }

    return m_doc.RootElement();
}

// gets the attributes for a given element and returns a map of attribute value pairs in strings. returns nullptr if no attributes
map<string, string> XmlReader::GetAttributes(tinyxml2::XMLElement * pElement)
{
    map<string, string> attributes;
    
    if (pElement->FirstAttribute() == NULL)
    {
        return attributes;
    }

    const tinyxml2::XMLAttribute* att = pElement->FirstAttribute();

    while (att)
    {
        attributes.emplace(att->Name(), att->Value());
        att = att->Next();
    }

    return attributes;
}

// gets all the following child elements not includeing their children
vector<tinyxml2::XMLElement*> XmlReader::GetElements(tinyxml2::XMLElement * pRoot)
{
    vector<tinyxml2::XMLElement*> pElements;

    if (pRoot->NoChildren())
    {
        return pElements;
    }

    tinyxml2::XMLElement* ele = pRoot->FirstChildElement();
    while (ele)
    {
        pElements.emplace_back(ele);
        ele = ele->NextSiblingElement();
    }

    return pElements;
}