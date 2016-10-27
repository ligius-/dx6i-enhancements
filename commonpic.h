typedef unsigned char byte;
typedef unsigned int uint16;


#define tris(x) tris##x
#define portz(x) port##x

#define out(x) tris(x)=0
#define in(x) tris(x)=1
#define set(x) portz(x)=1
#define unset(x) portz(x)=0
#define set1(x) out(x);portz(x)=1
#define set0(x) out(x);portz(x)=0
#define get(x) portz(x)

#define BIT(x) (1 << (x))
#define SETBITS(x,y) ((x) |= (y))
#define CLEARBITS(x,y) ((x) &= (~(y)))
#define SETBIT(x,y) SETBITS((x), (BIT((y))))
#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))
#define BITSET(x,y) ((x) & (BIT(y)))
#define BITCLEAR(x,y) !BITSET((x), (y))
#define BITSSET(x,y) (((x) & (y)) == (y))
#define BITSCLEAR(x,y) (((x) & (y)) == 0)
#define BITVAL(x,y) (((x)>>(y)) & 1) 