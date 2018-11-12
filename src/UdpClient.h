#ifndef __YOGURT_UDP_CLIENT_H__
#define __YOGURT_UDP_CLIENT_H__

namespace cppbrick {


class UdpClient
{
public:
	UdpClient();

	virtual ~UdpClient();

	int connect(const std::string &ip, ushort_t port, bool doconnect=false);

	int sendto(const char *buf, const uint_t len, int flags=0);

	int recvfrom(char *buf, const uint_t len);

	void close();

	bool is_close();

	int fd();

private:
	int _fd;
	bool _connect;
	bool _close;
	std::string _ip;
	unsigned short _port;
};

}

#endif


