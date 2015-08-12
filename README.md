# KFileMetaData

## Introduction

KFileMetaData provides a simple library for extracting the text and metadata
from a number of different files. This library is typically used by file
indexers to retreive the metadata.

## Using the library

In order to use the library you must implement your own `ExtractionResult`
class. Instances of this class will be passed to every applicable plugin and
they will populate with the information.

For convenience a `SimpleResult` class has been provided which stores all the
data in memory and allows it to be introspected later. Most clients *should*
implement thier own `ExtractionResult` as the data can get quite large when
extracting the text content from very large files.

## Extracting Metadata from a file

This requires us to create a `ExtractionPluginManger` class, fetch the extractor
plugins which are applicable for that file, and then pass the instance of
`ExtractionResult` to each Extractor.

A simple test example called `dump.cpp` has been written.

## Writing a custom file extractor

The Metadata is extracted with the help of Extraction Plugins. Each plugin
provides a list of mimetypes that it supports, and implements the extraction
function which extracts the data and fills it in an `ExtractionResult`.

Most of the common file types are already provided by the library.

Extractors should typically avoid implementing any logic themselves and should
just be wrappers on top of existing libraries.

### Adding data into an `ExtractionResult`

The ExtractionResult can be filled with (key, value) pairs and plain text. The
keys in these pairs typically correspond to a predefinied property. The list
of properties is defined in the `properties.h` header. Every plugin should
use the properties defined in this header. If a required property is missing
then it should be added to this framework.

The ExtractionResult should also be given a list of types. These types are
defined in the `types.h` header. The correspond to a higher level overview
of the files which the user typically expects.

## Links
- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-devel>
- IRC channel: #kde-devel on Freenode
