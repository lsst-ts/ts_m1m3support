/*
 * ResetBustList.h
 *
 *  Created on: Oct 19, 2017
 *      Author: ccontaxis
 */

#ifndef RESETBUSTLIST_H_
#define RESETBUSTLIST_H_

#include <BusList.h>

namespace LSST {
namespace M1M3 {
namespace SS {

class ResetBustList: public BusList {
public:
	ResetBustList();
	ResetBustList(ILCSubnetData* subnetData, ILCMessageFactory* ilcMessageFactory);
};

} /* namespace SS */
} /* namespace M1M3 */
} /* namespace LSST */

#endif /* RESETBUSTLIST_H_ */
