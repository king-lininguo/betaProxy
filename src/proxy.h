//
// Created by IntelliJ IDEA.
// User: AppleTree
// Date: 17/3/18
// Time: 下午6:33
// To change this template use File | Settings | File Templates.
//

#ifndef __hive__proxy__
#define __hive__proxy__

#include "common.h"
#include "buffer.h"
#include "token.h"
#include "object.h"
#include "timer.h"
#include "epoll.h"
#include "accept.h"
#include "client.h"
#include "listener.h"
#include "systemsignal.h"

NS_HIVE_BEGIN

class ProxyLogic
{
public:
	ProxyLogic(void){}
	virtual ~ProxyLogic(void){}

	// receive connection called
	virtual void onReceiveAccept(uint32 handle) = 0;
	// receive message from accept;
	// after client to back server create success this will never call again;
	virtual void onReceiveMessage(Buffer* pBuffer) = 0;
	// client connection to back server success
	virtual void onOpenClientSuccess(uint32 handle) = 0;
	// client connection to back server failed
	virtual void onOpenClientFailed(uint32 handle) = 0;
	// front direction close the connection
	virtual void onAcceptClosed(uint32 handle) = 0;
	// back direction close the connection
	virtual void onClientClosed(uint32 handle) = 0;
};

typedef ObjectPool<Listener> ListenerPool;
typedef ObjectPool<Accept> AcceptPool;
typedef ObjectPool<Client> ClientPool;

class Proxy : public RefObject
{
public:
	Epoll* m_pEpoll;
	Timer* m_pTimer;
	ListenerPool* m_pListenerPool;
	AcceptPool* m_pAcceptPool;
	ClientPool* m_pClientPool;
public:
	Proxy(void);
	virtual ~Proxy(void);

	static Proxy* getInstance(void);
	static Proxy* createInstance(void);
	static void destroyInstance(void);

	uint32 openListener(const char* ip, uint16 port, AcceptSocketFunction pFunc);
	uint32 openAccept(int fd, const char* ip, uint16 port);
	uint32 openClient(const char* ip, uint16 port);
	void receiveClient(Client* pClient);

	Listener* getListener(uint32 handle);
	Accept* getAccept(uint32 handle);
	Client* getClient(uint32 handle);

	bool closeListener(uint32 handle);
	bool closeAccept(uint32 handle);
	bool closeClient(uint32 handle);

	void update(void);
	void initialize(void);
	void destroy(void);

	inline Epoll* getEpoll(void){ return m_pEpoll; }
	inline Timer* getTimer(void){ return m_pTimer; }
public:

};

NS_HIVE_END

#endif
