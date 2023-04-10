/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyunah <hyunah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:54:24 by hyunah            #+#    #+#             */
/*   Updated: 2023/03/30 15:54:37 by hyunah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Cgi.hpp"

Cgi::Cgi(){
    this->cmdMap.insert(std::make_pair(".php", "/usr/bin/php-cgi"));
	this->file.tmpLoc = "./data/upload";
}

Cgi::~Cgi(){}

Cgi::Cgi(Cgi & src){
	*this = src;
}

Cgi & Cgi::operator=(Cgi & rhs){
	if (this != &rhs)
	{
		this->cmd = rhs.cmd;
		this->scriptPath = rhs.scriptPath;
	}
	return (*this);
}

std::string Cgi::getScriptPath() const {return (this->scriptPath);}
std::string	Cgi::getCmd() const {return (this->cmd);}

void	Cgi::addEnvParam(std::string key, std::string value){this->env.push_back(key + "=" + value);}
void	Cgi::addEnvParam(std::string str){this->env.push_back(str);}

int	Cgi::analyse(Server *server, Request *request){

	//scriptPath - TODO : gotta check if exist
	this->scriptPath = server->root + server->cgiBloc.cgiScriptPath + "/" + request->target.getPath().back();
	if (check_filename_get_str(this->scriptPath.c_str()).empty())
	{
		std::cout << "cgi script php file error \n";
		return (404);
	}
	this->file.tmpLoc = server->uploadPath;
	if (!isDirectory(this->file.tmpLoc.c_str()))
	{
		std::cout << "cgi file tmpLoc error \n";
		return (404);
	}
	// std::cout << "this->file.tmpLoc:" << this->file.tmpLoc<< std::endl;

	//cmd
	for (std::map<std::string, std::string>::iterator it = this->cmdMap.begin();\
		it != this->cmdMap.end(); ++it)
	{
		if (request->target.getPath().back().find(it->first) != std::string::npos)
			this->cmd = it->second;
	}

	//env
	addEnvParam("AUTH_TYPE", "Basic");
	// addEnvParam(env, "CONTENT_LENGTH", headers.getHeaderValue("Content-Length"));
	addEnvParam("CONTENT_TYPE", request->headers.getHeaderValue("Content-Type"));
	addEnvParam("GATEWAY_INTERFACE", "CGI/1.1");
	addEnvParam("SCRIPT_NAME", "upload.php");
	addEnvParam("SCRIPT_FILENAME", this->scriptPath);
	addEnvParam("REDIRECT_STATUS", "200");
	addEnvParam("QUERY_STRING", request->target.getQuery());
	addEnvParam("REMOTE_ADDR", server->host + ":" + intToString(server->port));
	addEnvParam("SERVER_NAME", server->host);
	addEnvParam("SERVER_PORT", intToString(server->port));
	addEnvParam("REQUEST_METHOD", request->method);
	addEnvParam("REQUEST_URI", request->target.generateString());
	addEnvParam("SERVER_PROTOCOL", "HTTP/1.1");
	return (0);
}

char	**Cgi::getPathArray(){

	char	**path = (char **)calloc(sizeof(char *), 2 + 1);
	path[0] = strdup(this->cmd.c_str());
	path[1] = strdup(this->scriptPath.c_str());
	// std::cout << "scriptPath" << this << std::endl;
	path[2] = NULL;

	return (path);	
}

char	**Cgi::getEnvArray(){

	char	**newEnv = (char **)calloc(sizeof(char *), env.size() + 1);
	int i = 0;

	for (std::vector<std::string>::iterator it = env.begin(); it != env.end(); ++it)
	{
		newEnv[i] = strdup(it->c_str());
		// std::cout << i << ". " << it->c_str() << std::endl;
		i++;
	}
	return (newEnv);
}

bool	Cgi::parsingFileBody(std::vector<char> data, MessageHeaders headers, int maxClientBodySize, unsigned int *errorCode)
{
	std::vector<char>			filebody;
	
	// find body deliminator
	filebody = data;
	std::string deliminator = "boundary=";
	std::size_t b = headers.getHeaderValue("Content-Type").find(deliminator);
	std::string nextline = "\r\n";
	std::string bodyDeliminator = headers.getHeaderValue("Content-Type").substr(b + deliminator.length());
	std::size_t a = vecFind(data, bodyDeliminator);

	// erase start boundary
	filebody.erase(filebody.begin(), filebody.begin() + a + bodyDeliminator.length() + nextline.length());

	// erase end boundary		
	a = vecFind(filebody, bodyDeliminator);
	filebody.erase(filebody.begin() + a - (nextline.length() * 2), filebody.end());
	
	// split fileinfo and body
	std::vector<char>	infoFile;
	a = vecFind(filebody, "\r\n\r\n");
	infoFile.insert(infoFile.begin(), filebody.begin(), filebody.begin() + a);

	// printData(infoFile);
	filebody.erase(filebody.begin(), filebody.begin() + a + 4);


	this->file.data = filebody;
	
	// Put variable, filename in env.
	std::string infoFileHeader;

	infoFileHeader.insert(infoFileHeader.begin(), infoFile.begin(), infoFile.end());
	std::cout <<"infoFileHeader" <<infoFileHeader<<std::endl;
	if (!this->file.headers.parseFromString(infoFileHeader))
	{
		std::cout << "No extension or body size 0" << std::endl;
		// return (false);
	}
	std::string rest = this->file.headers.getHeaderValue("Content-Disposition");
	this->file.type = this->file.headers.getHeaderValue("Content-Type");
	std::cout << "Content-Disposition" << rest << std::endl;
	std::cout << "Content-Type" << this->file.type << std::endl;
	size_t j;
	size_t k;
	std::string value;
	while ((j = rest.find(" ")) != std::string::npos)
	{
		value = rest.substr(0, j);
		if ((k = value.find("=")) != std::string::npos)
		{
			size_t l = value.find("\"", k + 2);
			this->file.var = value.substr(k + 2, value.size() - (k + 2) - (value.size() - l));
			std::cout <<value.substr(0, k) << " " << value.substr(k + 2, value.size() - (k + 2) - (value.size() - l)) << std::endl;
			this->addEnvParam(value.substr(0, k), this->file.var);
		}
		rest.erase(rest.begin(), rest.begin() + j + 1);
	}
	if ((k = rest.find("=")) != std::string::npos)
	{
		size_t l = rest.find("\"", k + 2);
		this->file.name = rest.substr(k + 2, value.size() - (k + 2) - (value.size() - l));
	}
	this->addEnvParam("FILE_TEMPLOC", file.tmpLoc);
	// Add type and size and time.
	
	//for error 413
	if (filebody.size() > (long unsigned int)maxClientBodySize)
	{
		// std::cout << filebody.size() << " > " << maxClientBodySize << std::endl;
		*errorCode = 413;
		return (false);
	}
	std::cout << "5\n";
	return (true);
}

bool	Cgi::upload(){

	// if (this->file.name.empty() || this->file.data.empty())
	if (this->file.name.empty())
		return (this->addEnvParam("UPLOAD_ERROR", intToString(1)), false);

	std::ifstream tmp;
	std::string	nameTmp;
	// CHECK IF THERE IS ALREADY THE SAME NAME FILE EXIST
	std::string patch = this->file.tmpLoc + "/" + this->file.name;
	tmp.open(patch.c_str());
	int i = 0;
	while (tmp)
	{
		tmp.close();
		std::string	inc = "(" + intToString(++i) + ")";
		std::size_t delim = this->file.name.find(".");
		if(delim == std::string::npos)
		{
			nameTmp = this->file.name + inc;
			patch = this->file.tmpLoc + "/" + nameTmp;
		}
		else
		{
			nameTmp = this->file.name.substr(0, delim) + inc + this->file.name.substr(delim);
			patch = this->file.tmpLoc + "/" + nameTmp;
		}
		tmp.open(patch.c_str());
	}
	this->file.name = nameTmp;
	this->addEnvParam("filename", this->file.name);

	std::ofstream myFile(patch.c_str());
	for (std::vector<char>::iterator it = this->file.data.begin(); it != this->file.data.end(); ++it)
		myFile << *it;
	myFile.close();
	std::cout << "\t4" << std::endl;
	this->addEnvParam("UPLOAD_ERROR", intToString(0));
	std::cout << "\t5" << std::endl;
	return (true);
}

#include <sys/wait.h>

std::vector<char>	Cgi::execute(){
	std::vector<char>	data;

	std::cout << "CGI Execution\n";
	int pipefd[2];
	if (pipe(pipefd) == -1)
		printf("Error in opening pipe\n");

	int id = fork();
	if (id == 0)
	{
		std::cout << "this->getCmd().c_str():"<< this->getCmd().c_str()<<std::endl;
		close(pipefd[0]);    // close reading end in the child
		dup2(pipefd[1], 1);  // send stdout to the pipe
		// dup2(pipefd[1], 2);  // send stderr to the pipe
		close(pipefd[1]);    // this descriptor is no longer needed
		execve(this->getCmd().c_str(), this->getPathArray(), this->getEnvArray());
	}
	else
	{
		// parent
		char buffer[1];
		int n;

		close(pipefd[1]);  // close the write end of the pipe in the parent
		while ((n = read(pipefd[0], buffer, 1)) > 0)
		{
			if (n < 0)
				printf("read failed\n");
			data.insert(data.end(), buffer, buffer + n);
			bzero(buffer, sizeof(buffer));
		}
		close(pipefd[0]);
		// printData(data);
		// printf("Got from child Process\n");
		// for (std::vector<char>::iterator it = data.begin(); it != data.end(); ++it)
		// {
		// 	std::cout << *it;
		// }
		// std::cout << std::endl;
	}
	wait(NULL);
	return (data);
}
