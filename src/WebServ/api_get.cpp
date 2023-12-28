#include "WebServerProg.hpp"
#include "api_helpers.hpp"
#include "Color.hpp"
#include "utils.hpp"
#include <unistd.h>

// Function to append status code to response based on what readFile() returned
void	appendStatus(std::string& _res, int status) {
	_res.append(HTTP_HEADER);
	_res.append(toString(status));
	switch (status) {
		case OK:
			_res.append(" OK");
			break;

		case NOT_FOUND:
			_res.append(" Not Found");
			break;

		case UNAUTHORIZED:
			_res.append(" Unauthorized");
			break;
		
		case FORBIDDEN:
			_res.append(" Forbidden");
			break;

		default:
			break;
	}
	_res.append(NEW_VALUE);
}

// Append content type, length and actual body
void	appendBody(std::string& _res, std::string& body, std::string const & path) {
	_res.append("Content-Length: ");
	_res.append(toString(body.size()));
 	_res.append(NEW_VALUE);
	_res.append("Content-type: ");
	std::string type = getFileExtension(path);
	if (type == EXT_HTML) {
		_res.append(TYPE_HTML);
	}
	else if (type == EXT_CSS) {
		_res.append(TYPE_CSS);
	}
	else if (type == EXT_PNG) {
		_res.append(TYPE_PNG);
	}
	_res.append(END_HEADER);
	_res.append(body);
}

// Check permissions and adjust status accordingly
void	checkRequest(int* status, std::string const & path)
{
	std::ifstream	file(path.c_str());
	if (file.is_open())
	{
		if (file.good())
			*status = OK;
		else
			*status = NOT_FOUND;
	}
	else
	{	
		std::cerr << COLOR_RED << "Error opening file: " << path << COLOR_RESET << std::endl;
		*status = NOT_FOUND;
	}
}

void	WebServerProg::getResponse(int clientSocket) {
	std::string body;
	std::string	path;
	int	status = 200;

	path = accessDataInMap(clientSocket, "Path");
	checkRequest(&status, path);
	if (status >= ERRORS) {
		char buffer[1024] = {};
		path = chooseErrorPage(status);
		path = getcwd(buffer, sizeof(buffer)) + path;
	}
	body = readFile(path);

	appendStatus(_response, status);
	appendBody(_response, body, path);
}
