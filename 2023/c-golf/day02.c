c,p,P,i,n,r,g,b;main(){while(~(c=getchar()))c==44|c==58|c==59?n=0:c>47&
c<58?n=n*10+c-48:c=='d'?r=r>n?r:n:c=='g'?g=g>n?g:n:c==98?b=b>n?b:n:10==
c?p+=++i*(r<13&g<14&b<15),P+=r*g*b,r=g=b=0:0;printf("02: %d %d\n",p,P);}
