# MPCC
Client Server chatbot - Multi Party Chat Conference like whatsapp broadcast list

Description :-
Users register/sign in using the user id and password to the MPCC Chat session.
Then they can send messages to other online members in the group.
Details:
Client-Server Connection: A connection will be established between the client and
server, it is the initial step to initiate the chat.
Active Client Session: A session with the active client must be maintained, in case of
connection loss the client must reconnect to the server with a new session.
Concurrent Server to Serve Multiple Clients: A concurrent Server should be maintained
to serve Multiple Client Connection/ Chat Requests.
Message Broadcast: Message sent by one client must be Broadcasted to all the
Connected Users along with their user-id to identify (what message is written by which
user)
Session Termination: Session must be terminated once the client is disconnected.
IP Mapping to Users: Name of the user is displayed by mapping the IP Address to user-
id
Encryption/Decryption: Encryption/Decryption must be used for sending user Id,
password and messages to attain security in the chat system
