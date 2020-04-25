#ifndef NiStatus_H_
#define NiStatus_H_

#include <stdint.h>

/**
 * Converts NI error to string description. See
 * http://zone.ni.com/reference/en-XX/help/372928H-01/capi/errors/ for error
 * codes.
 *
 * @param status status to convert
 *
 * @return error description, or "Unknow error status" if not known
 */
const char* NiStatus(int32_t status);

int32_t NiReportError(const char* msg, int32_t status);

#endif /* NiStatus_H_ */
