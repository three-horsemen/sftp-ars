#include "security/securesocket.hpp"
#include "shared/logger.hpp"
#include <iostream>
using namespace std;

const int DHKeyContainer::goodPrimePThreshold = 20;
const int DHKeyContainer::goodPrimeQThreshold = 20;
const std::string DHKeyContainer::goodLocalPrivateThreshold = "10";
const std::string DHKeyContainer::goodLocalPublicThreshold = "10";
const std::string DHKeyContainer::goodRemotePublicThreshold = "10";
const std::string DHKeyContainer::goodSharedSecretThreshold = "10";
DHKeyContainer::DHKeyContainer()
{
    setValidity(false);
    sharedSecret = "00000000";
}
void DHKeyContainer::setValidity(bool newValidity)
{
    valid = newValidity;
}
bool DHKeyContainer::getValidity()
{
    return valid;
}
int DHKeyContainer::getPrimeP()
{
    return primeP;
}
void DHKeyContainer::setPrimeP(int p)
{
    primeP = p;
}
int DHKeyContainer::getPrimeQ()
{
    return primeQ;
}
void DHKeyContainer::setPrimeQ(int q)
{
    primeQ = q;
}
std::string DHKeyContainer::getLocalPrivate()
{
    return localPrivate;
}
void DHKeyContainer::setLocalPrivate(std::string newLocalPrivate)
{
    localPrivate = newLocalPrivate;
}
std::string DHKeyContainer::getLocalPublic()
{
    return localPublic;
}
void DHKeyContainer::setLocalPublic(std::string newLocalPublic)
{
    localPublic = newLocalPublic;
}
std::string DHKeyContainer::getRemotePublic()
{
    return remotePublic;
}
void DHKeyContainer::setRemotePublic(std::string newRemotePublic)
{
    remotePublic = newRemotePublic;
}
std::string DHKeyContainer::getSharedSecret()
{
    return sharedSecret;
}
void DHKeyContainer::setSharedSecret(std::string newSharedSecret)
{
    sharedSecret = newSharedSecret;
}
bool DHKeyContainer::isGoodPrimeP()
{
    if(getPrimeP() < goodPrimePThreshold)
    {
        // cout << "isGoodPrimeP(), getPrimeP() failed" << endl;
        return false;
    }
    return true;
}
bool DHKeyContainer::isGoodPrimeQ()
{
    if(getPrimeQ() < goodPrimeQThreshold)
    {
        // cout << "isGoodPrimeQ(), getPrimeQ() failed" << endl;
        return false;
    }
    return true;
}
bool DHKeyContainer::isGoodLocalPrivate()
{
    if(string_to_int(getLocalPrivate()) < string_to_int(goodLocalPrivateThreshold))
    {
        // cout << "isGoodLocalPrivate(), getLocalPrivate() failed" << endl;
        return false;
    }
    return true;
}
bool DHKeyContainer::isGoodLocalPublic()
{
    if(string_to_int(getLocalPublic()) < string_to_int(goodLocalPublicThreshold))
    {
        // cout << "isGoodLocalPublic(), getLocalPublic() failed" << endl;
        return false;
    }
    return true;
}
bool DHKeyContainer::isGoodRemotePublic()
{
    if(string_to_int(getRemotePublic()) < string_to_int(goodRemotePublicThreshold))
    {
        // cout << "isGoodRemotePublic(), getRemotePublic() failed" << endl;
        return false;
    }
    return true;
}
bool DHKeyContainer::isGoodSharedSecret()
{
    if(string_to_int(getSharedSecret()) < string_to_int(goodSharedSecretThreshold))
    {
        // cout << "isGoodSharedSecret(), getSharedSecret() failed" << endl;
        return false;
    }
    return true;
}


