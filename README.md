---
layout: default
title: "Overview"
nav_order: 1
description: "HCL Domino C API Samples"
has_children: true
---

# Domino C-API Samples
This repository is a collection of examples for using the HCL Domino C-API, it compliments the [HCL Domino C API documentation](https://opensource.hcltechsw.com/domino-c-api-docs/) and is open to external [contributions](CONTRIBUTING.md).

The HCL C API Toolkit for Domino and Notes contains many sample programs that show you how to use various aspects of the C API. The samples are divided into the following groups:

### Administration
Programs that show how to perform Domino system administration operations like controlling database access and checking server performance

### Advanced Server
Programs that illustrate how to use the C API to develop applications that manage clustered servers and databases and implement customized billing managers and add-in tasks

### Basic
Programs that show the basic structure of API code or basic operations using the C API
see [Basic](basic.md)

### Client
Programs that show the structure of client specific API code or operations using the C API

### Database Design
Programs that show how to create and read database design elements, including forms, views, navigators, agents, access control lists, and policy and help documents

### DB2
Programs that show the structure of DB2 specific API code

### Mail
Programs that show how to use the C API for electronic mail operations, such as sending and receiving mail and writing mail gateways

### Miscellaneous
Programs that show various C API operations not covered by the other samples

### Rich Text
Programs that show how to read and write elements of a rich text field

### Server
Programs that show the structure of HCL Domino Server add-in tasks and features of the HCL Domino Server environment
[ref]()
### Views
Programs that show how to use indexes (views)

### HTML
Programs that show how to use HTML functions

### CALAPI
Programs that show how to use iCalendar functions

### OS
Programs that show how to use C API for OS operations.

### TIME
Program that shows how to use time conversion operations.

The sample descriptions below indicate the platforms each sample supports. Some samples do not support all platforms, but each sample contains valuable code relevant to all platforms. HCL C API functions for Domino and Notes are largely the same on every platform. Frequently, only the makefile varies from platform to platform.

Note that the sample programs use the minimum stack sizes necessary for execution. Other applications may require larger stack sizes. 

## Usage
Each sample can be compiled on its own using the Domino C API.
For instructions refer to the [HCL Domino C API documentation](https://opensource.hcltechsw.com/domino-c-api-docs/). Furthermore the following Domino applications provide additional information and references:

- [HCL Domino C API User Guide](https://github.com/HCL-TECH-SOFTWARE/domino-c-api-docs/blob/main/apiug.nsf) (*.nsf / Domino application) 
- [HCL Domino C API Reference Guide](https://github.com/HCL-TECH-SOFTWARE/domino-c-api-docs/blob/main/apiref.nsf) (*.nsf / Domino application) 

## Supported environments
The project is supported on Windows 64bit, Windows 32bit, Linux 64bit, see detailed information about supported run-time and build environments refer [HCL Domino C API User Guide](https://github.com/HCL-TECH-SOFTWARE/domino-c-api-docs/blob/main/apiug.nsf) (*.nsf / Domino application).

## Issues
For issues relating to the code samples and scripts published in this repository, please refer to the [GitHub issue tracker](issues)

## Contributing
We welcome contributions following [our guidelines](CONTRIBUTING.md).

## Community Support
Special Thanks go to the following people for having provided valuable input to this project

* [Ulrich Krause](https://www.eknori.de/) for his long term contribution to the C API .

## License
The code examples and scripts in this repository are licensed under the [Apache License 2.0](LICENSE.txt). 

Note that HCL Domino and add-on products are commercial software - The software licenses agreement does not permit further distribution of the HCL CAPI samples.
