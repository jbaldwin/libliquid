libturbohttp - C++17 HTTP/1.1 Request and Response stateful parser.
===================================================================

[![CI](https://github.com/jbaldwin/libturbohttp/workflows/build/badge.svg)](https://github.com/jbaldwin/libturbohttp/workflows/build/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/jbaldwin/libturbohttp/badge.svg?branch=master)](https://coveralls.io/github/jbaldwin/libturbohttp?branch=master)
[![language][badge.language]][language]
[![license][badge.license]][license]

[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow.svg
[badge.license]: https://img.shields.io/badge/license-Apache--2.0-blue

[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/Apache_License

**libturbohttp** is a C++17 HTTP/1.1 Request and Response stateful parser with zero memory allocations.  The intended purpose of this library is to provide a high quality and extremely fast stateful HTTP/1.1 parser with modern C++ coding standards.

**libturbohttp** is licensed under the Apache 2.0 license.

# Overview #
* Modern C++17 API
* Stateful parser, continue parsing where the previous call left off at when partial requests and responses are provided.
* Zero allocation parsing, The request and response objects can be created on the stack and do not allocate any memory when parsing.
* Custom maximum number of headers for request and response objects, default is 16.

# Usage #
TODO

## Examples
TODO

## Requirements
TODO

## Instructions

### Building
TODO

### CMake Projects
TODO

### Running Tests
TODO

## Benchmarks
TODO

## Support

File bug reports, feature requests and questions using [GitHub Issues](https://github.com/jbaldwin/libturbohttp/issues)

Copyright © 2018-2020, Josh Baldwin