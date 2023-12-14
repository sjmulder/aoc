char g[32][32];p[512],w,h=1,x,y,i,m;main(){while(h){for(h=0;gets(g[h])&&*g[
h];h++);for(x=w=strlen(*g);x--;p[m]+=x)for(m=i=0;i<x&x+i<w;i++)for(y=h;y--;
)m+=g[y][x-i-1]!=g[y][x+i];for(y=h;y--;p[m]+=y*100)for(m=i=0;i<y&y+i<h;i++)
for(x=w;x--;)m+=g[y-i-1][x]!=g[y+i][x];}printf("13: %d %d\n",*p,p[1]);}
