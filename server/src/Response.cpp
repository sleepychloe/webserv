/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yhwang <yhwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 12:13:24 by hyunah            #+#    #+#             */
/*   Updated: 2023/04/07 22:03:25 by yhwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"
#include <sys/wait.h>
#include <algorithm>

Response::Response() : cgiState(0)
{
	statusCodeDic.insert(std::pair<int, std::string>(200, "OK"));
	statusCodeDic.insert(std::pair<int, std::string>(204, "No Content"));
	statusCodeDic.insert(std::pair<int, std::string>(400, "Bad Request"));
	statusCodeDic.insert(std::pair<int, std::string>(403, "Forbidden"));
	statusCodeDic.insert(std::pair<int, std::string>(404, "Not Found"));
	statusCodeDic.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
	statusCodeDic.insert(std::pair<int, std::string>(413, "Content Too Large"));
	statusCodeDic.insert(std::pair<int, std::string>(500, "Internal Server Error"));
	statusCodeDic.insert(std::pair<int, std::string>(505, "HTTP Version Not Supported"));
    this->mimeMap.insert(std::make_pair("aac", "audio/aac"));
    this->mimeMap.insert(std::make_pair("abw", "application/x-abiword"));
    this->mimeMap.insert(std::make_pair("arc", "application/x-freearc"));
    this->mimeMap.insert(std::make_pair("avi", "video/x-msvideo"));
    this->mimeMap.insert(std::make_pair("bin", "application/octet-stream"));
    this->mimeMap.insert(std::make_pair("bmp", "image/bmp"));
    this->mimeMap.insert(std::make_pair("bz2", "application/x-bzip2"));
    this->mimeMap.insert(std::make_pair("csh", "application/x-csh"));
    this->mimeMap.insert(std::make_pair("css", "text/css"));
    this->mimeMap.insert(std::make_pair("csv", "text/csv"));
    this->mimeMap.insert(std::make_pair("doc", "application/msword"));
    this->mimeMap.insert(std::make_pair("gif", "image/gif"));
    this->mimeMap.insert(std::make_pair("htm", "text/html"));
    this->mimeMap.insert(std::make_pair("html", "text/html"));
    this->mimeMap.insert(std::make_pair("jpeg", "image/jpeg"));
    this->mimeMap.insert(std::make_pair("jpg", "image/jpg"));
    this->mimeMap.insert(std::make_pair("mpeg", "video/mpeg"));
    this->mimeMap.insert(std::make_pair("png", "image/png"));
    this->mimeMap.insert(std::make_pair("php", "application/x-httpd-php"));
    this->mimeMap.insert(std::make_pair("pdf", "application/pdf"));
    this->mimeMap.insert(std::make_pair("txt", "text/plain"));
}

Response::~Response(){}

std::string	Response::generateRawResponse(int code, MessageHeaders msg, std::string body){
	if (statusCodeDic[code].empty())
		std::cout << "There is no status available in Dictionnary for " << code <<" code " << intToString(code) << std::endl;
	std::string ret = ("HTTP/1.1 " + intToString(code) + " " + statusCodeDic[code] + "\r\n");
	if (!msg.hasHeader("Content-Length"))
		msg.addHeader("Content-Length", intToString(body.length()));
	else
	{
		if (msg.getHeaderValue("Content-Length") != intToString(body.length()))
			msg.setHeaderValue("Content-Length", intToString(body.length()));
	}
	ret += msg.generateRawMsg();
	ret += body;
	return (ret);
}

std::string	Response::getFormat(std::string & filepath)
{
	std::size_t	formatDelimitor;
	std::string	format;
	formatDelimitor = filepath.find_last_of(".");
	format = filepath.substr(formatDelimitor + 1);
	// std::cout << "format is : " << format << std::endl;
	return (format);
}

std::string	Response::getMimeType(std::string & filepath)
{
	std::string	format;
	format = getFormat(filepath);

	for(std::map<std::string, std::string>::iterator it = mimeMap.begin(); it != mimeMap.end(); ++it)
	{
		if (it->first == format)
		{
			// std::cout << "returning :" << it->second<< std::endl;
			return (it->second);
		}
	}
	return ("text/plain");
}

std::vector<char> Response::fileToBinary(std::string file_name1)
{
    std::vector<char> buffer;
    const char* file_name = file_name1.c_str();
    FILE* file_stream = fopen(file_name, "rb");
    size_t file_size;

	(void)file_size;
    if(file_stream != NULL)
    {
        fseek(file_stream, 0, SEEK_END);
        long file_length = ftell(file_stream);
        rewind(file_stream);

        buffer.resize(file_length);

        file_size = fread(&buffer[0], 1, file_length, file_stream);
    }
	fclose(file_stream);
	return (buffer);
}

#include <sys/types.h>
#include <dirent.h>

std::string	Response::getFileDateTime(time_t sec) {
	std::string	ret;
	char		buf[18];

	strftime(buf, sizeof(buf), "%d-%b-%Y %H:%M", localtime(&sec));
	ret += buf;

	return (ret);
}

std::vector<char>	Response::buildResponseforAutoIndex(Request* request, std::string filepath, int code){
	(void)data,
	(void)code;
	std::string 	ret;
	std::string 	uri_;
	DIR				*dir_ptr;
	struct dirent  	*dir_entry;
	struct stat		fileinfo;
	std::stringstream	ss;
	MessageHeaders	msg;

	uri_ = request->target.constructPath();
	if (!(*(uri_.rbegin()) == '/'))
		uri_.append("/");
	if (*(uri_.begin()) == '/')
		uri_.erase(0, 1);

	ret += "<!DOCTYPE html>\r\n";
	ret += "<html lang=\"en\">\r\n";
	ret += "<head>\r\n";
    ret += "<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">";
	ret += "<title>\r\n";
	ret += "Index of " + uri_ + "\r\n";
	ret += "</title>\r\n";
	ret += "</head>\r\n";
	ret += "<body>\r\n";
  	ret += "<h1>Index of " + uri_ + "</h1><hr><pre>\r\n";
  	ret += "<a href=\"../\">../</a>\r\n";// fix here

	dir_ptr = opendir(filepath.c_str()); //filepath to open directory
	if (dir_ptr != NULL)
	{
		while ((dir_entry = readdir(dir_ptr))) //get every file and directory info after opendir
		{
			if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0)
				continue;
			std::string filename = std::string(dir_entry->d_name);
			std::cout << "fileNAME IS :" << filepath + "/"+filename << std::endl;
			if (stat((filepath + "/"+filename).c_str(), &fileinfo) == 0)
			{
				if (S_ISDIR(fileinfo.st_mode))
				{
					filename += "/";
					ret += "\t\t<a href=\"" + uri_ + filename + "\">" + filename + "</a>";
				}
				else
					ret += "\t\t<a href=\"" + uri_ + filename + "\">" + filename + "</a>";
			}
			ret += std::string(30 - filename.size(), ' ');
			ret += getFileDateTime(fileinfo.st_mtime);
			std::string filesize;
			if (S_ISDIR(fileinfo.st_mode))
				filesize = "-";
			else
			{
				ss << fileinfo.st_size;
				filesize = ss.str();
				ss.str("");
			}
			ret += std::string(10, ' ');
			ret += filesize;
			ret += "\r\n";
		}
	}
	ret += "<hr></pre>\r\n";
	ret += "</body>\r\n";
	ret += "</html>\r\n";
	closedir(dir_ptr);
	std::cout << "Auto Index :\n" <<ret << std::endl;
	msg.addHeader("Content-Length", intToString(ret.size()));
	std::string go;
	go = ("HTTP/1.1 200 OK\r\n");
	go += msg.generateRawMsg();
	go += ret;
	data.insert(data.begin(), go.c_str(), go.c_str()+ go.size());
	return (data);
}

std::vector<char>	Response::buildResponseForRedirection(Server &server, Request * request, std::string dir, int code)
{
	std::string ret;
	MessageHeaders	msg;

	(void) dir;
	(void) code;
	ret = ("HTTP/1.1 301 Moved Permanently\r\n");
	msg.addHeader("Location", server.findMatchingUri(request->target.generateString()));
	ret += msg.generateRawMsg();
	data.insert(data.begin(), ret.c_str(), ret.c_str()+ ret.size());

	return (this->data);
}

std::vector<char>	Response::buildResponse(Server &server, Request * request, std::string dir, int code)
{
	MessageHeaders	msg;
	std::string		ret;
	std::string		txtBody;
	std::vector<char> null;

	(void) code;
	(void) server;
	
	// std::cout << "Is NOT Directory" << std::endl;
	(void) request;
	msg.addHeader("Date", generateDateHeader());
	msg.addHeader("Content-Type", getMimeType(dir));
	if (getMimeType(dir) == "text/plain" || getMimeType(dir) == "text/html")
	{
		// std::cout << "it is text or html" << std::endl;
		txtBody = check_filename_get_str(dir.c_str());
		if (txtBody.empty())
		{
			// std::cout << "Returning Null here" << std::endl;
			return (null);
		}
		data.insert(data.begin(), txtBody.c_str(), txtBody.c_str() + txtBody.size());
	}
	else
	{
		Uri	uri;
		uri.parsingFromString(dir);
		std::string fileDefaultFileName = "inline; filename=";
		fileDefaultFileName += "\"" + uri.getPath().back() + "\"";
		msg.addHeader("Content-Disposition", fileDefaultFileName.c_str());
		data = fileToBinary(dir);
	}
	std::cout << "data.size():"<<data.size()<< std::endl;
	// for (std::vector<char>::iterator it = data.begin(); it != data.end(); ++it)
	// {
	// 	std::cout << *it;
	// }
	msg.addHeader("Content-Length", intToString(data.size()));
	ret = ("HTTP/1.1 200 OK\r\n");
	ret += msg.generateRawMsg();
	data.insert(data.begin(), ret.c_str(), ret.c_str()+ ret.size());
	return (data);
}

std::string	Response::generateDateHeader()
{
	std::string weekdays[7] = {"Mon", "Tue", "Wed", "Thr", "Fri", "Sat", "Sun"};
	std::string months[13] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "July", "Aug", "Sep", "Oct", "Nov", "Dec"};
	time_t now = std::time(0);
	tm *ltm = localtime(&now);
	
	return(weekdays[ltm->tm_wday] + ", " + \
			intToString(ltm->tm_mday) + " " + \
			months[1 + ltm->tm_mon] + " " + \
			intToString(1900 + ltm->tm_year) + " " + \
			intToString(ltm->tm_hour) + ":" + \
			intToString(ltm->tm_min) + ":" + \
			intToString(ltm->tm_sec) + " " + \
			ltm->tm_zone);
}

std::vector<char>	Response::buildErrorResponse(std::string dir, int code)
{
	MessageHeaders	msg;
	std::string		ret;
	std::string		body;
	std::string		filePath;
	std::vector<char> null;

	if (statusCodeDic[code].empty())
		std::cout << "There is no status available in Dictionnary for code " << intToString(code) << std::endl;
	filePath = dir + "/"+ intToString(code) + ".html";
	std::cout << "FilePathName : " << filePath << std::endl;
	body = check_filename_get_str(filePath.c_str());
	if (body.empty())
		return (null);
	ret += "HTTP/1.1 " + intToString(code) + " " + statusCodeDic[code] + "\r\n";
	msg.addHeader("Date", generateDateHeader());
	msg.addHeader("Content-Type", "text/html");
	msg.addHeader("Content-Length", intToString(body.length()));
	ret += msg.generateRawMsg();
	ret += body;

	// make ret. return type to vector<char> and change the send function to see if the whole size of ficher+header has been transfered.
	data.insert(data.end(), ret.c_str(), ret.c_str()+ ret.size());
	
	return (data);
}

std::vector<char>	Response::buildResponseForCgi(std::vector<char> data, int code)
{
	MessageHeaders	msg;
	std::string		ret;
	std::string		txtBody;
	std::vector<char> null;

	(void) code;
	// std::cout << "In here : buildResponseForCgi\n";
	std::string str(data.begin(), data.end());
	msg.addHeader("Date", generateDateHeader());
	std::string bodyDeliminator = "\r\n\r\n";
	std::size_t i = vecFind(data, bodyDeliminator);
	std::string header(data.begin(), data.begin() + i + 2);
	std::cout << "this header : " << header << "\n";
	std::cout << "Body deliminator :  " << i << "\n";
	data.erase(data.begin(), data.begin() + i + 4);
	msg.addHeader("Content-Length", intToString(data.size()));
	ret = ("HTTP/1.1 200 OK\r\n");
	ret += header;
	ret += msg.generateRawMsg();
	data.insert(data.begin(), ret.c_str(), ret.c_str()+ ret.size());
	return (data);
}

std::vector<char>	Response::getMethod(Server &server, Request *request, std::size_t messageEnd){
	(void) messageEnd;

	// GET must have empty body, if not, Bad Request.
	if (!request->body.empty())
	{
		statusCode = 400;
		return (buildErrorResponse(server.error_page, 400));
	}

	if (!request->target.getQuery().empty())
	{
		server.cgiState = 1;
		Cgi cgi;
		cgi.analyse(&server, request);
		cgi.addEnvParam(request->target.getQuery());
		data = cgi.execute();

		data = buildResponseForCgi(data, 200);
		// printData(data);
		return (data);
	}
	std::string path = server.findMatchingUri(request->target.generateString());
	if (path.empty())
	{
		std::cout << server.error_page<< std::endl;
		return (buildErrorResponse(server.error_page, 404));	
	}
	if (path.find("http") != std::string::npos)
	{
		data = buildResponseForRedirection(server, request, path, 200);
		return (data);
	}
	//autoindex
	if (isDirectory(path.c_str()))
	{
	// std::cout << "Is Directory" << std::endl;
		// do auto index
		if (server.autoIndex)
			return (buildResponseforAutoIndex(request, path, 200));
		else
			return (buildErrorResponse(server.error_page, 403));
	}
	if (check_filename_get_str(path.c_str()).empty())
	{
		if (CheckPermission(path))
			return (buildErrorResponse(server.error_page, 403));
		return (buildErrorResponse(server.error_page, 404));
	}
	data = buildResponse(server, request, path, 200);
	statusCode = 200;
	if (data.empty())
	{
		statusCode = 400;
		return (buildErrorResponse(server.error_page, 400));	
	}
	return (data);
}

void	addEnv(std::vector<std::string> &env, std::string key, std::string value)
{
	env.push_back(key + "=" + value);
}

std::vector<char>	Response::postMethod(Server &server, Request *request, std::size_t messageEnd){
	(void) messageEnd;
	(void) statusCode;
	std::vector<char>	data;

	Cgi cgi;
	// printData(request->body);
	cgi.analyse(&server, request);
	// printData(request->body);
	if (!cgi.parsingFileBody(request->body, request->headers, server.maxClientBodySize, &statusCode))
	{
		statusCode = 413;
		return (buildErrorResponse(server.error_page, 413));
	}
	cgi.upload();
	data = cgi.execute();
	data = buildResponseForCgi(data, 200);
	return (data);
}

std::vector<char>	Response::deleteMethod(Server &server, Request *request, std::size_t messageEnd){
	(void) messageEnd;
	(void) statusCode;
	(void) server;
	MessageHeaders		msg;
	std::string			ret;
	std::string			body;
	std::vector<char>	data;
	int					code;

	msg.addHeader("Date", generateDateHeader());
	std::cout << *request << std::endl;
	std::string dir = server.root + request->target.generateString();
	std::cout << dir << std::endl;
	if (check_filename_get_str2(dir.c_str(), &code).empty())
	{
		if (!code)
		{
			msg.addHeader("Content-Type", "text/plain");
			body = "Status 404 : Not Found";
			ret = this->generateRawResponse(404, msg, body);
			data.insert(data.end(), ret.c_str(), ret.c_str()+ ret.size());
			// printData(data);
			return (data);
		}
	}
	std::remove(dir.c_str());
	msg.addHeader("Content-Type", "text/plain");
	body = "Status 200 : OK";
	ret = this->generateRawResponse(200, msg, body);
	data.insert(data.end(), ret.c_str(), ret.c_str()+ ret.size());
	// printData(data);
	return (data);
}
