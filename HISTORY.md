# G-Unicode Release History
### v0.3.0 - 2023-08-28
* New custom probe `Unicode String Probe.vi`
    * Detects and displays Unicode for any LabVIEW string
* New custom probe `Unicode String Probe (Extended).vi`
    * Displays ANSI, UTF-8, UTF-16LE, and UTF-16BE encodings plus raw bytes for a LabVIEW string
* Updated UTF-8 String / Path class custom probes to display Unicode text in the Value column of probe watch window
* `Search-Split String (Unicode).vim` can search on individual I32 codepoint
* Updated VI documentation detailing supported malleable VI input types
* Additional string functions

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



### v0.2.0 - 2023-08-21
* Additional string, file I/O, and UI functions.
* QuickDrop shortcut (Ctrl+G) to view/edit UTF-8 class value now works on front panel controls
* QuickDrop shortcut (Ctrl+Shift+G) to erase UTF-8 class value from constant / control

#### New VIs
##### String Palette
* `String To Codepoint Array (Unicode).vim`
* `Codepoint Array To String (Unicode).vi`
* `1D String Array To Delimited String (Unicode).vim`
* `Delimited String To 1D String Array (Unicode).vim`
* `Normalize End Of Line (Unicode).vim`
* `Trim Whitespace (Unicode).vim`
##### File I/O Palette
* `Detect Text File Encoding (Unicode).vim`
##### Dialog & User Interface Palette
* `Set Window Title (Unicode).vim`
    * Requires _UseUnicode=TRUE_ in labview.ini



### v0.1.0 - 2023-08-06
* Initial release
