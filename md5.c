#define HOST_c2l(c,l)   (l =(((unsigned long)(*((c)++)))<<24),      \
             l|=(((unsigned long)(*((c)++)))<<16),      \
             l|=(((unsigned long)(*((c)++)))<< 8),      \
             l|=(((unsigned long)(*((c)++)))    ),      \
             l)

#define HOST_l2c(l,c)   (*((c)++)=(unsigned char)(((l)>>24)&0xff),  \
             *((c)++)=(unsigned char)(((l)>>16)&0xff),  \
             *((c)++)=(unsigned char)(((l)>> 8)&0xff),  \
             *((c)++)=(unsigned char)(((l)    )&0xff),  \
             l)


#define	F(b,c,d)	((((c) ^ (d)) & (b)) ^ (d))
#define	G(b,c,d)	((((b) ^ (c)) & (d)) ^ (c))
#define	H(b,c,d)	((b) ^ (c) ^ (d))
#define	I(b,c,d)	(((~(d)) | (b)) ^ (c))

#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

#define R0(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+F((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };\

#define R1(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+G((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R2(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+H((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R3(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+I((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

/* Implemented from RFC1321 The MD5 Message-Digest Algorithm
 */

#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

#define MD5_LONG unsigned int
#define MD5_LBLOCK 16

typedef unsigned int size_t;

typedef struct MD5state_st
{
	MD5_LONG A,B,C,D;
	MD5_LONG Nl,Nh;
	MD5_LONG data[MD5_LBLOCK];
	unsigned int num;
} MD5_CTX;

int MD5_Init(MD5_CTX *c)
{
	c->A=INIT_DATA_A;
	c->B=INIT_DATA_B;
	c->C=INIT_DATA_C;
	c->D=INIT_DATA_D;
	c->Nl=0;
	c->Nh=0;
	c->num=0;
	return 1;
}