SecureSocket::SecureSocket()
{
    setValidity(false);
}
bool SecureSocket::getValidity()
{
    return valid;
}
void SecureSocket::setValidity(bool newValidity)
{
    valid = newValidity;
}
int SecureSocket::getSocketDescriptor()
{
    return socketDescriptor;
}
void SecureSocket::setSocketDescriptor(int newSocketDescriptor)
{
    socketDescriptor = newSocketDescriptor;
}
string SecureSocket::getTargetIPAddress()
{
    return targetIPAddress;
}
void SecureSocket::setTargetIPAddress(string newTargetIPAddress)
{
    targetIPAddress = newTargetIPAddress;
}
string SecureSocket::getTargetPortNumber()
{
    return targetPortNumber;
}
void SecureSocket::setTargetPortNumber(string newTargetPortNumber)
{
    targetPortNumber = newTargetPortNumber;
}
string SecureSocket::getSourceIPAddress()
{
    return sourceIPAddress;
}
void SecureSocket::setSourceIPAddress(string newSourceIPAddress)
{
    sourceIPAddress = newSourceIPAddress;
}
string SecureSocket::getSourcePortNumber()
{
    return sourcePortNumber;
}
void SecureSocket::setSourcePortNumber(string newSourcePortNumber)
{
    sourcePortNumber = newSourcePortNumber;
}
string SecureSocket::getBuffer()
{
    return buffer;
}
void SecureSocket::setBuffer(string newBuffer)
{
    buffer = newBuffer;
}
int SecureSocket::initSecureSocket()
{
    setSocketDescriptor(socket(PF_INET,SOCK_STREAM,0));
    if(getSocketDescriptor() < 0)
    {
        perror("The socket failed to create. socket():");
        setValidity(false);
    }
    else
    {
        //cout << "The socket was successfully created." << endl;
        setValidity(true);
    }
    int force_reuse_socket_port__yes = 1;
    if (setsockopt(getSocketDescriptor(), SOL_SOCKET, SO_REUSEADDR, &force_reuse_socket_port__yes, sizeof(force_reuse_socket_port__yes)) == -1)
    {
        perror("The socket couldn't be reused. setsockopt():");
        setValidity(false);
    }
    else
    {
        //cout << "The socket is being used successfully." << endl;
        setValidity(true);
    }

    return getSocketDescriptor();
}
int SecureSocket::destroySecureSocket()
{
    setValidity(false);
    return close(getSocketDescriptor());
}
string SecureSocket::getSourceAddrFromSockDesc()
{
    struct sockaddr tempSockAddr;
    socklen_t tempLen = sizeof(tempSockAddr);
    getsockname(getSocketDescriptor(), &tempSockAddr, &tempLen);
    return charArray_to_string(inet_ntoa(((struct sockaddr_in*)&tempSockAddr)->sin_addr));
}
string SecureSocket::getSourcePortFromSockDesc()
{
    struct sockaddr tempSockAddr;
    socklen_t tempLen = sizeof(tempSockAddr);
    getsockname(getSocketDescriptor(), &tempSockAddr, &tempLen);
    return int_to_string(ntohs(((struct sockaddr_in*)&tempSockAddr)->sin_port));
}
string SecureSocket::getTargetAddrFromSockDesc()
{
    struct sockaddr tempSockAddr;
    socklen_t tempLen = sizeof(tempSockAddr);
    getpeername(getSocketDescriptor(), &tempSockAddr, &tempLen);
    char *s = inet_ntoa(((struct sockaddr_in*)&tempSockAddr)->sin_addr);
    string result = charArray_to_string(s, strlen(s));
    //free(s);
    return result;
}
string SecureSocket::getTargetPortFromSockDesc()
{
    struct sockaddr tempSockAddr;
    socklen_t tempLen = sizeof(tempSockAddr);
    getpeername(getSocketDescriptor(), &tempSockAddr, &tempLen);
    string result = int_to_string(ntohs(((struct sockaddr_in*)&tempSockAddr)->sin_port));
    return result;
}

