# webserv

Mandatory part without any bonus

Success 96/100


## Installation

```bash
  git clone https://github.com/sleepychloe/webserv.git
  make
```

## Usage

```bash
  ./webserv
  ./webserv [config_file]
```

## Config file rules
```
 rule of our config file

  * http block: should be exist only once
	- keyword "root" should be exist only once, in the http block
	- keyword "autoindex" should be exist only once, in the http block
	- keyword "default_error_page" should be exist only once, in the http block
  * server block: should be exist at least once, in the http block
	- keyword "listen" should be exist only once, in the each server block
	- keyword "host" should be exist only once, in the each server block
	- keyword "client_max_body_size" should be exist only once, in the each server block
	- keyword "index" should be exist only once , in the each server block
	- keyword "allow_methods" should be exist only once, in the server block.
		: you can put "GET", "POST", "DELETE" or "UNKNOWN" here(seperated by space or tab)
	- keyword "save_path" should be exist only once, in the each server block
  * location block: cgi-bin location block should be exist only once in the each server block
		(it depends on you that using redirection location block and index location block)
	- redirection location block
	   : keyword "return" should be exist only once, in the each redirection location block
	- index location block
	   : keyword "index" should be exist only once, in the each index location block
	- cgi-bin location block
	   : keyword "cgi_path" should be exist only once, in the each cgi-bin location block
	     keyword "cgi_extention" should be exist only once, in the each cgi-bin location block

```
## Authors

- parsing part: [@sleepychloe](https://github.com/sleepychloe)
- executing part: [@jha0517](https://github.com/jha0517)
