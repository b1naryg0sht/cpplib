#include <iostream>
#include <Thread.h>
#include <unistd.h>
#include <XMLParser.h>
#include <string.h>
#include <vector>

using namespace cppbrick;

#define PRINT_INT(val, node) 	{std::string macro_##val = node.get_text();printf(#val"=%d\n", atoi(macro_##val.c_str()));}
#define PRINT_STR(val, node) 	{std::string macro_##val = node.get_text();printf(#val"=%s\n", macro_##val.c_str());}

int main()
{
	XMLParser parser;
	int ret = parser.parse_file(std::string("../src/test/test_xml_parser.xml"));
	if(ret != 0)
	{
		printf("parse_file fail\n");
		assert(ret == 0);
	}
	
	//parse string, one layer
	XMLNode node;
	ret = parser.get_node("Person/name", node);
	assert(ret == 0);
	PRINT_STR(name, node);
	
	//parse string, netsted node
	ret = parser.get_node("Person/province/city", node);
	assert(ret == 0);
	PRINT_STR(city, node);

	//parse int
	ret = parser.get_node("Person/age", node);
	assert(ret == 0);
	PRINT_INT(age, node);
	
	
	//array
	std::vector<XMLNode> vecNode;
	ret = parser.get_nodes("/Person/property/category", vecNode);
	assert(ret == 0);

	assert(vecNode.size() != 0);

	for(unsigned int i=0; i<vecNode.size(); ++i)
	{
		std::string name = vecNode[i].get_attr_str("name");
		long price = atol(vecNode[i].get_attr_str("name").c_str());
		printf("name=%s, price=%ld\n", name.c_str(), price);
	}
	
	return 0;
}
