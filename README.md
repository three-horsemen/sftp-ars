A novel implementation of an SFTP model with server-side file privacy
===

Video Preview
---
[![Video Preview](https://img.youtube.com/vi/XFDfCJqndik/0.jpg)](https://www.youtube.com/watch?v=XFDfCJqndik)


Team members:
	[Ayush Soni](https://github.com/Ayush-Soni),
	[Reuben John](https://github.com/reubenjohn),
	[Soorya Annadurai](https://github.com/soorya-annadurai)

Abstract:
---
The project involves a proposed improvement to the current SSH mechanism. In the current scenario, key-exchange mechanisms like the Diffie-Hellman algorithm are used to transmit security variables across an insecure connection to agree upon a shared secret. SSH consists of three stages, namely, SSH-TRANS, SSH-AUTH, and SSH-CONN. The key-exchange mechanism takes place in the SSH_TRANS stage, and the shared secret is hence used in symmetric encryption algorithms to secure all forthcoming data transfers. Typically, algorithms like AES (128 bit) are used to secure the data transfer.

However, for a given session, the shared secret remains constant. This implies that, if an SSH session is not closed, or the system maintains the connection for too long, attacks can be implemented on the sniffed data traffic to crack the symmetric encryption algorithm's key. This attack can become more effective on a weaker data encryption algorithm.

One way to mitigate this type of attack is by implementing the proposed algorithm, wherein the key exchange is done and repeated in regular intervals. The newly generated shared secret can depend on several factors, such as random system variables, previous data traffic, and improved key exchange mechanisms.

In the FTP model, files are stored in the server, but once a user logs into a machine, he or she is granted complete access to the workspace session, along with any other users who may be concurrently logged in.

In this project, we improve the feasibility of a secured FTP mechanism, by compartmentalizing users and enabling secured file collaboration facilities.


Software Requirements:
---
1. C++11
2. (C++) Boost libraries
3. SQLite3 libraries
4. C Socket Programming libraries
5. CMake and Make
6. Linux OS

Methodology:
---
The project consists of three abstract modules:

1. Security of stored files and connections
2. Synchronization and sharing of permissible files
3. CLI Interface with verified custom commands

Security
---
The initial key exchange mechanism is done using the Diffie-Hellman algorithm, using random primes to generate a shared secret. The shared secret is hence used to encrypt all future transmissions using a custom encryption algorithm. The key exchange is to be repeated in regular intervals, and the encryption key will therefore be regularly modified. An asymmetric algorithm like RSA is used to generate a key-pair, and the public key is used to secure the files on the server, to ensure total privacy.

Synchronization and Sharing
---
Each user is granted an initial compartmentalized workspace for the upload, storage, and download of files. The files stored on the file server can be explicitly shared from one user to another. This does not force the user to download it, but merely grants him or her access to the file. Notifications are also sent to all users if any corresponding files are affected, with the help of a custom timeline server.

User Interface
---
The client-server model is implemented here, where the client(s) uses a custom terminal to connect to the server. This client is capable of interfacing with both the client and the server machines. Commands are sent from the user to the server, and preprocessed for permission violations and syntactical dependencies. If the requested command by the user in question passes the validity challenges, then the user is granted access to the affected files.


Expected Results:
---
Upon completion of this project, we expect to have drafted an API for a new protocol for secured file transmission and storage on a private, local cloud service. This API will be demonstrated through its use in a secured and shared file server.

[Setup](/three-horsemen/sftp/wiki//Source-Setup)
---
Instructions on installation of dependencies and building the project from source can be found in the Wiki.
