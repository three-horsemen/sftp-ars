//Client program to keep talking to a server until a "exit" message is sent.
#include "security/securesocket.hpp"

#include <database/UserManager.hpp>
#include <database/TimelineManager.hpp>
#include <ui/UserSessionDetail.hpp>
#include <ui/CommandInterpreter.hpp>

using namespace std;
using namespace sftp::db;

#define boldAndRed(str) Utils::displayInBoldAndRed(str)
#define boldAndViolet(str) Utils::displayInBoldAndViolet(str)

int main() {
	boldAndRed(string("\t\tWelcome to Secure File Transfer Protocol\n"));
	SecureDataSocket clientSocket("127.0.0.1", "8081", HOST_MODE_CLIENT);

	string buffer;
	try {
		buffer = clientSocket.receiveAndDecrypt();
		if (buffer != UserManager::USERNAME)
			throw invalid_argument(
					string("Unknown server response: ") + buffer);
		string username = "";
		boldAndViolet(string("Username: "));
		getline(cin, username);
		clientSocket.encryptAndSend(username);

		buffer = clientSocket.receiveAndDecrypt();
		if (buffer != UserManager::PASSWORD)
			throw invalid_argument(string("Unknown server response: ") + buffer);
		string password = "";
		boldAndViolet(string("Password: "));
		password = Utils::getPassword();
		clientSocket.encryptAndSend(password);
		buffer = clientSocket.receiveAndDecrypt();
		if (buffer == UserManager::CREDENTIALS_VALID) {
			boldAndViolet(string("Login success (To logout use the '") + UserManager::EXIT + "' command)");
		} else {
			throw invalid_argument("Invalid credentials");
		}

		UserSessionDetail user(username);
		try {
			while (clientSocket.getValidity()) {
				boldAndViolet(string("\n") + user.getUsername());
				boldAndRed(string("@client-sftp"));
				cout << user.getPresentWorkingDirectory();
				std::string rawCommand;
				getline(cin, rawCommand);
				if (UserManager::isExitCommand(rawCommand)) {
					clientSocket.encryptAndSend(rawCommand);
					throw runtime_error("Logging out...");
				} else {
					try {
						Command *command = CommandInterpreter::getInterpretedCommand(rawCommand,
																					 user.getPresentWorkingDirectory());
						command->execute();

						if (command->getType() == ChangeDirectoryCommand::TYPE) {
							string newWorkingDirectory = ChangeDirectoryCommand::getPathSpecified(*command);
							user.setPresentWorkingDirectory(newWorkingDirectory);
						}
						cout << command->getOutput();
					} catch (UIException &e) {
						boldAndRed(e.what());
						continue;
					}
				}
			}
		} catch (SecureSocketException &e) {
			throw SecureSocketException(e.errorCode, string("Failed to received notification: ") + e.what());
		}

		//TODO Handle sending of commands once secure socket timeouts are implemented:
		while (clientSocket.getValidity()) {
			cout << "$ ";
			string command;
			getline(cin, command);
		}
	} catch (exception &e) {
		boldAndRed(string(e.what()) + "\n");
		clientSocket.destroySecureSocket();
		return 0;
	}
	cout << "The server unexpectedly hung up" << endl;
	return 0;
}