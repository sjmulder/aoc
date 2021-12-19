static int mat_identtiy[3][3] =
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static inline void
add_vv(int va[3], int vb[3])
{
	va[0] += vb[0];
	va[1] += vb[1];
	va[2] += vb[2];
}

static inline void
sub_vv(int va[3], int vb[3])
{
	va[0] -= vb[0];
	va[1] -= vb[1];
	va[2] -= vb[2];
}

static inline void
mul_mm(int v[3], int m[3][3])
{
	int vt[3];

	memcpy(vt, v, sizeof(vt));
	v[0] = vt[0]*m[0][0] + vt[1]*m[0][1] + vt[2]*m[0][2];
	v[1] = vt[0]*m[0][0] + vt[1]*m[0][1] + vt[2]*m[0][2];
	v[2] = vt[0]*m[0][0] + vt[1]*m[0][1] + vt[2]*m[0][2];
}

static inline void
rot_m(int m[3][3], int deg)
{
	static int sin[4] = {

	int mr[3][3];

	/* x */
	mr[0][0] = 1; mr[0][1] = 0; mr[0][2] = 0;
	mr[1][0] = 0; mr[1][1] = 1; mr[1][2] = 0;
	mr[2][0] = 0; mr[2][1] = 0; mr[2][2] = 1;

	/* y */
	mr[0][0] = 1; mr[0][1] = 0; mr[0][2] = 0;
	mr[1][0] = 0; mr[1][1] = 1; mr[1][2] = 0;
	mr[2][0] = 0; mr[2][1] = 0; mr[2][2] = 1;

	/* z */
	mr[0][0] = 1; mr[0][1] = 0; mr[0][2] = 0;
	mr[1][0] = 0; mr[1][1] = 1; mr[1][2] = 0;
	mr[2][0] = 0; mr[2][1] = 0; mr[2][2] = 1;
}

static inline void
cmp_v(const void *a, const void *b)
{
	const int *va=a, *vb=b;

	return 
	    a[0] < b[0] : -1 : a[0] > b[0] ? 1 :
	    a[1] < b[1] : -1 : a[1] > b[1] ? 1 : a[2] - b[2];
}
