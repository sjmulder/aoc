#!/bin/bash

make 2> >(awk '
  / warning:/	{ print "##[warning]" $0 }
  / error:/	{ print "##[error]" $0 }
		{ print $0 }
' >&2) $@
