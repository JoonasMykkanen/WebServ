# Webserv: Lightweight HTTP Server

## Introduction

Webserv is a lightweight and non-blocking HTTP server designed to meet specific requirements outlined in the project specifications. The server supports essential HTTP methods such as GET, POST, and DELETE, and it can handle static websites, file uploads, and CGI execution. The goal is to provide a resilient and efficient web server that adheres to the specified guidelines.

## Features

- **Configuration File**: Webserv takes a configuration file as an argument or uses a default path to configure various server parameters, including ports, hosts, server names, error pages, and more.

- **Non-blocking I/O**: The server employs the `poll()` system call (or equivalent) to handle read and write operations simultaneously, ensuring non-blocking communication with clients.

- **HTTP 1.1 Compliance**: Webserv is designed to be compatible with HTTP 1.1 standards, making it comparable to popular web servers like NGINX.

- **Multiple Ports and Hosts**: The configuration file allows users to specify multiple ports and hosts, with the first server for a given host:port acting as the default.

- **Dynamic Routing Configuration**: Users can set up routes with specific rules, including accepted HTTP methods, HTTP redirection, directory/file configurations, directory listing toggling, default files for directories, and CGI execution based on file extensions.

- **Error Handling**: The server provides accurate HTTP response status codes and default error pages if none are provided.

- **File Uploads**: Clients can upload files, and the server allows configuration of where these files should be saved.

- **CGI Execution**: The server can execute CGI scripts based on certain file extensions, providing dynamic content generation.

## Getting Started

**Build the Server**: `make`
**Run the Server**: `./webserv [config_file]` (use a configuration file or the default path)

## Configuration File Example

```plaintext
# webserv.conf

server {
    listen 8080;
    server_name localhost;

    location / {
        root /path/to/static/files;
        index index.html;
        allow_methods GET POST DELETE;
        upload_dir /path/to/uploaded/files;
        cgi_extension .php;
        enable_directory_listing on;
    }

    location /api {
        allow_methods GET POST;
        redirect /api /new_api;
    }
}
```

## Comparison with NGINX

To understand the behavior and features of Webserv, compare its behavior with NGINX, especially in areas like `server_name` handling.

## Contributors

- [Sheree Morphett](https://github.com/ShereeMorphett)
- [Joonas Mykkänen](https://github.com/JoonasMykkanen)
- [Tuomo Talvenheimo](https://github.com/TTalvenH)
