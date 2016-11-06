#ifndef INCLUDE_SECURITY_SECURESOCKET_HPP_
#define INCLUDE_SECURITY_SECURESOCKET_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "security/cryptmath.hpp"
#include "security/cryptstr.hpp"
#include "security/encrypt.hpp"
#include "security/socketexceptions.hpp"
#include "shared/logger.hpp"
#include <exception>
using namespace std;

#define HOST_MODE_SERVER 100
#define HOST_MODE_CLIENT 101
#define DEFAULT_TIMEOUT_VALUE 5
class DHKeyContainer
{
private:
    static const int goodPrimePThreshold;
    static const int goodPrimeQThreshold;
    static const std::string goodLocalPrivateThreshold;
    static const std::string goodLocalPublicThreshold;
    static const std::string goodRemotePublicThreshold;
    static const std::string goodSharedSecretThreshold;

    bool valid;
    int primeP;
    int primeQ;
    std::string localPrivate;
    std::string localPublic;
    std::string remotePublic;
    std::string sharedSecret;
public:
    DHKeyContainer();
    bool isGoodPrimeP();
    bool isGoodPrimeQ();
    bool isGoodLocalPrivate();
    bool isGoodLocalPublic();
    bool isGoodRemotePublic();
    bool isGoodSharedSecret();
    void setValidity(bool newValidity);
    bool getValidity();
    int getPrimeP();
    void setPrimeP(int p);
    int getPrimeQ();
    void setPrimeQ(int q);
    std::string getLocalPrivate();
    void setLocalPrivate(std::string newLocalPrivate);
    std::string getLocalPublic();
    void setLocalPublic(std::string newLocalPublic);
    std::string getRemotePublic();
    void setRemotePublic(std::string newRemotePublic);
    std::string getSharedSecret();
    void setSharedSecret(std::string newSharedSecret);
};

class SecureSocket
{
private:
    bool valid;
    int socketDescriptor;
    std::string targetIPAddress;
    std::string targetPortNumber;

    std::string sourceIPAddress;
    std::string sourcePortNumber;

    std::string buffer;
public:
    SecureSocket();
    void setValidity(bool newValidity);
    bool getValidity() const;
    int getSocketDescriptor() const;
    void setSocketDescriptor(int newSocketDescriptor);
    std::string getTargetIPAddress() const;
    void setTargetIPAddress(std::string newTargetIPAddress);
    std::string getTargetPortNumber() const;
    void setTargetPortNumber(std::string newTargetPortNumber);

    std::string getSourceIPAddress() const;
    void setSourceIPAddress(std::string newSourceIPAddress);
    std::string getSourcePortNumber() const;
    void setSourcePortNumber(std::string newSourcePortNumber);

    std::string getBuffer() const;
    void setBuffer(std::string newBuffer);

    int initSecureSocket();
    int destroySecureSocket();

    std::string getTargetAddrFromSockDesc() const;
    std::string getTargetPortFromSockDesc() const;
    std::string getSourceAddrFromSockDesc() const;
    std::string getSourcePortFromSockDesc() const;

    std::string getTargetAddrFromSockDesc(int) const;
    std::string getTargetPortFromSockDesc(int) const;
    std::string getSourceAddrFromSockDesc(int) const;
    std::string getSourcePortFromSockDesc(int) const;

    // ~SecureSocket();
};

class SecureDataSocket : public SecureSocket
{
private:
    DHKeyContainer keyContainer;
    int timeoutSecValue;
public:
    int getTimeoutSecValue() const;
    void setTimeoutSecValue(int newTimeoutSecValue);
    SecureDataSocket();
    // SecureDataSocket(const SecureDataSocket&);
    SecureDataSocket(int socketDescriptor);
    SecureDataSocket(std::string targetIPAddress, std::string targetPortNumber, int hostMode);
    DHKeyContainer getKeyContainer() const;
    int connectSecureSocket();
    int isTimeout(int timeoutSec, int socket);
    int readSecureSocket();
    int writeSecureSocket();

    void setAndEncryptBuffer(std::string message);
    std::string getAndDecryptBuffer() const;
    void encryptAndSend(std::string message);
    void encryptAndSend();
    std::string receiveAndDecrypt();

    int performDHExchange_asClient();
    int performDHExchange_asServer();
};

class SecureListenSocket : public SecureSocket
{
private:
    static const int queueSize;
public:
    SecureListenSocket();
    SecureListenSocket(std::string serverIPAddress, std::string serverPortNumber);
    int bindSecureSocket();
    int listenSecureSocket();
    SecureDataSocket acceptSecureSocket();
};

#endif
