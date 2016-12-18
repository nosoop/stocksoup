# stocksoup
A mishmashed collection of various SourceMod stock functions.  Might be useful at some point.

## Usage (as a submodule, preferred)
Installing stocksoup as a Git submodule means that you and possible contributors won't be tripped up by function and include renames whenever I feel like doing them.  Of course, this is only useful if you're using a git-compatible system for your repository.

1.  Add the repository as a submodule (as an include relative to your `scripting` directory).

        $ git submodule add https://github.com/nosoop/stocksoup scripting/include/stocksoup
        
    If you're using Github for Windows (like I am), you'll probably have to perform the commit via Git Bash, too.  Commits on top of the submodule addition can proceed as normal.

2.  If not already, make sure your SourcePawn compiler looks into the custom include directory.

        spcomp "scripting/in_progress_file.sp" -i"scripting/include/"

3.  Include a specific file and use a stock.

        #include <stocksoup/client>
        
        public void Example_OnPlayerSpawn(int client) {
                SetClientScreenOverlay(client, "combine_binocoverlay");
        }

4.  For collaboration, you should know how to recursively initialize a repository.

## Updates (as a submodule)
1.  Pull in updates for all the submodules.

        $ git submodule update --remote --merge

2.  Fix everything that broke because I can't maintain a stable API.  Function stocks generally won't move between includes, but the includes themselves might've changed names between updates.

3.  The submodule handling in Github for Windows is ass, so you'll want to make the commit via Git Bash.  As with the installation, you can use your normal workflow for commits on top of the submodule update.

## Versioned installation (non-Git)
If you aren't using Git for your project, you can install stocksoup as a default library with a reference to the commit for easier reference.  Not the ideal option, but better than copy / pasting all willy-nilly and setting things on fire.

1.  Click on the "commits" section and click on the "angle brackets" icon to browse the repository at a given commit point.
2.  Click on the green "Clone or Download" button whever Github usually puts it.
3.  Download the ZIP file, extracting it so all the contents are put into a folder with the same name as the ZIP file.
4.  Place the newly extracted folder under the include directory (for your project or for your SourceMod compiler install, doesn't really matter).  Rename the folder such that only the first seven alphanumeric characters past the hyphen (the shorthand way to refer to a commit) are present.  (Example: `stocksoup-9ad2d81`)
5.  When you want to include the particular version of stocksoup, use the folder name.  (Example: `#include <stocksoup-9ad2d81/client_effects>`)
6.  For updates, just install the latest version in the same way, with the new shorthand commit reference.


## Directory structure
Pretty simple:

*   Base directory has stocks applicable to all games.
    *   The `sdkports/` directory contains ports of select Source SDK functions.
*   Other subdirectories have stocks applicable to a specific mod.  Mainly TF2, since that's the only game I write for.


## Questions and Answers

**Is the name of the library a reference to Weird Al's [*Talk Soup*][yt-talksoup]?**
Yes.  Yes it is.

[yt-talksoup]: https://youtu.be/555ndsDM2qo
