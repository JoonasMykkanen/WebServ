#include "WebServerProg.hpp"
#include "api_helpers.hpp"
#include "constants.hpp"
#include "utils.hpp"
#include "Color.hpp"
#include <unistd.h>
#include <iomanip>//
#include <sstream>//

// TODO: MOVE TO HELPERS SINCE ITS DUPLICATE; WILL NOT WORK
static void	appendMisc(std::string& _res, size_t contentSize) {
	_res.append("Content-Type: text/html");
	_res.append(NEW_VALUE);
	_res.append("Content-length: " + std::to_string(contentSize));
	_res.append(END_HEADER);
}

std::string urlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        // Keep alphanumeric and other safe characters unchanged
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            // Any other characters are percent-encoded
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
            escaped << std::nouppercase;
        }
    }

    return escaped.str();
}

//TODO: make the css linked and correct the file path from these links to the root upload directory when its in
void WebServerProg::postResponse(int clientSocket) {
    clientData& client = accessClientData(clientSocket);
	
	if (client._status < ERRORS) {

		std::ofstream outFile("./uploads/" + client._fileName, std::ios::binary);
		if (!outFile)
			client._status = INT_ERROR;
		else {
			outFile << client._fileData;
			outFile.close();
			client._status = OK;
		}
	}
	client._response.clear();
    appendStatus(client._response, client._status);

	std::string css_content = R"(body {
        font-family: 'Arial', sans-serif;
        background-color: #f2f2f2;}h1 {
		color: rgb(0, 128, 128);
		display: inline-block; 
		margin-right: 10px; 
	}
    p {
        color: black;
        font-size: 40px;
    })";

	if (client._status == OK)
	{
        std::string html_content = "<!DOCTYPE html>\n<html>\n<head>\n<title>You Successfully uploaded!</title>\n</head><style>" + css_content + "</style>\n<body>\n<p>You Successfully uploaded! Click the link below to view your file.</p>\n";
		std::string filePath  = "uploads/" + urlEncode(client._fileName); 
		html_content += "<a href=\"" + filePath + "\" target=\"_blank\">View File</a>\n";
		html_content += "<form action=\"/delete\" method=\"post\">\n";
		html_content += "<input type=\"hidden\" name=\"file\" value=\"" + filePath+ "\">\n";
		html_content += "<input type=\"submit\" value=\"Delete File\">\n";	
		html_content += "</form>\n";
		html_content += "</body>\n</html>\r\n\r\n";
        appendMisc(client._response, html_content.size());
        client._response.append(html_content);
        client._response.append(html_content);
	}
	else
	{
		std::string html_content = "<!DOCTYPE html>\n<html>\n<head>\n<title>Error with upload!</title>\n</head>\n<body>Error with upload!\n</body>\n</html>\r\n\r\n";
		appendMisc(client._response, html_content.size());
		client._response.append(html_content);
	}
}