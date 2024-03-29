# G-Unicode
<p align="center">
  <img width="346" height="115" src="images/g-unicode-logo.png">
</p>
<p align="center">
A LabVIEW library for displaying Unicode text, manipulating Unicode strings, and performing file I/O on Unicode paths.
</p>
<p align="center">
<a href="https://www.vipm.io/package/dataflow_g_lib_g_unicode/"> <img src="https://www.vipm.io/package/dataflow_g_lib_g_unicode/badge.svg?metric=installs"></a> <a href="https://www.vipm.io/package/dataflow_g_lib_g_unicode/"><img src="https://www.vipm.io/package/dataflow_g_lib_g_unicode/badge.svg?metric=stars"></a>
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
* New custom probe `Unicode String Probe.vi`
    * Detects and displays Unicode for any LabVIEW string
    * Works as a regular string probe
* New custom probe `Unicode String Probe (Extended).vi`
    * Displays ANSI, UTF-8, UTF-16LE, and UTF-16BE encodings plus raw bytes for a LabVIEW string
* Updated UTF-8 String / Path class custom probes to display Unicode text in the Value column of probe watch window
* `Search-Split String (Unicode).vim` can search on individual I32 codepoint
* `Normalize End Of Line.vim` supports Unicode line separator (LS) and paragraph separator (PS)
* Updated VI documentation detailing supported malleable VI input types
* Additional string functions
* Fix potential file dialog hang

#### New VIs
##### String Palette
* `Normalize String (Unicode).vim` & `Normalize String.vi`
    * Use to normalize Unicode codepoints into C, D, KC, and KD forms
    * Example: `Unicode String Normalization Example.vi`
* `Spreadsheet String To Array.vim`
* `Array To Spreadsheet String.vim`

##### Custom Probes
* `Unicode String Probe.vi`
* `Unicode String Probe (Extended).vi`

## <a id="motivation"></a>Motivation
LabVIEW doesn't provide official Unicode support under Windows, but through a combination of hidden control properties, byte order marks, and good luck, it is possible to read and display Unicode text. Unfortunately there are still many gaps in functionality:
* File I/O functions don't support Unicode, and any Unicode characters in paths are replaced with `?` rendering them invalid
* String operations aren't Unicode aware

There is a UTF-8 locale setting in Windows, and ability to embed a UTF-8 application manifest to a compiled LabVIEW app (both available since Windows 10 v1903), which brings another level of Unicode support to LabVIEW. This still requires special string handling and encoding for display.

G-Unicode seeks to streamline Unicode handling in LabVIEW, making it simpler to develop applications with Unicode and multilingual support.

## <a id="features"></a>Features
* Drop-in Unicode replacements for string, file I/O, and user dialog functions
* Unicode path support
    * Read/write filenames and paths which contain Unicode - no more _?????_ in filenames
    * Native system file dialog which returns Unicode paths
    * File references opened from Unicode paths compatible with LabVIEW's File I/O functions
* All Unicode data is internally encoded as UTF-8, stored in new _UTF-8 String_ and _UTF-8 Path_ class types
* New VIs accept both standard LabVIEW strings/paths and UTF-8 String/Path class inputs
* Custom class probes allows viewing Unicode text when debugging
* Custom QuickDrop plugin allows viewing and editing UTF-8 String/Path constant values
* VIs for reading and writing Unicode text to LabVIEW controls

![G-Unicode palettes](images/g-unicode-palettes.png?raw=true "G-Unicode palettes")

