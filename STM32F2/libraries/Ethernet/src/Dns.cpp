// Arduino DNS client for WizNet5100-based Ethernet shield
// (c) Copyright 2009-2010 MCQN Ltd.
// Released under Apache License, version 2.0

//#include "EthernetUdp.h"
#include "utility/util.h"

#include "Dns.h"
#include <string.h>
//#include <stdlib.h>
#include "Arduino.h"

// Possible return codes from ProcessResponse
#define SUCCESS          1
#define TIMED_OUT        -1
#define INVALID_SERVER   -2
#define TRUNCATED        -3
#define INVALID_RESPONSE -4

void DNSClient::begin(const IPAddress& aDNSServer)
{
  (void) aDNSServer;
}

int DNSClient::getHostByName(const char* aHostname, IPAddress& aResult)
{
  uint32_t ip_addr = 0;
  int err;

  if( ERROR_NONE != (err = (int) FEATHERLIB->sdep_execute(SDEP_CMD_DNSLOOKUP, strlen(aHostname), aHostname, NULL, &ip_addr)) )
  {
    return (-err);
  }

  aResult = ip_addr;

  return 1;
}

IPAddress DNSClient::getHostByName(const char* aHostname)
{
  uint32_t ip_addr = 0;

  VERIFY( ERROR_NONE == FEATHERLIB->sdep_execute(SDEP_CMD_DNSLOOKUP, strlen(aHostname), aHostname, NULL, &ip_addr),
          INADDR_NONE);

  return IPAddress(ip_addr);
}

#if 0
int DNSClient::inet_aton(const char* aIPAddrString, IPAddress& aResult)
{
    // See if we've been given a valid IP address
    const char* p =aIPAddrString;
    while (*p &&
           ( (*p == '.') || (*p >= '0') || (*p <= '9') ))
    {
        p++;
    }

    if (*p == '\0')
    {
        // It's looking promising, we haven't found any invalid characters
        p = aIPAddrString;
        int segment =0;
        int segmentValue =0;
        while (*p && (segment < 4))
        {
            if (*p == '.')
            {
                // We've reached the end of a segment
                if (segmentValue > 255)
                {
                    // You can't have IP address segments that don't fit in a byte
                    return 0;
                }
                else
                {
                    aResult[segment] = (byte)segmentValue;
                    segment++;
                    segmentValue = 0;
                }
            }
            else
            {
                // Next digit
                segmentValue = (segmentValue*10)+(*p - '0');
            }
            p++;
        }
        // We've reached the end of address, but there'll still be the last
        // segment to deal with
        if ((segmentValue > 255) || (segment > 3))
        {
            // You can't have IP address segments that don't fit in a byte,
            // or more than four segments
            return 0;
        }
        else
        {
            aResult[segment] = (byte)segmentValue;
            return 1;
        }
    }
    else
    {
        return 0;
    }
}
#endif
