// Made by Donovan Colen
#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "../Tinyxml2/tinyxml2.h"

using namespace std;

class XmlReader
{
private:
    string m_path;
    tinyxml2::XMLDocument m_doc;

public:
    XmlReader() = default;
    ~XmlReader() = default;

    // attemps to load the file and returns the root element
    tinyxml2::XMLElement* Load(string path);

    // gets the attributes for a given element and returns a map of attribute value pairs in strings. returns nullptr if no attributes
    map<string, string> GetAttributes(tinyxml2::XMLElement* pElement);

    // gets all the following child elements not includeing their children
    vector<tinyxml2::XMLElement*> GetElements(tinyxml2::XMLElement* pRoot);
};