void MD5_Update (MD5_CTX *c, const void *data_, size_t num)
{
	const unsigned char *data=data_;
	register unsigned long A,B,C,D,l;
	MD5_LONG XX[MD5_LBLOCK];
# define X(i)	XX[i]

	A=c->A;
	B=c->B;
	C=c->C;
	D=c->D;

	for (;num--;)
	{
		HOST_c2l(data,l); X( 0)=l;		HOST_c2l(data,l); X( 1)=l;
		/* Round 0 */
		R0(A,B,C,D,X( 0), 7,0xd76aa478L);	HOST_c2l(data,l); X( 2)=l;
		R0(D,A,B,C,X( 1),12,0xe8c7b756L);	HOST_c2l(data,l); X( 3)=l;
		R0(C,D,A,B,X( 2),17,0x242070dbL);	HOST_c2l(data,l); X( 4)=l;
		R0(B,C,D,A,X( 3),22,0xc1bdceeeL);	HOST_c2l(data,l); X( 5)=l;
		R0(A,B,C,D,X( 4), 7,0xf57c0fafL);	HOST_c2l(data,l); X( 6)=l;
		R0(D,A,B,C,X( 5),12,0x4787c62aL);	HOST_c2l(data,l); X( 7)=l;
		R0(C,D,A,B,X( 6),17,0xa8304613L);	HOST_c2l(data,l); X( 8)=l;
		R0(B,C,D,A,X( 7),22,0xfd469501L);	HOST_c2l(data,l); X( 9)=l;
		R0(A,B,C,D,X( 8), 7,0x698098d8L);	HOST_c2l(data,l); X(10)=l;
		R0(D,A,B,C,X( 9),12,0x8b44f7afL);	HOST_c2l(data,l); X(11)=l;
		R0(C,D,A,B,X(10),17,0xffff5bb1L);	HOST_c2l(data,l); X(12)=l;
		R0(B,C,D,A,X(11),22,0x895cd7beL);	HOST_c2l(data,l); X(13)=l;
		R0(A,B,C,D,X(12), 7,0x6b901122L);	HOST_c2l(data,l); X(14)=l;
		R0(D,A,B,C,X(13),12,0xfd987193L);	HOST_c2l(data,l); X(15)=l;
		R0(C,D,A,B,X(14),17,0xa679438eL);
		R0(B,C,D,A,X(15),22,0x49b40821L);
		/* Round 1 */
		R1(A,B,C,D,X( 1), 5,0xf61e2562L);
		R1(D,A,B,C,X( 6), 9,0xc040b340L);
		R1(C,D,A,B,X(11),14,0x265e5a51L);
		R1(B,C,D,A,X( 0),20,0xe9b6c7aaL);
		R1(A,B,C,D,X( 5), 5,0xd62f105dL);
		R1(D,A,B,C,X(10), 9,0x02441453L);
		R1(C,D,A,B,X(15),14,0xd8a1e681L);
		R1(B,C,D,A,X( 4),20,0xe7d3fbc8L);
		R1(A,B,C,D,X( 9), 5,0x21e1cde6L);
		R1(D,A,B,C,X(14), 9,0xc33707d6L);
		R1(C,D,A,B,X( 3),14,0xf4d50d87L);
		R1(B,C,D,A,X( 8),20,0x455a14edL);
		R1(A,B,C,D,X(13), 5,0xa9e3e905L);
		R1(D,A,B,C,X( 2), 9,0xfcefa3f8L);
		R1(C,D,A,B,X( 7),14,0x676f02d9L);
		R1(B,C,D,A,X(12),20,0x8d2a4c8aL);
		/* Round 2 */
		R2(A,B,C,D,X( 5), 4,0xfffa3942L);
		R2(D,A,B,C,X( 8),11,0x8771f681L);
		R2(C,D,A,B,X(11),16,0x6d9d6122L);
		R2(B,C,D,A,X(14),23,0xfde5380cL);
		R2(A,B,C,D,X( 1), 4,0xa4beea44L);
		R2(D,A,B,C,X( 4),11,0x4bdecfa9L);
		R2(C,D,A,B,X( 7),16,0xf6bb4b60L);
		R2(B,C,D,A,X(10),23,0xbebfbc70L);
		R2(A,B,C,D,X(13), 4,0x289b7ec6L);
		R2(D,A,B,C,X( 0),11,0xeaa127faL);
		R2(C,D,A,B,X( 3),16,0xd4ef3085L);
		R2(B,C,D,A,X( 6),23,0x04881d05L);
		R2(A,B,C,D,X( 9), 4,0xd9d4d039L);
		R2(D,A,B,C,X(12),11,0xe6db99e5L);
		R2(C,D,A,B,X(15),16,0x1fa27cf8L);
		R2(B,C,D,A,X( 2),23,0xc4ac5665L);
		/* Round 3 */
		R3(A,B,C,D,X( 0), 6,0xf4292244L);
		R3(D,A,B,C,X( 7),10,0x432aff97L);
		R3(C,D,A,B,X(14),15,0xab9423a7L);
		R3(B,C,D,A,X( 5),21,0xfc93a039L);
		R3(A,B,C,D,X(12), 6,0x655b59c3L);
		R3(D,A,B,C,X( 3),10,0x8f0ccc92L);
		R3(C,D,A,B,X(10),15,0xffeff47dL);
		R3(B,C,D,A,X( 1),21,0x85845dd1L);
		R3(A,B,C,D,X( 8), 6,0x6fa87e4fL);
		R3(D,A,B,C,X(15),10,0xfe2ce6e0L);
		R3(C,D,A,B,X( 6),15,0xa3014314L);
		R3(B,C,D,A,X(13),21,0x4e0811a1L);
		R3(A,B,C,D,X( 4), 6,0xf7537e82L);
		R3(D,A,B,C,X(11),10,0xbd3af235L);
		R3(C,D,A,B,X( 2),15,0x2ad7d2bbL);
		R3(B,C,D,A,X( 9),21,0xeb86d391L);

		A = c->A += A;
		B = c->B += B;
		C = c->C += C;
		D = c->D += D;
	}
}

