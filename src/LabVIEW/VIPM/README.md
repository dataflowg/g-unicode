# Creating A VIPM Package

## Background
The combination of G-Unicode's external DLL libraries, malleable VIs, 32/64-bit support, and (future) cross-platform support means building a package with VIPM requires a few extra steps.

The main issues these steps address are:
1. VIPM changes Call Library Function Node library paths when building (see [this thread](https://forums.vipm.io/topic/2427-call-library-function-node-32-and-64-bit))
    * When building a package with VIs that contain CLFNs that are configured with wildcard paths (e.g. g_unicode_*.*), VIPM will replace the wildcards with the target bitness and platform library type (g_unicode_32.dll for 32-bit LabVIEW on Windows). This means when the package is installed on a different platform or bitness, these VIs will be broken.
	* There is a [post-install script](https://forums.vipm.io/topic/2427-call-library-function-node-32-and-64-bit/?do=findComment&comment=8609) which can be used to undo the CLFN changes.
2. Malleable VIs and CLFNs
    * VIs which call malleable subVIs that contain CLFNs also run into the above issue, except the modified CLFNs are tucked away inside malleable VI _instances_ (basically a copy of the malleable VI preconfigured with wire types, stored inside the calling VI).
	* Undoing the library changes for the instance VIs requires [additional scripting](https://forums.vipm.io/topic/2427-call-library-function-node-32-and-64-bit/?do=findComment&comment=10107) which works, but is _very_ slow.
3. Malleable VI sizes
    * Any VI which calls multiple nested malleable VIs will save the entire heirarchy of instance VIs. This can cause VI sizes to balloon, and can push the package size over VIPM's package size limit.

Points 1. and 2. above aren't an issue if the LabVIEW library code is simply copied to the target environment. VIPM does have a _Do Not Compile on Build_ option for VIs which may help, but this has the unfortunate side effect of no longer including those VIs in the palette. Rather than execute post-install scripts to revert the CLFN path changes, a second copy of the source code can be zipped and bundled inside the package file. A post-install script then extracts the zip file over the installed files, reverting any CLFN path changes. This was the technique used to create the G-Audio and G-Image packages.

Bundling a zipped version of the original code works for G-Unicode too, but point 3. above means the final vipc is large (>16MB). To remove the size overhead of including two copies of the code in the package, a modified version of the code is created which removes all block diagram code except the terminals. This stripped down version of code is used to create the package (primarily for palette entries), and the original untouched source included in a zip as before. The same post-install extraction script is used to revert to the original code. Using these steps on G-Unicode drops the package size down to ~9.5MB.

## Creating the Build
With the above in mind the process for creating the G-Unicode package is as follows:
1. Close any LabVIEW projects, then navigate to the `src\LabVIEW\VIPM\Scripting` folder.
2. Open and run `1 - Prepare Source Files.vi`
    * This will copy all project source code to `src\LabVIEW\VIPM\Package Source`, and strip the block diagrams to just their terminals
3. Open the VIPM build spec at `src\LabVIEW\VIPM\G-Unicode.vipb` and build a new package.
4. From the `Scripting` folder, open and run `2 - Generate Post-Install.vi`. When prompted, select the newly created package from the `Packages` folder.
    * This script will cross check the VIs and VIMs in the VIPM package against the original source folder, and create a zip of the matching files from the original code. The resulting zip is placed in `Package Source\Post Install`.
5. Open the VIPM build spec again, set the build version as necessary, then on the Source File Settings page hit the refresh arrow button to ensure the new zip file is included in the build.
6. Create the final build.
