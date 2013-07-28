/*
 * TrackerService.h
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#ifndef TRACKERSERVICE_H_
#define TRACKERSERVICE_H_
#include "../common/NetworkHelper.h"


namespace sdfs {

class TrackerService {
public:
	TrackerService();
	virtual ~TrackerService();
public:
	virtual int Start();
	virtual int Stop();
protected:

	virtual int ReadConfiguration() = 0;

	virtual int OpenListenSocket() = 0;

	virtual int ConnectBackendServer() = 0;

	virtual int InitFileTable() = 0;

	virtual int InitConnectionPool() = 0;

	virtual int StartWorkers() = 0;

	virtual int CloseConnectionPool() = 0;

	virtual int StopWorkers() = 0;

	virtual int CloseFileTable() = 0;

	virtual int CloseListenSocket() = 0;

};

} /* namespace sdfs */
#endif /* TRACKERSERVICE_H_ */
