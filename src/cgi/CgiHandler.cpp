/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjacobs <bjacobs@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 18:26:05 by bjacobs           #+#    #+#             */
/*   Updated: 2024/05/22 16:03:57 by bjacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/CgiHandler.hpp"
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

static std::string	getQuery(const std::string &url) {
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

static int8_t	waitLimit(pid_t &pid, const int &timeLimit) {
	std::chrono::time_point		start = std::chrono::system_clock::now();
	std::chrono::duration<long>	elapsed;
	int							wstatus;
	pid_t						waitStatus;
	

	while (true) {
		waitStatus = waitpid(pid, &wstatus, WNOHANG);
		if (waitStatus == pid) {
			if (WEXITSTATUS(wstatus) != EXIT_SUCCESS || WIFSIGNALED(wstatus))
				return (-1);
			return (1);
		}
		if (waitStatus == -1) {
			kill(pid, SIGINT);
			waitpid(pid, NULL, 0);
			return (-1);
		}
		elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start);
		if (elapsed.count() >= timeLimit) {
			kill(pid, SIGINT);
			waitpid(pid, NULL, 0);
			return (0);
		}
	}
}

CgiHandler::CgiHandler(void) {}

CgiHandler::CgiHandler(const CgiHandler &source) {
	*this = source;
}

CgiHandler::~CgiHandler(void) {}

CgiHandler::CgiHandler(const t_httprequest &request, const t_route &route) {
	_init(request, route);
}

CgiHandler	&CgiHandler::operator=(const CgiHandler &rightSide) {
	if (this != &rightSide) {
		_body = rightSide._body;
		_env = rightSide._env;
		_output = rightSide._output;
	}
	return (*this);
}

void	CgiHandler::_init(const t_httprequest &request, const t_route &route) {
	std::string	path = parse_resource(request.url, route);
	std::string	hostname;
	std::string	port;

	_body = byteToString(request.body);
	if (request.head.find("Host") != request.head.end()) {
		hostname = request.head.at("Host");
		hostname.erase(hostname.find(":"));
		port = request.head.at("Host").substr(hostname.length() + 1);
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
	_env["QUERY_STRING"] = getQuery(request.url);
	if (request.head.find("Content-Type") != request.head.end())
		_env["CONTENT_TYPE"] = request.head.at("Content-Type");
	if (request.head.find("Content-Length") != request.head.end())
		_env["CONTENT_LENGTH"] = request.head.at("Content-Length");
	else
		_env["CONTENT_LENGTH"] = std::to_string(_body.length());
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

void	CgiHandler::_executeCgi(const int &inFD, const int &outFD) const {
	char		**envp;
	const char	*path = _env.at("PATH_INFO").c_str();
	char		*argv[2] = {const_cast<char*>(path), NULL};

	try {
		envp = _envToChar();
	}
	catch (const std::bad_alloc &e) {
		std::cerr << ERR_MSG_START << strerror(errno) << std::endl;
	}
	dup2(inFD, STDIN_FILENO);
	dup2(outFD, STDOUT_FILENO);
	close(inFD);
	close(outFD);
	execve(path, argv, envp);
	std::cerr << ERR_MSG_START << "execve: " << strerror(errno) << std::endl;
	_deleteCharEnv(envp);
	exit(EXIT_FAILURE);
}

int8_t	CgiHandler::startExecution(void) {
	int		inPipe[2], outPipe[2];
	int		waitStatus;
	pid_t	pid;

	if (pipe(inPipe))
		return (-1);
	if (pipe(outPipe)) {
		closePipe(inPipe);
		return (-1);
	}
	if (write(inPipe[1], _body.c_str(), _body.size()) == -1) {
		closePipe(inPipe);
		closePipe(outPipe);
		return (-1);
	}
	close(inPipe[1]);
	pid = fork();
	if (!pid) {
		close(outPipe[0]);
		_executeCgi(inPipe[0], outPipe[1]);
	}
	if (pid == -1) {
		close(inPipe[0]);
		closePipe(outPipe);
		return (-1);
	}
	close(inPipe[0]);
	close(outPipe[1]);
	waitStatus = waitLimit(pid, TIME_LIMIT);
	if (waitStatus != 1) {
		close(outPipe[0]);
		return (waitStatus);
	}
	_output = read_file(outPipe[0]);
	close(outPipe[0]);
	return (1);
}

void	CgiHandler::_deleteCharEnv(char **envp) const {
	for (size_t i = 0; envp[i]; ++i)
		delete[] envp[i];
	delete[] envp;
}

std::string	CgiHandler::getOutput(void) const {
	return (_output);
}
