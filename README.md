# stocksoup
A collection of various SourceMod stock functions.

## Usage
1.  Add the repository as a submodule (as an include relative to your `scripting` directory).

        $ git submodule add https://github.com/nosoop/stocksoup scripting/include/stocksoup

2.  If not already, make sure your SourcePawn compiler looks into the custom include directory.

        spcomp "scripting/in_progress_file.sp" -i"scripting/include/"

3.  Include.  (Soon.)

        #include <stocksoup/stocksoup>
