long p[128];n[128],c[4],i,j,z;C(int*a,int*b){return*a-*b;}main(){while(~
scanf("%d",++z+n));for(qsort(n,z,sizeof*n,C);++i<z;)c[n[i]-n[i-1]]++;for
(p[i=z-1]=1;i--;)for(j=i;++j<z&&n[j]-n[i]<4;)p[i]+=p[j];printf("%d %ld\
\n",c[1]*++c[3],*p);}
