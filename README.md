# G-Unicode
<p align="center">
  <img width="346" height="115" src="images/g-unicode-logo.png">
</p>
<p align="center">
A cross-platform LabVIEW library for manipulating unicode strings, and performing file I/O on unicode paths.
</p>

<p align="center">
    <a href="#whats-new">What's New?</a> -
    <a href="#features">Features</a> -
    <a href="#installation">Installation</a> -
    <a href="#usage">Usage</a> -
	<a href="#building">Building</a> -
	<a href="#comparison">Comparison</a> - 
	<a href="#license">License</a> - 
	<a href="#acknowledgments">Acknowledgments</a>
</p>

## <a id="whats-new"></a>What's New?
* Library is currently under development, with the first release coming soon

## <a id="features"></a>Features
* New classes: _UTF-8 String_ and _UTF-8 Path_
* Drop-in unicode replacements for string, file I/O, and user dialog functions
* Unicode path support
    * Read/write filenames and paths which contain unicode - no more _?????_ in filenames
    * Native system file dialog which returns unicode paths
	* File references opened from unicode paths compatible with LabVIEW's File I/O functions
* New VIs accept both standard LabVIEW strings/paths and UTF-8 String/Path class inputs
* Custom class probes allows viewing unicode text when debugging
* Custom QuickDrop plugin allows viewing and editing UTF-8 String/Path constant values

![G-Unicode feature overview](images/g-unicode-demo-1.png?raw=true "G-Unicode feature overview")

![G-Unicode custom probe](images/g-unicode-demo-2.png?raw=true "G-Unicode custom probe")

## <a id="installation"></a>Installation
TBD

## <a id="usage"></a>Usage
### Types & Naming Conventions
G-Unicode introduces two classes for handling unicode text: _UTF-8 String.lvclass_ and _UTF-8 Path.lvclass_. These classes are containers around LabVIEW's standard string and path types, adding a custom wire appearance and unicode capable probes.

Type         | Wire Appearance
-------------|----------------
UTF-8 String | ![UTF-8 String wire](images/wire-utf8-string.png?raw=true "UTF-8 String wire")
UTF-8 Path   | ![UTF-8 Path wire](images/wire-utf8-path.png?raw=true "UTF-8 Path wire")

Most functions in G-Unicode are designed to accept both the new UTF-8 classes and regular string/path types as inputs. When a string/path is wired to a function which supports unicode, it will first be encoded as UTF-8.

### Using Unicode Paths With LabVIEW Functions
The File I/O functions included with G-Unicode cover most common file operations performed in LabVIEW. To use unicode paths with other LabVIEW functions which have a standard path input, first convert the unicode path to a short 8.3 format path with `Path to Short Name.vi`. Note that the unicode path must exist on disk for this function to work. If it doesn't exist, it can be created using `Create File.vim`.

In the example below the unicode path `c:\LabVIEW Data\안녕하세요.tdms` is created, then the short name retrieved and passed to LabVIEW's TDMS functions. After the file is closed, the short name is converted back to the full unicode path.

![Unicode path with TDMS functions](images/g-unicode-tdms-example.png?raw=true "Unicode path with TDMS functions")

There are caveats to this approach (the file must exist, ReFS file system isn't supported, TDMS index files will have the short path name, etc) so proceed with caution!

### Examples
See the example VIs in [Examples](src/LabVIEW/G-Unicode/Examples).

Unit tests are included and can be run individually, or with the [AST Unit Tester](https://www.autosofttech.net/documents/ast-unit-tester).

## <a id="building"></a>Building
TBD

## <a id="comparison"></a>Comparison
TBD

## <a id="license"></a>License
TBD

## <a id="acknowledgments"></a>Acknowledgments
This library uses the following open source code. Massive thanks to the authors of these libraries.

Library | License
--------|----------------------
[utf8.h](https://github.com/sheredom/utf8.h) | [Unlicense](https://github.com/sheredom/utf8.h/blob/master/LICENSE)
[Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs) | [WTFPL](https://github.com/samhocevar/portable-file-dialogs/blob/main/COPYING)
[TinyDir](https://github.com/cxong/tinydir) | [Simplified BSD](https://github.com/cxong/tinydir/blob/master/COPYING)

