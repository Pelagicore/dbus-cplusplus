#!/bin/sh

function autocmd()
{
	echo "Running ${1}..."
	$* || {
		echo "Error running ${1}"
		exit 1
	}
}

autocmd aclocal
autocmd automake -acf
autocmd autoconf
autocmd libtoolize -f
autocmd autoheader

echo "Autogen done, now you can ./configure"
