# The XouverKit
*Manage; Build and Run Xouver Applications*

## What is it?
The *XouverKit* is an easy to use tool for developers designed to create, manage and build Xouver applications.
Also it is the tool used to execute Xouver-Based applications and controls how the XouverRuntime should behave.

Although it can execute ``.xvr`` (Xouver-Object) files, it is mainly designed to (build and) run ``.xapp`` files,
which contain various configurations used to define the behavour of the application.

## How to use it?
The XouverKit is shipped together with the Xouver Development Kit (XDK) and the Xouver Runtime Environment (XRE),
but both of those contain different versions of the kit. Whilst the kit shipped with the XDK supports building and creating Xouver projects,
the kit shipped with the XRE only allows for executing applications.

To show a list of available commands, use:
```
xouverKit help
```
***Note*** instead of typig out xouverkit you should also be able to use the aliases ``xouver`` and ``xvr``

If your project contains a valid ``project.xproj`` file, which is compatible with the X-Kit, it should easily be built using the command:
```
xouverKit build
```
No further arguments are expected; configurations should be done inside of the project-file.

### Project structure
The XouverKit by default expectes a specific project-structure to be used:
```
Project Directory
├── Dependencies
├── Output
├── Resources
├── Source
├── project.xproj
```

This can be disabled by adding the line ``UseDefaultProjectStructure false`` under ```#BuildConfiguration`` inside the project-file.
Following paths must be set inside the project-File, so the kit can detect files (all paths are relative to the project-file):
- DependenciesDirectory
- OutputDirectory
- ResourcesDirectory
- SourceDirectory

#### Dependencies-folder
The dependencies-folder contains compiled ``packages`` (APIs and Frameworks) to be used in your project.
Although ``.xvr``-files can be loaded as dependencies (`SafeDependency`` must be false in your project-file; defaults to true),
it is recommended to refrain from that, as an object-file only contains function signatures, withouth them being mapped to their
respective classes. **Only** use these as dependency if you know what you're doing.

``.xpkg``-files on the other hand contain fully mapped and (if done so) descripted functions, classes and their field names,
to be used to integrate them as-written into your project.
Use 
```
xouverKit tools RetrieveMap <PACKAGE> <OUTPUT>
```
to retrieve the map-description of the package file and write it into an output file. (File will be overriden, if exists)
#### Resources-folder
The resources-folder shall contain resources which are packaged as-is (no compression) into your built project.
These can then be used by using the ``ResourceManager``-class.

#### Source-folder
The source-folder should only contain compilable xouver classes.
Non ``.xs`` (Xouver-Source) files will throw a warning, the build process will not be cancelled.
This can be disabled using:
```
xouverKit config ThrowWarningIfNotSource false
```

## Versioning
The command:
```
xouverKit version
```

does output two different versions (three on the XDK):
- Kit version
- Bytecode version
- (Xouver version)

The kit version display the current version of the kit, this may contain feature-updates for the kit-application iself
The bytecode version displays the version of the bytecode the vm uses / the compiler compiles to. This version is retrieved from the vm and the compiler.
The xouver version (XDK only) displays the version of the Xouver-Language uses.