#define HOST_p_c2l(c,l,n)   {                   \
            switch (n) {                    \
            case 0: l =((unsigned long)(*((c)++)))<<24; \
            case 1: l|=((unsigned long)(*((c)++)))<<16; \
            case 2: l|=((unsigned long)(*((c)++)))<< 8; \
            case 3: l|=((unsigned long)(*((c)++)));     \
                } }

#define HASH_MAKE_STRING(c,s)   do {    \
    unsigned long ll;       \
    ll=(c)->A; HOST_l2c(ll,(s));    \
    ll=(c)->B; HOST_l2c(ll,(s));    \
    ll=(c)->C; HOST_l2c(ll,(s));    \
    ll=(c)->D; HOST_l2c(ll,(s));    \
    } while (0)


int MD5_Final (unsigned char *md, MD5_CTX *c)
{
	register MD5_LONG *p;
	register unsigned long l;
	register int i,j;
	static const unsigned char end[4]={0x80,0x00,0x00,0x00};
	const unsigned char *cp=end;

	/* c->num should definitly have room for at least one more byte. */
	p=c->data;
	i=c->num>>2;
	j=c->num&0x03;

#if 0
	/* purify often complains about the following line as an
	 * Uninitialized Memory Read.  While this can be true, the
	 * following p_c2l macro will reset l when that case is true.
	 * This is because j&0x03 contains the number of 'valid' bytes
	 * already in p[i].  If and only if j&0x03 == 0, the UMR will
	 * occur but this is also the only time p_c2l will do
	 * l= *(cp++) instead of l|= *(cp++)
	 * Many thanks to Alex Tang <altitude@cic.net> for pickup this
	 * 'potential bug' */
#ifdef PURIFY
	if (j==0) p[i]=0; /* Yeah, but that's not the way to fix it:-) */
#endif
	l=p[i];
#else
	l = (j==0) ? 0 : p[i];
#endif
	HOST_p_c2l(cp,l,j); p[i++]=l; /* i is the next 'undefined word' */

	if (i>(MD5_LBLOCK-2)) /* save room for Nl and Nh */
	{
		if (i<MD5_LBLOCK) p[i]=0;
		MD5_Update (c,p,1);
		i=0;
	}
	for (; i<(MD5_LBLOCK-2); i++)
		p[i]=0;

#if   defined(DATA_ORDER_IS_BIG_ENDIAN)
	p[MD5_LBLOCK-2]=c->Nh;
	p[MD5_LBLOCK-1]=c->Nl;
#elif defined(DATA_ORDER_IS_LITTLE_ENDIAN)
	p[MD5_LBLOCK-2]=c->Nl;
	p[MD5_LBLOCK-1]=c->Nh;
#endif
	MD5_Update(c,p,1);

	HASH_MAKE_STRING(c,md);

	c->num=0;
	/* clear stuff, HASH_BLOCK may be leaving some stuff on the stack
	 * but I'm not worried :-)
	 OPENSSL_cleanse((void *)c,sizeof(HASH_CTX));
	 */
	return 1;
}

#include <stdio.h>

int main(int argc, char * argv[])
{
	FILE * fp;
	MD5_CTX ctx;
	char buf[1024];
	char md5[16];
	unsigned n;
	
	fp = fopen(argv[1], "r");
	
	MD5_Init(&ctx);

	while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
		// fwrite(buf, 1, n, stdout);
		MD5_Update(&ctx, buf, n);
	}

	MD5_Final(md5, &ctx);

	for (n = 0; n < sizeof(md5); n++) {
		printf("%02x", md5[n]);
	}
	printf("\n");

	fclose(fp), fp = NULL;

	return 0;
}
