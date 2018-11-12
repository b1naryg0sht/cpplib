#ifndef _BASE_XML_PARSER_H
#define _BASE_XML_PARSER_H

#include "base_common.h"
#include "tinyxml.h"


NS_BASE_BEGIN


class XML_Node
{
public:

	std::string get_text();

	std::string get_attr_str(std::string key);

	template <typename T>
	T get_attr(std::string key);

	std::string to_string();
	
public:
	std::string m_text;
	std::map<std::string, std::string> m_mapAttr;
};




/*
【注意】: tinyxml解析的xml文件标签名称只允许全英文字符
*/
class XML_Parser
{
public:

	int parse_file(std::string xmlFile);

	int parse_str(std::string xmlStr);

	int get_nodes(std::string key, std::vector<XML_Node> &vecNode);
	
	int get_node(std::string key, XML_Node &node); 

private:
	int get_node(TiXmlElement *father_element, std::list<std::string> &lstKey, 
		std::vector<XML_Node> &vecNode);

	void create_node(TiXmlElement *element, std::vector<XML_Node> &vecNode);
		
private:
	TiXmlDocument m_document;

};


NS_BASE_END


#include "base_xml_parser.inl"


#endif


