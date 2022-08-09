# the Command-Line LibreTranslator

## Introduction

`cllt` is a command-line tool for translating text by using the [LibreTranslate API](https://github.com/LibreTranslate/LibreTranslate).

## Building

`cllt` depends on [curl](https://github.com/curl/curl) and [cJSON](https://github.com/DaveGamble/cJSON). Make sure these libraries are installed. Then use `make` to build `cllt`.

```
# build in debug mode
$ make

# build in release mode
$ make release
```

## Configuration

By default, `cllt` connects to the public instance at [libretranslate.de](https://libretranslate.de/).
But you can change the host or provide an API key by modifing `src/config.h`.

## Usage

```
Usage:
  cllt [OPTIONS] [TEXT]

Options:
  -h        : show help
  -l        : list all supported languages
  -s <code> : the source language code
  -t <code> : the target language code
  -f <file> : the file to be translated
              if TEXT is given, this option will be ignored
  -o <file> : the output filename of the translated file
              if this option is omitted, only a url to the
              translated file will be printed
```

## Examples

```
# detect the language of "Ciao!"
$ cllt "Ciao!"

# list all supported languages
$ cllt -l

# translate "Hello world!" from english to japanese
$ cllt -s en -t ja "Hello world!"

# translate the file named "text_en" and output to a file named "text_ja"
$ cllt -s en -t ja -f text_en -o text_ja
```
