" Vim indent file
" Language:	Paste Level Description
" Maintainer:	Chris Schmich <schmich@uiuc.edu>

" Only load this indent file when no other was loaded.
if exists("b:did_indent")
  finish
endif
let b:did_indent = 1

" Some preliminary settings
setlocal nolisp		" Make sure lisp indenting doesn't supersede us
setlocal autoindent	" indentexpr isn't much help otherwise

setlocal indentexpr=GetPasteIndent()
setlocal indentkeys+=<:>,=elif,=except

" Only define the function once.
if exists("*GetPasteIndent")
  finish
endif

let s:maxoff = 50	" maximum number of lines to look backwards for ()

function GetPasteIndent()
    let lnum = v:lnum

    if lnum == 0
        return 0
    endif

    if indent(lnum - 1) > 0 && getline(lnum - 1) !~ '^\s*$'
        return &sw
    endif

    if indent(lnum + 1) == 0
        return 0
    endif

    return 0
endfunction

" vim:sw=2
