# EVe_HallC

EVe_HallC is an event display program for the Hall C HMS and SHMS detector
stacks.


## Branches

There are several branches currenlty in the [JeffersonLab/EVe_HallC][EVe_GitHub]
repository:
- `master`: holds code common to both HMS and SHMS stacks
- `HMS`: holds HMS specific code
- `SHMS`: holds SHMS specific code
- `develop`: deprecated
- `BetheHeitler`: ??

[EVe_GitHub]: https://github.com/JeffersonLab/EVe_HallC (JeffersonLab/EVe_HallC)


## Usage

- Select appropriate branch (`HMS` or `SHMS`):
    - `git checkout <branch>`
- Check for updates:
    - `git fetch upstream`
    - `git merge upstream/<branch>`
- Setup environment:
    - setup ROOT 5.34
    - `setenv ANALYZER <path/to/podd>`
- Build library:
    - `make`
- Load library into ROOT and start viewer:
    - `root -l simEVe.C`


## Contributions

Contributions/changes to the EVe_HallC repo will follow the same development
model as for the Hall C analyzer (hcana) outlined in this [link][hcana].

They boil down to this:
- Fork repo to your GitHub.
- **Do not edit files in the `master`, `HMS` or `SHMS` branches!**
- Select appropriate branch for contribution
    - `master` for changes that affect both HMS and SHMS,
    - `HMS` or `SHMS` for spectrometer specific changes,
    - `git checkout <branch>`
- Check for upstream updates:
    - `git fetch upstream`
    - `git merge upstream/<branch>`
- Branch a new `<featureBranch>` off of selected branch and work on your changes.
    - Make frequent _atomic_ commits with _useful_ commit logs.
    - **Test your code.**
- When your feature is _done_:
    - push `<featureBranch>` to your GitHub repository:
        - `git push --set-upstream origin <featureBranch>`
    - make pull request to `JeffersonLab/<branch>` from `<you>/<featureBranch>`
      on GitHub.
    - Make sure changes are still compatible with any recent upstream changes
      and fix if not.

And most of all, good luck. You will need it.

[hcana]: https://hallcweb.jlab.org/wiki/index.php/Analyzer/Git#Setup_and_creating_a_personal_fork_of_the_analyzer (hcana GitHub repository)


### Formatting/Style

Please follow the style conventions (4-space indent, no tabs, etc):
- 4 space indent
- no hard tabs
- no trailing whitespace
- end file with a newline

#### Vim

Vim users can install [this plugin][vim_plugin] to make use of the `.lvimrc`
defaults file.

    vim Docs/localvimrc.vba
    :so %
    :q

I'd also add this to your `~/.vimrc`:

    if !exists("g:localvimrc_persistent")
      let g:localvimrc_persistent=2    " save local vimrc responses
    endif

#### Emacs

A `.dir-locals.el` defaults file is present for Emacs users.

[vim_plugin]: http://www.vim.org/scripts/script.php?script_id=441 (Vim plugin)


## Documentation

Some documentation may be found in the `Docs` directory, however it has not
kept pace with some of the modifications so YMMV.


## Acknowledgments

This code is based off an event display originally written by Miha Mihovilovic
in 2008 for the Hall A BigBite Spectrometer. Changes are significant and
ongoing. See `git log` for the history.