DHKeyContainer SecureDataSocket::getKeyContainer()
{
    return keyContainer;
}
int SecureDataSocket::connectSecureSocket()
{
    int result = -2;
    if(getValidity() == true)
    {
        struct sockaddr_in server_address;
        memset(&server_address,0,sizeof(server_address));
    	server_address.sin_family = AF_INET;
        char* targetPortNumber_charArray = string_to_charArray(getTargetPortNumber());
    	server_address.sin_port = htons(atoi(targetPortNumber_charArray));
        //free(targetPortNumber_charArray);
        char* targetIPAddress_charArray = string_to_charArray(getTargetIPAddress());
        server_address.sin_addr.s_addr = inet_addr(targetIPAddress_charArray);
        //free(targetIPAddress_charArray);


        result = connect(getSocketDescriptor(),(struct sockaddr*)&server_address,sizeof(server_address));
        if(result < 0)
        {
            setValidity(false);
            perror("Something went wrong with connect(): ");
        }
        else
        {
            //cout << "Connect was successful!" << endl;
            setValidity(true);
        }
    }
    return result;
}
int SecureDataSocket::readSecureSocket()
{
    int len = -2;
    if(getValidity() == true)
    {
        setBuffer("");
        char buffer_char[256];
        bzero(buffer_char,256);
        len = read(getSocketDescriptor(), buffer_char, sizeof(buffer_char));
        if(len <= 0)
        {
            setValidity(false);
            //printf("Perhaps, the client was disconnected forcefully by the server?\n");
        }
        setBuffer(charArray_to_string(buffer_char, strlen(buffer_char)));
    }
    else
    {
        //printf("%s\n", "While trying to read, the socket was invalid.");
    }
    return len;
}
int SecureDataSocket::writeSecureSocket()
{
    int len = -2;
    if(getValidity() == true)
    {
        char* buffer_char = string_to_charArray(getBuffer());
        len = write(getSocketDescriptor(), buffer_char, strlen(buffer_char));
        if(len <= 0)
        {
            setValidity(false);
            //printf("Perhaps, the server went offline?\n");
        }
        free(buffer_char);
    }
    return len;
}
int SecureDataSocket::performDHExchange_asClient()
{
    if(this->getValidity() == true)
    {
        //First, hello to server.
        // cout << "Sending hello_exchangeDH to the server." << endl;
        this->setBuffer("hello_exchangeDH");
        this->writeSecureSocket();
        if(this->getValidity() == false)
        {
            return -2;
        }
        // cout << "Sent hello_exchangeDH to the server." << endl;
        bool passFlag = false;
        do
        {
            //Second, receive the server's primes and public key.
            //Expected format: <primeP#primeQ@serverPublic>
            this->readSecureSocket();
            if(this->getValidity() == false)
            {
                return -1;
            }
            // cout << "Read the primes and the server public key." << endl;
            //Format checking.
            int index = 0;
            if(this->getBuffer()[index] != '<') return -1;
            while(isdigit(this->getBuffer()[++index]));
            if(this->getBuffer()[index] != '#') return -1;
            while(isdigit(this->getBuffer()[++index]));
            if(this->getBuffer()[index] != '@') return -1;
            while(isdigit(this->getBuffer()[++index]));
            if(this->getBuffer()[index] != '>') return -1;

            vector <string> t = Tokenize(this->getBuffer(), "@#<>");
            this->keyContainer.setPrimeP(string_to_int(t[0]));
            this->keyContainer.setPrimeQ(string_to_int(t[1]));
            this->keyContainer.setRemotePublic(t[2]);

            //Third, send the client's public key.
            this->keyContainer.setLocalPrivate(int_to_string(custom_rand(100)));
            this->keyContainer.setLocalPublic(int_to_string(mpmod(this->keyContainer.getPrimeQ(),
                                                                string_to_int(this->keyContainer.getLocalPrivate()),
                                                                this->keyContainer.getPrimeP())));
            this->setBuffer(this->keyContainer.getLocalPublic());
            this->writeSecureSocket();
            if(this->getValidity() == false)
            {
                return -1;
            }
            // cout << "Sent the client public key." << endl;
            if((this->keyContainer.isGoodPrimeQ() &&
                this->keyContainer.isGoodPrimeP() &&
                this->keyContainer.isGoodRemotePublic() &&
                this->keyContainer.isGoodLocalPublic() &&
                this->keyContainer.isGoodLocalPrivate() &&
                this->getValidity()) == false)
            {
                this->keyContainer.setValidity(false);
                continue;
            }

            //Finally, calculate the shared secret.
            this->keyContainer.setSharedSecret(int_to_string(mpmod(string_to_int(this->keyContainer.getRemotePublic()),
                                                                    string_to_int(this->keyContainer.getLocalPrivate()),
                                                                    (this->keyContainer).getPrimeP()
                                                                )
                                                            )
                                                );
            if(this->keyContainer.isGoodSharedSecret() == false)
            {
                this->keyContainer.setValidity(false);
                continue;
            }
            // cout << "All client checks passed!" << endl;
            passFlag = true;
            this->keyContainer.setValidity(true);
        } while (passFlag == false);
        return 1;
    }
    return 0;
}
int SecureDataSocket::performDHExchange_asServer()
{
    if(this->getValidity() == true)
    {
        //First, hello from client.
        // cout << "Waiting for a hello_exchangeDH" << endl;
        this->readSecureSocket();
        if(this->getValidity() == false)
        {
            return -3;
        }
        if(this->getBuffer() != "hello_exchangeDH")
        {
            this->setValidity(false);
            return -2;
        }
        // cout << "Found a hello_exchangeDH" << endl;
        bool passFlag = false;
        do
        {
            // cout << "Entering the do-while loop." << endl;
            //Second, send the server's primes and public key.
            //Predefined format: <primeP#primeQ@serverPublic>
            while (true)
            {
                this->keyContainer.setPrimeP(next_pr(custom_rand(100)));
                this->keyContainer.setPrimeQ(custom_rand(this->keyContainer.getPrimeP()));
                this->keyContainer.setLocalPrivate(int_to_string(custom_rand(100)));
                this->keyContainer.setLocalPublic(int_to_string(mpmod(this->keyContainer.getPrimeQ(),
                                                                    string_to_int(this->keyContainer.getLocalPrivate()),
                                                                    this->keyContainer.getPrimeP())));
                if((this->keyContainer.isGoodPrimeQ() &&
                    this->keyContainer.isGoodPrimeP() &&
                    this->keyContainer.isGoodLocalPublic() &&
                    this->keyContainer.isGoodLocalPrivate() &&
                    this->getValidity()) == true)
                {
                    break;
                }
                else
                {
                    // cout << "Restarting the server key calculations." << endl;
                }
            }

            // cout << "Calculated the primes and the server key pair." << endl;

            this->setBuffer("<" +
                            int_to_string(this->keyContainer.getPrimeP()) +
                            "#" +
                            int_to_string(this->keyContainer.getPrimeQ()) +
                            "@" +
                            this->keyContainer.getLocalPublic() +
                            ">");
            this->writeSecureSocket();
            if(this->getValidity() == false)
            {
                return -1;
            }
            // cout << "Sent the primes and the server key pair." << endl;
            //Third, receive the client's public key.
            this->readSecureSocket();
            if(this->getValidity() == false)
            {
                return -1;
            }
            // cout << "Read the client's public key." << endl;
            this->keyContainer.setRemotePublic(this->getBuffer());

            if((this->keyContainer.isGoodPrimeQ() &&
                this->keyContainer.isGoodPrimeP() &&
                this->keyContainer.isGoodRemotePublic() &&
                this->keyContainer.isGoodLocalPublic() &&
                this->keyContainer.isGoodLocalPrivate() &&
                this->getValidity()) == false)
            {
                this->keyContainer.setValidity(false);
                continue;
            }

            //Finally, calculate the shared secret.
            this->keyContainer.setSharedSecret(int_to_string(mpmod(string_to_int(this->keyContainer.getRemotePublic()),
                                                    string_to_int(this->keyContainer.getLocalPrivate()),
                                                    this->keyContainer.getPrimeP())));
            if(this->keyContainer.isGoodSharedSecret() == false)
            {
                this->keyContainer.setValidity(false);
                continue;
            }
            // cout << "All server checks passed!" << endl;
            passFlag = true;
            this->keyContainer.setValidity(true);
        } while (passFlag == false);
        return 1;
    }
    // cout << "The situation is invalid." << endl;
    return 0;
}

