# G-Unicode
<p align="center">
  <img width="346" height="115" src="images/g-unicode-logo.png">
</p>
<p align="center">
A LabVIEW library for manipulating unicode strings, and performing file I/O on unicode paths.
</p>

<p align="center">
    <a href="#whats-new">What's New?</a> -
    <a href="#motivation">Motivation</a> -
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

## <a id="motivation"></a>Motivation
LabVIEW doesn't provide official unicode support under Windows, but through a combination of hidden control properties, byte order marks, and good luck, it is possible to read and display unicode strings. Unfortunately there are still many gaps in functionality:
* File I/O functions don't support unicode, and any unicode characters in paths are replaced with `?` rendering them invalid
* String operations aren't unicode aware

There is a UTF-8 locale setting in Windows, and ability to embed a UTF-8 application manifest to a compiled LabVIEW app (both available since Windows 10 v1903), which brings another level of unicode support to LabVIEW. This still requires special string handling and encoding for display.

G-Unicode seeks to streamline unicode handling in LabVIEW, making it simpler to develop applications with unicode and multilingual support.

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
**IMPORTANT:** LabVIEW has a [UseUnicode](https://labviewwiki.org/wiki/LabVIEW_configuration_file/Miscellaneous#useUnicode) ini token to help aid in handling unicode input. It enables right-click options for unicode display, and enables pasting unicode text from external applications. This is useful, but can also make dealing with string controls and constants problematic, silently mixing UTF-16LE and ANSI encodings when pasting or typing text. It is recommended to disable this token during development, though can be enabled in built applications. _G-Unicode does not require this token._

**IMPORTANT II:** G-Unicode relies on LabVIEW's experimental unicode support for unicode string input. This unicode string input is _**very buggy**_, and prone to strange input behavior and outright crashes. Where possible it's recommended to avoid using LabVIEW's string controls for unicode input.

### New Types
G-Unicode use two classes for handling unicode text: _UTF-8 String.lvclass_ and _UTF-8 Path.lvclass_. These classes are containers around LabVIEW's standard string and path types (encoded as UTF-8), adding a custom wire appearance and unicode capable probes.

Type         | Wire Appearance
-------------|----------------
UTF-8 String | ![UTF-8 String wire](images/wire-utf8-string.png?raw=true "UTF-8 String wire")
UTF-8 Path   | ![UTF-8 Path wire](images/wire-utf8-path.png?raw=true "UTF-8 Path wire")

Most functions in G-Unicode support wiring either a UTF-8 String/Path class, a standard LabVIEW string/path, or a combination of both (as in `Concatenate Strings.vim`). When a standard LabVIEW type is wired to a G-Unicode function it is internally converted to UTF-8 using LabVIEW's built-in `Text to UTF-8` primitive, and any return values are in the respective UTF-8 class.

### Error Wiring
There are two types of error wiring conventions used throughout G-Unicode. The first is the standard error in/error out clusters wired to the lower left and right terminals respectively. It is recommended to always wire these terminals.

The second convention is the error in/error out clusters wired to the bottom two terminals in the center of the connector pane. These error terminals only need to be wired when a standard LabVIEW string or path is wired to a function (and subject to the internal UTF-8 conversion mentioned above). If only UTF-8 String/Path(s) are wired to the function, these terminals will never contain error information and so can be ignored.

![Example G-Unicode error wiring](images/g-unicode-error-wiring.png?raw=true "Example G-Unicode error wiring")

### Using Unicode Paths With LabVIEW Functions
The File I/O functions included with G-Unicode cover most common file operations performed in LabVIEW. To use unicode paths with other LabVIEW functions which have a standard path input, first convert the unicode path to a short 8.3 format path with `Path to Short Name.vi`. Note that the unicode path must exist on disk for this function to work. If it doesn't exist, it can be created using `Create File.vim`.

In the example below the unicode path `c:\LabVIEW Data\안녕하세요.tdms` is created, then the short name retrieved and passed to LabVIEW's TDMS functions. After the file is closed, the short name is converted back to the full unicode path.

![Unicode path with TDMS functions](images/g-unicode-tdms-example.png?raw=true "Unicode path with TDMS functions")

There are caveats to this approach (the file must exist, ReFS file system isn't supported, TDMS index files will have the short path name, etc) so proceed with caution!

### Tips On Displaying Unicode
LabVIEW's unicode support is experimental, and it can be very easy to run into issues. If unicode text doesn't display correctly (it has the characteristic double-spacing or is completely wrong), check the table below for tips on how to set unicode text.

If there's a check mark in the _Copy + Paste Unicode_ column, the target text will need to have some unicode text copied and pasted into the text field first to force the text to display as unicode. The unicode text must be copied from a LabVIEW string displaying unicode.

**NOTE:** The _Stacked_ text orientation doesn't handle combining characters, diacritics, etc

Text Item            | Method                                               | Copy + Paste Unicode
---------------------|------------------------------------------------------|---------------------
Caption              | `Write Control Caption.vi` with scalar UTF-8 String  |
String               | `Write Control Text.vi` with scalar UTF-8 String     |
String Array (1D)    | `Write Control Text.vi` with 1D UTF-8 String array   |
String Array (2D)    | `Write Control Text.vi` with 2D UTF-8 String array   |
Tab Pages            | `Write Control Text.vi` with 1D UTF-8 String array   | :heavy_check_mark: (once for each page, see `Unicode Tab Page Example.vi` for details)
Boolean Text         | `Write Control Text.vi` with 1D UTF-8 String array (4 elements) |
Table Data           | `Write Control Text.vi` with 2D UTF-8 String array   |
Listbox              | `Write Control Text.vi` with 1D UTF-8 String array   |
MultiColumn Listbox  | `Write Control Text.vi` with 2D UTF-8 String array   |
Text / Menu Ring     | `Write Control Text.vi` with 1D UTF-8 String array   |
Graph Axis Labels    | `Write Control Text.vi` with 2x scalar UTF-8 String  |
Graph Plot Names     | `Write Control Text.vi` with 1D UTF-8 String array   | :heavy_check_mark: (once on any plot name, doesn't persist)
Graph Cursor Names   | `Write Control Text.vi` with 1D UTF-8 String array   |
Radio Buttons        | `Write Control Text.vi` with 1D UTF-8 String array (1 element per button) |
Tree                 | Write UTF-16LE + BOM to _Left Cell String_ (see `Unicode Tree File Browser Example.vi`) |
Picture Text         | Write UTF-16LE + BOM with `Draw Text At Point.vi`    |

If the above hasn't helped, and unicode display still looks wrong:
* Disable the _UseUnicode_ flag in `labview.ini` to avoid accidentally mixing text encodings. This flag does not affect unicode input or display (which is managed by a control's __Force Unicode Text__ and __Interpret As Unicode__ private properties).
* Try copy and paste some unicode text over the current text (from a LabVIEW string control). This is sometimes necessary to force the text to display as unicode (e.g. tab page text)
* The unicode text may be UTF-16LE encoded when UTF-8 is assumed (or vica versa). Try switching encoding using `Encode String.vi`
* Some text items require a Byte Order Mark (BOM) in addition to UTF-16LE encoded text. Try adding a BOM with `Prepend BOM.vi` (__Note:__ `Write Control Text.vi` does this automatically for supported controls)
* If a control or constant still isn't displaying text correctly, try deleting and recreating it.

If the control to be written doesn't have explicit support in G-Unicode, it may still be possible to display unicode text. There are three methods LabVIEW for Windows uses to display unicode, depending on the control type:
1. Erasing the text, setting __Force Unicode Text__ to `TRUE`, then writing UTF-16LE encoded text
2. Writing UTF-16LE encoded text with BOM
3. Copying + pasting unicode text to the target to enable unicode display, then writing UTF-16LE encoded text


### Examples
See the example VIs in [Examples](src/LabVIEW/G-Unicode/Examples).

Unit tests are included and can be run individually, or with the [AST Unit Tester](https://www.autosofttech.net/documents/ast-unit-tester).

## <a id="building"></a>Building
TBD

## <a id="comparison"></a>Comparison
TBD

## <a id="license"></a>License
G-Unicode is made available under the zlib license. See [LICENSE](LICENSE) for details.

## <a id="acknowledgments"></a>Acknowledgments
This library uses the following open source code. Massive thanks to the authors of these libraries.

Library | License
--------|----------------------
[utf8.h](https://github.com/sheredom/utf8.h) | [Unlicense](https://github.com/sheredom/utf8.h/blob/master/LICENSE)
[Portable File Dialogs](https://github.com/samhocevar/portable-file-dialogs) | [WTFPL](https://github.com/samhocevar/portable-file-dialogs/blob/main/COPYING)
[TinyDir](https://github.com/cxong/tinydir) | [Simplified BSD](https://github.com/cxong/tinydir/blob/master/COPYING)

