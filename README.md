# InventorCI
 -----
 
 ## Description
 This is a single-executable project that allows for CI pipeline checks on Inventor Professional 2023 Projects. 
 
 Targeted to work easily with a git pre-push hook as most online runners do not have windows available as an option (for a good reason!)
 - Tested with:
	- GitHub Desktop
	- SourceTree
	- Git Bash
 
 ## Structure
 - Can output JUnit XML files

## Compilation
- Must use MSVC on Visual Studio 2019.

  DESCRIPTION
	
	This builds a simple console application that demonstrates how a client may access
  Inventor's Automation or API from outside its process

  One may also use InvenTor component inside the client's process space if the relevant sub-set of the API
  is all that ISs needed. This trivial application latches on to either a running Inventor application or starts
  up one and allows the user to toggle the 'visibility' property.

  Language/Compiler: VC++ 2012.
	Server: Inventor Server.

  How to Create This Sample?
	Build SimpleExe Project.

  Executable: SimpleExe.exe

  How to Run this sample?
	Run the SimpleExe.exe and you will be given an option to toggle Inventor's visiblility.

	