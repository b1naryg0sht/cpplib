#ifndef _CPPBRICK_XML_PARSER_H__
#define _CPPBRICK_XML_PARSER_H__

#include <string>
#include <map>
#include <vector>
#include <list>
#include "tinyxml.h"


namespace cppbrick {


class XMLNode
{
public:

	std::string get_text();

	std::string get_attr_str(std::string key);

	template <typename T>
	T get_attr(std::string key)
	{
	}

	std::string to_string();
	
public:
	std::string m_text;
	std::map<std::string, std::string> m_mapAttr;
};




/*
【注意】: tinyxml解析的xml文件标签名称只允许全英文字符
*/
class XMLParser
{
public:

	int parse_file(std::string xmlFile);

	int parse_str(std::string xmlStr);

	int get_nodes(std::string key, std::vector<XMLNode> &vecNode);
	
	int get_node(std::string key, XMLNode &node); 

private:
	int get_node(TiXmlElement *father_element, std::list<std::string> &lstKey, 
		std::vector<XMLNode> &vecNode);

	void create_node(TiXmlElement *element, std::vector<XMLNode> &vecNode);
		
private:
	TiXmlDocument m_document;

};


}



#endif


