/*
 * AliasApplicationSettings.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: ccontaxis
 */

#include <AliasApplicationSettings.h>
#include <pugixml.hpp>
#include <XMLDocLoad.h>
#include <boost/tokenizer.hpp>

namespace LSST {
namespace M1M3 {
namespace SS {

void AliasApplicationSettings::load(const std::string &filename) {
	pugi::xml_document doc;
        XMLDocLoad(filename, doc);
	pugi::xpath_node_set nodes = doc.select_nodes("//AliasApplicationSettings/Aliases/Alias");

	typedef boost::tokenizer< boost::escaped_list_separator<char> > tokenizer;

	this->Aliases.clear();
	for(pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); ++node) {
		std::string value = node->node().child_value();
		tokenizer tokenize(value);
		tokenizer::iterator token = tokenize.begin();
		Alias alias;
		alias.Name = *token;
		++token;
		alias.Set = *token;
		++token;
		alias.Version = *token;

		this->Aliases.push_back(alias);
	}
}

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */
