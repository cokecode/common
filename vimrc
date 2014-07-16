if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
let &cpo=s:cpo_save
unlet s:cpo_save

set backspace=2
set helplang=cn
set modelines=1
set window=0
set cindent

set background=light
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

" 输入:set list命令是应该显示些啥？
set listchars=tab:\|\ ,trail:.,extends:>,precedes:<,eol:$

" 侦测文件类型
filetype on

" 载入文件类型插件
filetype plugin on

" 为特定文件类型载入相关缩进文件
filetype indent on

"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 文件设置
"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
" 不要备份文件（根据自己需要取舍）
set nobackup

" 不要生成swap文件，当buffer被丢弃的时候隐藏它
setlocal noswapfile
set bufhidden=hide

" 增强模式中的命令行自动完成操作
set wildmenu

" 在状态行上显示光标所在位置的行号和列号
set ruler
"set rulerformat=%20(%2*%<%f%=\ %m%r\ %3l\ %c\ %p%%%)

" 命令行（在状态行下）的高度，默认为1，这里是2
" set cmdheight=2
set cmdheight=1

" 通过使用: commands命令，告诉我们文件的哪一行被改变过
set report=0

" 在被分割的窗口间显示空白，便于阅读
set fillchars=vert:\ ,stl:-,stlnc:-

" 继承前一行的缩进方式，特别适用于多行注释
set autoindent

" 为C程序提供自动缩进
set smartindent

" 使用C样式的缩进
set cindent

" 制表符为4
set tabstop=4

" 统一缩进为4
set softtabstop=4
set shiftwidth=4

set noet

nnoremap <F6> "=strftime("%Y/%m/%d %T")<CR>P
inoremap <F6> <C-R>=strftime("%Y/%m/%d %T")<CR>

nnoremap gA :!$HOME/bin/a<CR><CR>
nnoremap gB :!$HOME/bin/a+build<CR>
nnoremap gG :!$HOME/bin/G<CR><CR>

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
	"set guifont=Monaco:h13
	"set guifontwide="Hiragino Sans GB"
	set lines=38
	set columns=139
	set cursorline
	set background=light
	set fileencodings=utf-8,gbk
	set foldcolumn=3	" left margin

    " 与windows/mac共享剪贴板（有问题）
    " set clipboard+=unnamed

	colorscheme my-colorscheme

	let rgb_file = "/Applications//MacVim.app/Contents/Resources/vim/runtime/rgb.txt"
	command ListMemo !ls -lrt memo.*.txt
	command Proj !ls -lrt *.PROJ.*
    command WebOpen !open -a "google chrome" %
    command Mou !open -a mou %
    command -nargs=1 Ls !ls | grep -i <args>

	if fnamemodify(argv(0), ":t") =~ "memo\..\*"
		cd ~/memo/
	endif

	if argc() == 0
		cd ~/memo/
		edit TODAY.markdown				" b1
		badd+ link2						" b2
		badd+ memo.2013-04-19.工作周报.WeeklyReport.markdown " b3
		badd+ F.Internet.Industry		" b4
		badd+ F.TechMemo				" b5 tech memo
		badd+ F.Daily					" b6
		badd+ memo.2013-03-01.LinuxOSAndKernel.markdown	" b7
		badd+ E.People					" b8
		badd+ memo.2013-05-07.Defender.代码学习.markdown " b9
		badd+ F.WorkMethod				" b10
	endif
endif