## <a id="installation"></a>Installation
G-Unicode is published on [vipm.io](https://www.vipm.io/package/dataflow_g_lib_g_unicode/), and can be installed using VI Package Manager (VIPM). The packages are also available as [github releases](https://github.com/dataflowg/g-unicode/releases) and can be installed manually using VIPM.

## <a id="usage"></a>Usage
**IMPORTANT:** LabVIEW has a [UseUnicode](https://labviewwiki.org/wiki/LabVIEW_configuration_file/Miscellaneous#useUnicode) ini token to help aid in handling Unicode input. It enables right-click options for Unicode display, and enables pasting Unicode text from external applications. This is useful, but can also make dealing with string controls and constants problematic, silently mixing UTF-16LE and ANSI encodings when pasting or typing text. It is recommended to disable this token during development, though it can be enabled in built applications. _G-Unicode does not require this token, except for the following functions:_

* `Set Window Title (Unicode).vim`

**IMPORTANT II:** G-Unicode relies on LabVIEW's experimental Unicode support for Unicode text input. This Unicode text input is _**very buggy**_, and prone to strange input behavior and outright crashes. Where possible it's recommended to avoid using LabVIEW's string controls for Unicode text input and instead use `Input Box (Unicode).vim` or some other method.

### New Types
G-Unicode use two classes for handling Unicode text: _UTF-8 String.lvclass_ and _UTF-8 Path.lvclass_. These classes are containers around LabVIEW's standard string and path types (encoded as UTF-8), adding a custom wire appearance and Unicode capable probes.

Type         | Wire Appearance
-------------|----------------
UTF-8 String | ![UTF-8 String wire](images/wire-utf8-string.png?raw=true "UTF-8 String wire")
UTF-8 Path   | ![UTF-8 Path wire](images/wire-utf8-path.png?raw=true "UTF-8 Path wire")

Most functions in G-Unicode support wiring either a UTF-8 String/Path class, a standard LabVIEW string/path, or a combination of both (as in `Concatenate Strings (Unicode).vim`). When a standard LabVIEW type is wired to a G-Unicode function it is internally converted to UTF-8 using LabVIEW's built-in `Text to UTF-8` primitive, and any return values are in the respective UTF-8 class.

#### Editing UTF-8 String/Path Constants
UTF-8 String and UTF-8 Path class values can be set during edit time using the QuickDrop shortcut **Ctrl+G**. Select a UTF-8 control/constant on the front panel/block diagram, then press _Ctrl+Space_ to bring up QuickDrop, then press _Ctrl+G_ to open a Unicode input dialog to view and set the UTF-8 value. When a custom value is applied to the class, its icon will have a dark colored background. The custom value of a UTF-8 class can be cleared using the QuickDrop shortcut **Ctrl+Shift+G**.

![G-Unicode custom probe](images/g-unicode-demo-2.png?raw=true "G-Unicode custom probe")

### Error Wiring
There are two types of error wiring conventions used throughout G-Unicode. The first is the standard error in/error out clusters wired to the lower left and right terminals respectively. It is recommended to always wire these terminals.

The second convention is the error in/error out clusters wired to the bottom two terminals in the center of the connector pane. These error terminals only need to be wired when a standard LabVIEW string or path is wired to a function (which are subject to the internal UTF-8 conversion mentioned above). If only UTF-8 String/Path(s) are wired to the function, these terminals will never contain error information and so can be ignored.

![Example G-Unicode error wiring](images/g-unicode-error-wiring.png?raw=true "Example G-Unicode error wiring")

### Using Unicode Paths With LabVIEW Functions
The File I/O functions included with G-Unicode cover most common file operations performed in LabVIEW. To use Unicode paths with other LabVIEW functions which have a standard path input, first convert the Unicode path to a short 8.3 format path with `Path to Short Name (Unicode).vi`. Note that the Unicode path must exist on disk for this function to work. If it doesn't exist, it can be created using `Create File (Unicode).vim`.

In the example below the Unicode path `c:\LabVIEW Data\안녕하세요.tdms` is created, then the short name retrieved and passed to LabVIEW's TDMS functions. After the file is closed, the short name is converted back to the full Unicode path.

![Unicode path with TDMS functions](images/g-unicode-tdms-example.png?raw=true "Unicode path with TDMS functions")

There are caveats to this approach (the file must exist, ReFS file system isn't supported, TDMS index files will have the short path name, etc) so proceed with caution!

### Tips On Displaying Unicode
LabVIEW's Unicode support is experimental, and it can be very easy to run into issues. If Unicode text doesn't display correctly (it has the characteristic double-spacing or is completely wrong), check the table below for tips on how to set Unicode text.

If there's a check mark in the _Copy + Paste Unicode_ column, the target text will need to have some Unicode text copied and pasted into the text field first to force the text to display as Unicode. If `UseUnicode` is not set, the text must be copied from a LabVIEW control already displaying Unicode. Otherwise it can be copied from an external app such as a web browser.

**NOTE:** The _Stacked_ text orientation doesn't handle combining characters, diacritics, etc

Text Item            | Method                                                         | Copy + Paste Unicode
---------------------|----------------------------------------------------------------|---------------------
Caption              | `Write Control Caption (Unicode).vi` with scalar UTF-8 String  |
String               | `Write Control Text (Unicode).vi` with scalar UTF-8 String     |
String Array (1D)    | `Write Control Text (Unicode).vi` with 1D UTF-8 String array   |
String Array (2D)    | `Write Control Text (Unicode).vi` with 2D UTF-8 String array   |
Tab Pages            | `Write Control Text (Unicode).vi` with 1D UTF-8 String array   | :heavy_check_mark: (once for each page, see `Unicode Tab Page Example.vi` for details)
Boolean Text         | `Write Control Text (Unicode).vi` with 1D UTF-8 String array (4 elements) |
Table Data           | `Write Control Text (Unicode).vi` with 2D UTF-8 String array   |
Listbox              | `Write Control Text (Unicode).vi` with 1D UTF-8 String array   |
MultiColumn Listbox  | `Write Control Text (Unicode).vi` with 2D UTF-8 String array   |
Text / Menu Ring     | `Write Control Text (Unicode).vi` with 1D UTF-8 String array   |
Graph Axis Labels    | `Write Control Text (Unicode).vi` with 2x scalar UTF-8 String  |
Graph Plot Names     | `Write Control Text (Unicode).vi` with 1D UTF-8 String array   | :heavy_check_mark: (once on any plot name, doesn't persist)
Graph Cursor Names   | `Write Control Text (Unicode).vi` with 1D UTF-8 String array   |
Radio Buttons        | `Write Control Text (Unicode).vi` with 1D UTF-8 String array (1 element per button) |
Tree                 | Write UTF-16LE + BOM to _Left Cell String_ (see `Unicode Tree File Browser Example.vi`) |
Picture Text         | Write UTF-16LE + BOM with the picture function text VIs (see `Unicode Picture Control Example.vi`) |
Probe Value          | Write UTF-16LE + BOM to custom probe VI's Value String output  |

If the above hasn't helped, and Unicode display still looks wrong:
* Disable the _UseUnicode_ flag in `labview.ini` to avoid accidentally mixing text encodings. This flag does not affect Unicode input or display (which is managed by a control's __Force Unicode Text__ and __Interpret As Unicode__ private properties).
* Try copy and paste some Unicode text over the current text (from a LabVIEW string control). This is sometimes necessary to force the text to display as Unicode (e.g. tab page text)
* The Unicode text may be UTF-16LE encoded when UTF-8 is assumed (or vice versa). Try switching encoding using `Encode String.vi`
* Some text items require a Byte Order Mark (BOM) in addition to UTF-16LE encoded text. Try adding a BOM with `Prepend BOM.vi` (__Note:__ `Write Control Text (Unicode).vi` does this automatically for supported controls)
* If a control or constant still isn't displaying text correctly, try deleting and recreating it.

If the text item to be written doesn't have explicit support in G-Unicode, it may still be possible to display Unicode text. There are three methods LabVIEW for Windows uses to display Unicode, depending on the control type:
1. Erasing the text, setting __Force Unicode Text__ to `TRUE`, then writing UTF-16LE encoded text
2. Writing UTF-16LE encoded text with BOM
3. Copying + pasting Unicode text to the target to enable Unicode display, then writing UTF-16LE encoded text

Try one of the above to see if the text item supports Unicode. If it does, please raise an issue to get it added to the library!

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
