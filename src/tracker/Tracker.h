/*
 * Tracker.h
 *
 *  Created on: 2013年7月27日
 *      Author: sun
 */

#ifndef TRACKER_H_
#define TRACKER_H_

#include "TrackerService.h"

namespace sdfs {

typedef struct{
	ServerConfig	tracker;
	ServerConfig	backend[BACKED_SERVER_SIZE];
	int				worksize;
	//TODO: other configurations...
}TrackerConfig;

class Tracker: public TrackerService {
public:
	Tracker();
	virtual ~Tracker();
protected:
	int ReadConfiguration();

	int OpenListenSocket();

	int ConnectBackendServer();

	int InitFileTable();

	int InitConnectionPool();

	int StartWorkers();

	int CloseConnectionPool();

	int StopWorkers();

	int CloseFileTable();

	int CloseListenSocket();
private:
	int				m_listenfd;
	TrackerConfig	m_sConf;
};

} /* namespace sdfs */
#endif /* TRACKER_H_ */
