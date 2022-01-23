/*Kent Studer - krspcp (12544417), CS 4850 Socket Chatroom Server Source Code*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT  14417
#define MAX_LINE 256
#define MAX_USER 15

typedef struct userstruct{
	char username[33]; //added 1 for end parentheses and comma
	char password[9];
}User;

//Function Prototypes
User * loadUserData(char *);

int main(){

	//creating the server socket
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//Defining the address structure for the server
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//binding the socket to our specified IP and port
	bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
	
	//listening for connections
	listen(server_socket, 5);

	int client_socket = accept(server_socket, NULL, NULL);
	if(client_socket < 0){
		printf("Error at accept\n");
	}else{
		printf("My chat room server. Version One.\n");
	}

	int command_flag = 0; //used to block commands if logged in or out
	int disc_flag = 1; //used to determine if someone has logged out with resets the client socket connetion for further use

	//error/success codes
	char already_logged[] = "Denied. Already logged in.";
	char not_logged[] = "Denied. Please login first.";
	char loginSuccess[] = "login confirmed.";
	char loginFail[] = "Denied. Username or password incorrect.";
	char newUserSuccess[] = "New user account created. Please login.";
	char newUserFail[] = "Denied. User account already exists.";

	//infinite loop since server stay on unless control C'ed
	while(1){
		char server_message[MAX_LINE];
		char client_message[MAX_LINE];

		//receiving messages from the client socket
		int read_error = recv(client_socket, client_message, MAX_LINE, 0);
		if(read_error == 0 || disc_flag == 0){ //if messages arent not receive (i.e., disconnected)
			printf("Client Disconnected\n");
			client_socket = 0; //reset client socket variable
			client_socket = accept(server_socket, NULL, NULL); //open it for connecting to new users again
			disc_flag = 1;
		}
		//used for echo
		char client_name[32];

		//int read_error = recv(client_socket, client_message, MAX_LINE, 0);

		//Everytime this loops we need a way to update the array with newusers
		User * array = loadUserData("users.txt");

		//we have now received a message from the client and can begin to analyze what to do with it
		char bufCopy[MAX_LINE]; //me make a copy for the send function
		strcpy(bufCopy, client_message);

		char * command = strtok(client_message, " ");
		if(strcmp(command, "login") == 0){
			//we need to first compare the username to our array of usernames to determine if its in the database
			//tokenize the string and copy each token into a string for both the username and password
			if(command_flag == 1){ //this checks if we are already logged in to prevent the command from executing again
				send(client_socket, already_logged, MAX_LINE, 0);
				continue;
			}else{//not logged in
				char user[32], pass[8];

				//tokening username and password for authentication
				command = strtok(NULL, " ");
				strcpy(user, command);

				command = strtok(NULL, "\n");
				strcpy(pass, command);

				int flag = 0; //for determining if we have a successful login

				//now we compare the usernames, if we find a match we compare its password, if not we send message back to client
				for(int k = 0; k < MAX_USER; k++){
					if(strcmp(array[k].username, user) == 0){ //if we find a matching username
						if(strcmp(array[k].password, pass) == 0){ //we check that usernames password
							flag = 1; //set to 1 for success
							strcpy(client_name, user);
							k = MAX_USER; //exits outer for loop
						}
					}
				}
				if(flag == 1){ //successful login
					printf("%s login\n", client_name);
					command_flag = 1;
					send(client_socket, loginSuccess, MAX_LINE, 0);
				}else{
					send(client_socket, loginFail, MAX_LINE, 0);
				}
			}
		}
		else if(strcmp(command, "newuser") == 0){ //if new user
			if(command_flag == 1){
				send(client_socket, already_logged, MAX_LINE, 0);
				continue;
			}else{
				char username[32], password[8];

				//same as for login command
				command = strtok(NULL,  " ");
				strcpy(username, command);

				command = strtok(NULL, "\n");
				strcpy(password, command);

				int index = 0; //to ensure we dont overwrite array of users
				int flag = 0;

				//need to loop through our list and compare usernames
				for(int i = 0; i < MAX_USER; i++){
					if(strcmp(array[i].username, username) == 0){
						flag = 1;
					}else{
						index += 1;
					}
				}
				if(index > MAX_USER){ //if we cant fit anymore users, we throw error and continue
					send(client_socket, "Maximum number of accounts reached.", MAX_LINE, 0);
					continue;
				}
				if(flag == 1){ //if we find same username we send error to client and continue
					send(client_socket, newUserFail, MAX_LINE, 0);
					continue;
				}
				else if(flag == 0){
					//we now have looped through and compared usernames and did not find a match, we now write it to file

					FILE * fp = fopen("users.txt", "a");
					if(fp == NULL){
						printf("ERROR opening file");
					}
					fputs("\n(", fp);
					fputs(username, fp);
					fputs(", ", fp);
					fputs(password, fp);
					fputs(")", fp);

					fclose(fp);

					//send successful account creation to client
					send(client_socket, newUserSuccess, MAX_LINE, 0);
					
					printf("New user account created\n");
					command_flag = 0; //new users still must login, redundancy for safety
				}
			}
		}
		else if(strcmp(command, "send") == 0){ //if send
			if(command_flag == 0){
				send(client_socket, not_logged, MAX_LINE, 0);
				continue;
			}else{
				char response[MAX_LINE];
				strcpy(response, client_name);
				strcat(response, ":");

				char * strip = "send";
				
				int stringLen = strlen(bufCopy);
				int stripLen = strlen(strip);
				int i, j;
				int search = 0;

				//This function essentially strips the first word of the sentence so we can concatenate and echo the real message back
				for(i = 0; i < stringLen; i++){
					search = 1;
					for(j = 0; j < stripLen; j++){
						if(bufCopy[i+j] != strip[j]){
							search = 0;
							break;
						}
					}
					if(search == 1){
						for(j = i; j <= stringLen - stripLen; j++){
							bufCopy[j] = bufCopy[j+stripLen];
						}
						break;
					}
				}
				strcat(response, bufCopy);
				
				printf("%s", response);
				send(client_socket, response, MAX_LINE, 0);
			}
		}
		else if(strcmp(command, "logout\n") == 0){ //if logout
			if(command_flag == 0){
				send(client_socket, not_logged, MAX_LINE, 0);
				continue;
			}else{
				//creating logout message for client
				char message[MAX_LINE];
				strcpy(message, client_name);
				strcat(message, " left.");

				send(client_socket, message, MAX_LINE, 0);
			
				printf("%s logout\n", client_name);

				strcpy(client_name, "");

				//setting flags for appropriate usage
				command_flag = 0;
				disc_flag = 0;

				close(client_socket);

				//because the server must stay open we simple continue, reset our socket for new connections
				continue;
			}
		}

	}
	return 0;
}
//Called everytime the while loop iterates so that we constantly update and keep track of new users and information
User * loadUserData(char * filename){
	FILE * fp = fopen(filename, "r");
	if(fp == NULL){
		return 0;
	}
	User * array = malloc(MAX_USER * sizeof(User));
	if(array == NULL){
		return array;
	}else{
		for(int i = 0; i < MAX_USER; i++){
			fgetc(fp);
			fscanf(fp, "%s%s\n", array[i].username, array[i].password);
			array[i].username[strlen(array[i].username) - 1] = '\0';
			array[i].password[strlen(array[i].password) - 1] = '\0';
		}
	}
	fclose(fp);
	return array;
}
