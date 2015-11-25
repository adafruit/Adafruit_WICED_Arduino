extern "C" {
  #include "string.h"
}

#include "Arduino.h"

#include "Ethernet.h"
#include "EthernetClient.h"
//#include "EthernetServer.h"
#include "Dns.h"

EthernetClient::EthernetClient()
{
  _tcp_handle = 0;
}

EthernetClient::EthernetClient(uint8_t sock)
{
  (void) sock;
  _tcp_handle = 0;
}

int EthernetClient::connect(const char* host, uint16_t port) {
  // Look up the host first
  int ret = 0;
  DNSClient dns;
  IPAddress remote_addr;

  ret = dns.getHostByName(host, remote_addr);
  VERIFY( ret == 1, ret);

  return this->connect(remote_addr, port);
}

int EthernetClient::connect(IPAddress ip, uint16_t port) {
  typedef struct PACKED {
    uint8_t  interface;
    uint8_t  is_tls;
    uint32_t ipv4;
    uint16_t port;
    uint32_t timeout_ms;
  } sdep_tcp_connect_t;

  sdep_tcp_connect_t connect_input;
  connect_input.interface  = WIFI_INTERFACE_STATION;
  connect_input.is_tls     = false;
  connect_input.ipv4       = (uint32_t) ip;
  connect_input.port       = port;
  connect_input.timeout_ms = 1000;

  int err;
  if( ERROR_NONE != (err = (int) FEATHERLIB->sdep_execute(SDEP_CMD_TCP_CONNECT,
                                                                   sizeof(sdep_tcp_connect_t), &connect_input,
                                                                   NULL, &_tcp_handle) ) )
  {
    return (-err);
  }

  return 1;
}

size_t EthernetClient::write(uint8_t b) {
  return this->write(&b, 1);
}

size_t EthernetClient::write(const uint8_t *buf, size_t size) {
  if( ERROR_NONE != FEATHERLIB->sdep_execute_extend(SDEP_CMD_TCP_WRITE,
                                                             4, &_tcp_handle,
                                                             size, buf,
                                                             NULL, NULL) )
  {
    return 0;
  }

  return size;
}

int EthernetClient::available() {
  return 0;
}

int EthernetClient::read() {
  uint8_t b;
  if ( this->read(&b, 1) > 0 )
  {
    // recv worked
    return b;
  }
  else
  {
    // No data available
    return -1;
  }
}

int EthernetClient::read(uint8_t *buf, size_t size)
{
  uint8_t para2[6];

  memcpy(para2, &size, 2); // uint16_t only
  *((uint32_t*)(para2+2)) = 1000; // timeout


  if( ERROR_NONE != FEATHERLIB->sdep_execute_extend(SDEP_CMD_TCP_READ,
                                                             4, &_tcp_handle,
                                                             6, para2,
                                                             NULL, buf) )
  {
    return -1;
  }

  return size;
}

int EthernetClient::peek() {
//  uint8_t b;
//  // Unlike recv, peek doesn't check to see if there's any data available, so we must
//  if (!available())
//    return -1;
//  ::peek(_sock, &b);
//  return b;
  return -1;
}

void EthernetClient::flush() {
  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_FLUSH,
                                    4, &_tcp_handle,
                                    NULL, NULL);
}

void EthernetClient::stop() {
  if ( _tcp_handle == 0 ) return;

  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_CLOSE,
                                    4, &_tcp_handle,
                                    NULL, NULL);
  _tcp_handle = 0;
}

uint8_t EthernetClient::connected() {
  if ( _tcp_handle == 0 ) return 0;
  
  uint8_t s = status();
  return !(s == TCP_STATUS_LISTEN_STATE || s == TCP_STATUS_CLOSED ||
           s == TCP_STATUS_FIN_WAIT_1 || s == TCP_STATUS_FIN_WAIT_2 ||
           (s == TCP_STATUS_CLOSED && !available()) );
}

uint8_t EthernetClient::status() {
  if ( _tcp_handle == 0 ) return TCP_STATUS_CLOSED;

  uint8_t status = TCP_STATUS_CLOSED;
  FEATHERLIB->sdep_execute(SDEP_CMD_TCP_STATUS,
                                    4, &_tcp_handle,
                                    NULL, &status);
  return status;
}

// the next function allows us to use the client returned by
// EthernetServer::available() as the condition in an if-statement.

EthernetClient::operator bool() {
  return ( _tcp_handle != 0 );
}

bool EthernetClient::operator==(const EthernetClient& rhs) {
  return (this->_tcp_handle == rhs._tcp_handle) && (this->_tcp_handle != 0);
}
