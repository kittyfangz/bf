if ! type tcc > /dev/null
then
	gcc bf.c
else
	tcc bf.c
fi
