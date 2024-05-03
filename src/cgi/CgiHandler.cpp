/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:26:05 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/03 19:46:42 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>

static void	closePipe(int *pipe) {
	if (pipe[0] != -1) {
		close(pipe[0]);
		pipe[0] = -1;
	}
	if (pipe[1] != -1) {
		close(pipe[1]);
		pipe[1] = -1;
	}
}

static std::string	method_to_str(const HTTPMethod &method)
{
	if (method == HTTP_GET)
		return ("GET");
	if (method == HTTP_POST)
		return ("POST");
	if (method == HTTP_DELETE)
		return ("DELETE");
	return ("UNKNOWN METHOD");
}

static std::string	get_query(const std::string &url) {
	size_t	query_pos = url.find("?");

	if (query_pos == std::string::npos)
		return (std::string());
	return (url.substr(query_pos + 1));
}

static std::string	byteToString(const std::vector<std::byte> &body) {
	std::string	s;

	for (const std::byte b : body)
		s += (char)b;
	return (s);
}


CgiHandler::CgiHandler(void) {}

CgiHandler::CgiHandler(const t_httprequest &request, const t_route &route, const std::vector<std::byte> &body) {
	_body = byteToString(body);
	//std::cout << "body: " << _body << std::endl;
	_init(request, route);
}

void	CgiHandler::_init(const t_httprequest &request, const t_route &route) {
	std::string	path = parse_resource(request.url, route);
	std::string	hostname;
	std::string	port;

	if (request.head.find("Host") != request.head.end()) {
		hostname = request.head.at("Host");
		hostname.erase(hostname.find(":"));
		port = request.head.at("Host").substr(hostname.length());
	}
	_env["SERVER_SOFTWARE"] = "webserv/1.0";
	_env["SERVER_NAME"] = hostname;
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["SERVER_PROTOCOL"] = HTTP_VERSION;
	_env["SERVER_PORT"] = port;
	_env["REQUEST_METHOD"] = method_to_str(request.method);
	_env["PATH_INFO"] = path;
	_env["PATH_TRANSLATED"] = path;
	_env["SCRIPT_NAME"] = path;
	_env["QUERY_STRING"] = get_query(request.url);
	if (request.head.find("Content-Type") != request.head.end())
		_env["CONTENT_TYPE"] = request.head.at("Content-Type");
	if (request.head.find("Content-Length") != request.head.end())
	_env["CONTENT_LENGTH"] = request.head.at("Content-Length");
}

char	**CgiHandler::_envToChar(void) const {
	char	**envp = new char*[_env.size() + 1];
	size_t	i = 0;

	for (const std::pair<const std::string, std::string> &kv : _env) {
		std::string	element(kv.first + "=" + kv.second);
		envp[i] = new char[element.size() + 1];
		std::strncpy(envp[i], element.c_str(), element.size() + 1);
		++i;
	}
	envp[i] = NULL;
	return (envp);
}

void	CgiHandler::_execute_cgi(const int &inFD, const int &outFD) const {
	char		**envp;
	const char	*path = _env.at("PATH_INFO").c_str();
	char		*argv[2] = {const_cast<char*>(path), NULL};

	try {
		envp = _envToChar();
	}
	catch (const std::bad_alloc &e) {
		std::cerr << ERR_MSG_START << "bad malloc" << std::endl;
	}
	dup2(inFD, STDIN_FILENO);
	dup2(outFD, STDOUT_FILENO);
	close(inFD);
	close(outFD);
	execve(path, argv, envp);
	std::cerr << ERR_MSG_START << "execve failure" << std::endl;
	_deleteCharEnv(envp);
	exit(EXIT_FAILURE);
}

bool	CgiHandler::startExecution(void) {
	int		inPipe[2], outPipe[2];
	pid_t	pid;

	if (pipe(inPipe))
		return (false);
	if (pipe(outPipe)) {
		closePipe(inPipe);
		return (false);
	}
	write(inPipe[1], _body.c_str(), _body.size());
	close(inPipe[1]);
	pid = fork();
	if (!pid) {
		close(outPipe[0]);
		_execute_cgi(inPipe[0], outPipe[1]);
	}
	if (pid == -1) {
		close(inPipe[0]);
		closePipe(outPipe);
		return (false);
	}
	close(inPipe[0]);
	close(outPipe[1]);
	waitpid(pid, NULL, 0);
	_output = read_file(outPipe[0]);
	close(outPipe[0]);
	return (true);
}

void	CgiHandler::_deleteCharEnv(char **envp) const {
	for (size_t i = 0; envp[i]; ++i)
		delete[] envp[i];
	delete[] envp;
}

std::string	CgiHandler::getOutput(void) const {
	return (_output);
}
