# Creating A VIPM Package
The combination of G-Unicode's external DLL libraries, malleable VIs, 32/64-bit support, and (future) cross-platform support means building a package with VIPM requires a few extra steps.

The main issues which require workarounds are:
1. Call Library Function Node library path changes
    * When building a package with VIs that contain CLFNs that are configured with wildcard paths (e.g. g_unicode_*.*), VIPM will replace the wildcards with the target bitness and platform library type (g_unicode_32.dll for 32-bit LabVIEW on Windows). This means when the package is installed on a different platform or bitness, these VIs will be broken.
	* There is a script which can be run post-install to undo the CLFN changes.
2. Malleable VIs and CLFNs
    * VIs which call malleable subVIs that contain CLFNs also run into the above issue, except the modified CLFNs are tucked away inside malleable VI _instances_ (basically a copy of the malleable VI preconfigured with wire types, stored inside the calling VI).
	* Undoing the library changes for the instance VIs requires additional scripting, and is very slow.
3. Malleable VI sizes
    * Any VI which calls multiple nested malleable VIs will save the entire heirarchy of instance VIs. This can cause VI sizes to balloon, and push the package size over VIPM's package size limit.
