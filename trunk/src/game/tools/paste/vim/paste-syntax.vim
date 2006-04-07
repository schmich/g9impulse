" Vim syntax file
" Language:	Paste Level Description
" Maintainer:	Chris Schmich <schmich@uiuc.edu>

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if !exists("main_syntax")
  if version < 600
    syntax clear
  elseif exists("b:current_syntax")
  finish
endif
  let main_syntax = 'paste'
endif

syn case match

syn keyword pasteCommand spawn

syn keyword pasteType config
syn region  pasteDeclare matchgroup=pasteType start=/^\(sprite\|projectile\|enemy\)/ end=/$/ contains=pasteInherit
syn match   pasteInherit /\(<\(\(<\|\s\)*\)\)\@<!</ contained

syn match pasteEvent /\v^\d+/

syn match pasteField /animation:/
syn match pasteField /impact:/
syn match pasteField /weapon:/
syn match pasteField /death:/
syn match pasteField /search:/

syn region pasteHealth matchgroup=pasteField start=/\v^\s*health:/ end=/$/ contains=pasteInteger,pasteBadHealthValue
syn match  pasteBadHealthValue /\v[^ 0-9]/ contained

syn region pasteDamage matchgroup=pasteField start=/\v^\s*damage:/ end=/$/ contains=pasteInteger,pasteBadDamageValue
syn match  pasteBadDamageValue /\v[^ 0-9]/ contained

syn region  pasteFiring matchgroup=pasteField start=/\v^\s*firing:/ end=/$/ contains=pasteFiringValue,pasteInteger,pastePercent
syn keyword pasteFiringValue random contained
syn match   pasteFiringValue "lock\-on" contained

syn region  pasteAnim matchgroup=pasteField start=/\v^\s*animate:/ end=/$/ contains=pasteAnimValue,pasteInteger
syn keyword pasteAnimValue never once forever contained

syn region  pasteMotion matchgroup=pasteField start=/\v^\s*motion:/ end=/$/ contains=pasteMotionValue
syn keyword pasteMotionValue boring chase directed contained

syn region  pasteLayer matchgroup=pasteField start=/\v^\s*layer:/ end=/$/ contains=pasteLayerValue
syn keyword pasteLayerValue overlay underlay default contained

syn region pastePair start="(" end=")" contains=pasteBadPair,pasteInteger,pastePercent keepend
syn match  pasteBadPair /\v[^(),\t 0-9%]/ contained
syn match  pasteBadPair /\v\([^,]+\)/ contained
syn match  pasteBadPair /\v\([^0-9]*,/ contained
syn match  pasteBadPair /\v\,[^0-9]*\)/ contained

syn match pasteInteger "\(\(,\|\[\|\s\|(\|:\)\)\@<=[-+]\?\d\+\(,\|\s\|)\|:\|]\|\n\)\@="
syn match pastePercent "\(\(,\|\s\|(\|:\)\)\@<=[-+]\?\d\+%\(,\|\s\|)\|:\|\n\)\@="

syn match pasteColon ':'
syn match pasteColon ':' contained containedin=pasteDamage,pasteHealth,pasteAnim,pasteMotion,pasteFiring,pasteField

if main_syntax == "paste"
  syn sync minlines=10
endif

" Define the default highlighting.
" For version 5.7 and earlier: only when not done already
" For version 5.8 and later: only when an item doesn't have highlighting yet
if version >= 508
    command -nargs=+ Hi hi def link <args>

    " Type Statement Function Number String
    " Constant Comment Error
    " Special StorageClass PreProc

    Hi pasteType Special
    Hi pasteEvent Special
    Hi pasteField Type
    Hi pasteCommand Function
    Hi pasteInherit Special
    Hi pasteInteger Constant
    Hi pastePercent Constant

    Hi pasteValue Constant
    Hi pasteAnimValue Constant
    Hi pasteMotionValue Constant
    Hi pasteLayerValue Constant
    Hi pasteFiringValue Constant

    Hi pasteBadPair Error
    Hi pasteBadDamageValue Error
    Hi pasteBadHealthValue Error

    Hi pasteColon None

    delcommand Hi
endif

let b:current_syntax = "paste"

if main_syntax == 'paste'
  unlet main_syntax
endif

" vim: ts=8

