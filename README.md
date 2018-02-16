# stocksoup
A mishmashed collection of various SourceMod stock functions.
Might be useful at some point.

## Usage
Installing stocksoup as a Git submodule means that you'll be targeting a specific revision of the repository; you and possible contributors won't be tripped up by function and include renames whenever I feel like doing them.
Of course, this is only useful if you're using a git-compatible system for your repository.

1.  Add the repository as a submodule (as an include relative to your `scripting` directory).

        $ git submodule add https://github.com/nosoop/stocksoup scripting/include/stocksoup
        
    If you're using Github for Windows (like I am), you'll probably have to perform the commit via Git Bash, too.  Commits on top of the submodule addition can proceed as normal.

2.  If not already, make sure your SourcePawn compiler looks into the custom include directory.

        spcomp "scripting/in_progress_file.sp" -i"scripting/include"

3.  Include a specific file and use a stock.

        #include <stocksoup/client>
        
        public void Example_OnPlayerSpawn(int client) {
                SetClientScreenOverlay(client, "combine_binocoverlay");
        }

4.  For collaboration, you should know how to recursively initialize a repository:

        $ git clone --recurse-submodules $YOUR_GIT_REPOSITORY

## Updates (as a submodule)
1.  Pull in updates for all the submodules.

        $ git submodule update --remote --merge

2.  Make sure your project actually builds; fix things as necessary.  No stability guaranteed.

3.  Commit as usual.

## Directory structure
Pretty simple:

*   Base directory has stocks applicable to all games.
    *   The `sdkports/` directory contains ports of select Source SDK functions.
*   Other subdirectories have stocks applicable to a specific mod.  Mainly TF2, since that's the only game I write for.  Any stock functions for a specific game should be prefixed with a game abbreviation, similar to SourceMod functions.

## Questions and Answers

**Is the name of the library a reference to Weird Al's [*Talk Soup*][yt-talksoup]?**
Yes.  Yes it is.

[yt-talksoup]: https://youtu.be/555ndsDM2qo
