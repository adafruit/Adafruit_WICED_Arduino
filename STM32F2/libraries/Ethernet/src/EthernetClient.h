#ifndef ethernetclient_h
#define ethernetclient_h
#include "Arduino.h"	
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"

enum
{
  TCP_STATUS_CLOSED           = 1,               /* Connection is closed state   */
  TCP_STATUS_LISTEN_STATE     = 2,               /* Server listen state          */
  TCP_STATUS_SYN_SENT         = 3,               /* SYN sent state               */
  TCP_STATUS_SYN_RECEIVED     = 4,               /* SYN received state           */
  TCP_STATUS_ESTABLISHED      = 5,               /* Connection established state */
  TCP_STATUS_CLOSE_WAIT       = 6,               /* Close Wait state             */
  TCP_STATUS_FIN_WAIT_1       = 7,               /* Finished Wait 1 state        */
  TCP_STATUS_FIN_WAIT_2       = 8,               /* Finished Wait 2 state        */
  TCP_STATUS_CLOSING          = 9,               /* Closing state                */
  TCP_STATUS_TIMED_WAIT       = 10,              /* Timed wait state             */
  TCP_STATUS_LAST_ACK         = 11,              /* Last ACK state               */
};


class EthernetClient : public Client {

public:
  EthernetClient();
  EthernetClient(uint8_t sock);

  uint8_t status();
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
  virtual bool operator==(const bool value) { return bool() == value; }
  virtual bool operator!=(const bool value) { return bool() != value; }
  virtual bool operator==(const EthernetClient&);
  virtual bool operator!=(const EthernetClient& rhs) { return !this->operator==(rhs); };

  friend class EthernetServer;
  
  using Print::write;

private:
  uint32_t _tcp_handle;
};

#endif
