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
[tiny file dialogs](https://sourceforge.net/projects/tinyfiledialogs/) | zlib
[TinyDir](https://github.com/cxong/tinydir) | [Simplified BSD](https://github.com/cxong/tinydir/blob/master/COPYING)

