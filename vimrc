version 6.0
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
let &cpo=s:cpo_save
unlet s:cpo_save
set backspace=2
set helplang=cn
set modelines=0
set window=0
set cindent

set background=dark
set ruler
set cscopetag			" if match more than one ...

set tags=./tags;/		" search until /
if has("cscope")
    set cst
    " add any database in current directory
    if filereadable("cscope.out")
        cs add cscope.out
        " else add database pointed to by environment
    elseif $CSCOPE_DB != ""
        cs add $CSCOPE_DB
    endif
endif

syntax on
set ignorecase
set tabstop=4 shiftwidth=4 autoindent
set expandtab
set hlsearch
set wrap
set vb t_vb=		" turn off beep
set autowriteall
"set scrolloff=999	" keep the cursor line always being in the middle of the window.
set scrolloff=3

nnoremap <F6> "=strftime("%Y/%m/%d %T")<CR>P
inoremap <F6> <C-R>=strftime("%Y/%m/%d %T")<CR>

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" for netrw
let g:netrw_list_hide='\(^\|\s\s\)\zs\.\S\+,\~$'

""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

" disable matchparen plugin at startup 
" details: http://vim.1045645.n5.nabble.com/re-enabling-the-disabled-matchparen-plugin-td3413501.html
au VimEnter * NoMatchParen 

if has('gui_running')
	" set guifont=Courier\ New:h16
	set guifont=FixedSys:h15
	set guifontwide=stsong
	set lines=38
	set columns=139
	set cursorline
	set background=light
	set fileencodings=utf-8,gbk
	set foldcolumn=3	" left margin

	colorscheme my-colorscheme

	let rgb_file = "/Applications//MacVim.app/Contents/Resources/vim/runtime/rgb.txt"
	command ListMemo !ls -lrt memo.*.txt
	command Proj !ls -lrt *.PROJ.*
    command WebOpen !open -a "google chrome" %
    command Mou !open -a mou %
    command -nargs=1 Ls !ls | grep -i <args>

	cd ~/memo/
	edit TODAY.markdown				" b1
	badd+ F.Mail.Memo				" b2
	badd+ F.Alibaba.Tech			" b3
	badd+ F.Internet.Industry		" b4
	badd+ F.TechMemo				" b5 tech memo
	badd+ F.Daily					" b6
	badd+ memo.2013-03-01.LinuxOSAndKernel.markdown	" b7
	badd+ E.People					" b8
    badd+ memo.2013-05-07.Tech.defender代码学习.markdown " b9
	badd+ F.WorkMethod				" b10
endif