const int SecureListenSocket::queueSize = 16;
int SecureListenSocket::bindSecureSocket()
{
    int result = -2;
    if(getValidity() == true)
    {
        struct sockaddr_in servAddr;
        memset(&servAddr,0,sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = inet_addr(string_to_charArray(getSourceIPAddress()));
        servAddr.sin_port = htons(atoi(string_to_charArray(getSourcePortNumber())));
        result = bind(getSocketDescriptor(),(struct sockaddr*)&servAddr,sizeof(servAddr));
    }
    return result;
}
int SecureListenSocket::listenSecureSocket()
{
    int result = -2;
    if(getValidity() == true)
    {
        result = listen(getSocketDescriptor(),queueSize);
    }
    return result;
}
SecureDataSocket SecureListenSocket::acceptSecureSocket()
{
    SecureDataSocket newSecureDataSocket;
    newSecureDataSocket.setValidity(false);
    if(getValidity() == true)
    {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int s = accept(getSocketDescriptor(), (struct sockaddr *)&clientAddr, &clientAddrLen);
        newSecureDataSocket.setSocketDescriptor(s);
        newSecureDataSocket.setSourceIPAddress(this->getSourceIPAddress());
        newSecureDataSocket.setSourcePortNumber(this->getSourcePortNumber());
        newSecureDataSocket.setTargetIPAddress(newSecureDataSocket.getTargetAddrFromSockDesc());
        newSecureDataSocket.setTargetPortNumber(newSecureDataSocket.getTargetPortFromSockDesc());
        newSecureDataSocket.setValidity(true);
    }
    return newSecureDataSocket;
}
