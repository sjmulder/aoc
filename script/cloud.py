#!/usr/bin/env python3
from wordcloud import WordCloud

wc = WordCloud(background_color="white",
               regexp=r"[^\s]+",
               collocations=False,
               normalize_plurals=False,
               width=800,
               height=400)
wc.generate(open("varnames.txt").read())
wc.to_file('cloud.png')
