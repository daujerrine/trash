$if(title)$
# $title$
$if(subtitle)$
## $subtitle$
$endif$
$for(author)$
### $author$
$endfor$
$if(date)$
_last updated: $date$_
$endif$

------------------------------------------------------------------------

$endif$

$if(titleblock)$
$titleblock$

$endif$
$for(header-includes)$
$header-includes$

$endfor$
$for(include-before)$
$include-before$

$endfor$
$if(toc)$
# $toc-title$
$table-of-contents$
$endif$
$body$
$for(include-after)$

$include-after$
$endfor$

