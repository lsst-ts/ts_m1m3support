/*
 * AliasApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <AliasApplicationSettings.h>
#include <pugixml.hpp>
#include <boost/tokenizer.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace pugi;

namespace LSST {
namespace M1M3 {
namespace SS {

void AliasApplicationSettings::load(const std::string &filename) {
	xml_document doc;
	xml_parse_result result = doc.load_file(filename.c_str());
	xpath_node_set nodes = doc.select_nodes("//AliasApplicationSettings/Aliases/Alias");

	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;

	for(xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); ++node) {
		string value = node->node().child_value();
		tokenizer tokenize(value);
		tokenizer::iterator token = tokenize.begin();
		Alias alias;
		alias.Name = *token;
		++token;
		alias.Set = *token;
		++token;
		alias.Version = *token;

		Aliases.push_back(alias);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
