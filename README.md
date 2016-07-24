# stocksoup
A collection of various, mostly one-off SourceMod stock functions.  They're not particularly organized and might be rearranged at any time, but hey, they might come in useful.

## Usage (for standalone projects)
1.  Add the repository as a submodule (as an include relative to your `scripting` directory).

        $ git submodule add https://github.com/nosoop/stocksoup scripting/include/stocksoup
        
    If you're using Github for Windows (like I am), you'll probably have to perform the commit via Git Bash, too.

2.  If not already, make sure your SourcePawn compiler looks into the custom include directory.

        spcomp "scripting/in_progress_file.sp" -i"scripting/include/"

3.  Include a specific file and use a stock.

        #include <stocksoup/client_effects>
        
        /* Not actually a callable forward. */
        public void OnPlayerSpawn(int client) {
                SetClientScreenOverlay(client, "combine_binocoverlay");
        }

## Updates (for standalone projects)
1.  Pull in updates for all the submodules.

        $ git submodule update --remote --merge
        
    (And yes, Git Bash again for casuals like us.)

2.  Fix everything that broke because I can't maintain a stable API.  Function stocks generally won't move between includes, but the includes themselves might've changed names between updates.

## Directory structure
Pretty simple:

*   Base directory has stocks applicable to all games.
*   Subdirectories have stocks applicable to a specific mod.  Mainly TF2, since that's the only game I write for.
    *   Ideally, the filenames of game-specific includes should match the generic ones.
