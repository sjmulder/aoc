char*N[]={0,"one","two","three","four","five","six","seven","eight","nine"};p,P,
i,c,a,b;A,B;m[10];main(){while(~(c=getchar())){c==10?p+=a*10+b,P+=A*10+B,a=b=A=B
=0:0;c>47&&c<58?b=B=c-48,a||(a=b),A||(A=b):0;for(i=10;--i;)c^N[i][m[i]++]?m[i]=c
==*N[i]:!N[i][m[i]]?A||(A=i),B=i:0;}printf("01: %d %d\n",p,P);}
