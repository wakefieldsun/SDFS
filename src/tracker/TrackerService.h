/*
 * TrackerService.h
 *
 *  Created on: 2013年7月26日
 *      Author: sun
 */

#ifndef TRACKERSERVICE_H_
#define TRACKERSERVICE_H_

namespace sdfs {


class TrackerService {
public:
	TrackerService();
	virtual ~TrackerService();

	virtual void Start();
protected:

	virtual int ReadConfiguration();

	virtual int OpenListenSocket();

	virtual int ConnectBackendServer();

	virtual int InitFileTableServer();

	virtual int InitWorkers();
};

} /* namespace sdfs */
#endif /* TRACKERSERVICE_H_ */
