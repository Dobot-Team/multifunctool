/****************************************Copyright(c)*****************************************************
**                            Shenzhen Yuejiang Technology Co., LTD.
**
**                                 http://www.dobot.cc
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           md5.c
** Latest modified Date: 2019-11-08
** Latest Version:      V1.0.0
** Descriptions:
**
**--------------------------------------------------------------------------------------------------------
** Created by:          Deng xinjin
** Created date:        2019-11-08
** Version:             V1.0.0
** Descriptions:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "md5.h"

void MD5Transform(uint32_t state[4], uint8_t block[SIZE_OF_MD5_BLOCK]);
void MD5Encode(uint8_t *output, uint32_t *input, uint32_t len);
void MD5Decode(uint32_t *output, uint8_t *input, uint32_t len);

/*用于bits填充的缓冲区*/
uint8_t PADDING[] = {0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*********************************************************************************************************
** Function name:       MD5Init
** Descriptions:        初始化context信息
** Input parameters:    MD5_CTX
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void MD5Init(MD5_CTX *context)
{
    context->count[0] = 0;
    context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
}

/*********************************************************************************************************
** Function name:       MD5Update
** Descriptions:        把输入信息放入context的buffer元素中，并给count[0]赋上输入信息的长度值，单位是字节
**                      给buffer填充信息，需要三次
**                      1.把输入信息放入 
**                      2.把填充字段放入最后一个512字节凑够448字节 
**                      3.把输入信息长度存最后一个512字节的后64字节
**                      其中每一个转换后buffer里的长度都存入context-> count中。
**                      等三次后，开始执行MD5Transform()函数进行转换，转换后信息放入context-> state中
** Input parameters:    MD5_CTX
**                      *input写入buffer的指针
**                      inputLen是往buffer写入信息的长度
** Output parameters:   None
** Returned value:      None
*********************************************************************************************************/
void MD5Update(MD5_CTX *context, uint8_t *input, uint32_t inputlen)
{
    uint32_t i = 0, index = 0, partlen = 0;
    index = (context->count[0] >> 3) & 0x3F;
    partlen = SIZE_OF_MD5_BLOCK - index;
    context->count[0] += inputlen << 3;
    if (context->count[0] < (inputlen << 3))
    {
        context->count[1]++;
    }
    context->count[1] += inputlen >> 29;
    if (inputlen >= partlen)
    { //如果填充完毕，则进行换算
        memcpy(&context->buffer[index], input, partlen);
        MD5Transform(context->state, context->buffer);
        for (i = partlen; i + SIZE_OF_MD5_BLOCK <= inputlen; i += SIZE_OF_MD5_BLOCK)
        {
            MD5Transform(context->state, &input[i]);
        }
        index = 0;
    }
    else
    {
        i = 0;
    }
    /*进行填充*/
    memcpy(&context->buffer[index], &input[i], inputlen - i);
}

/*********************************************************************************************************
** Function name:       MD5Final
** Descriptions:        MD5计算的输出结果（16个字节的数据）
** Input parameters:    MD5_CTX
** Output parameters:   digest[16]：输出的结果
** Returned value:      None
*********************************************************************************************************/
void MD5Final(MD5_CTX *context, uint8_t digest[16])
{
    uint32_t index = 0, padlen = 0;
    uint8_t bits[8];
    /*输入信息的长度模512后的长度，即输入信息的最后512位占了几位*/
    index = (context->count[0] >> 3) & 0x3F;
    /*算出补充多少个位，就可以给最后一个512凑够448*/
    padlen = (index < 56) ? (56 - index) : (120 - index);
    /*把输入信息长度放入bit中*/
    MD5Encode(bits, context->count, 8);
    /*填充信息，使最后512位凑够448*/
    MD5Update(context, PADDING, padlen);
    /*在448后面填充输入信息长度，此次填充64位，凑够512，然后进行转换*/
    MD5Update(context, bits, 8);
    /*把state中信息转换到digest中，从而输出加密结果*/
    MD5Encode(digest, context->state, 16);
}

/*********************************************************************************************************
** Function name:       MD5Encode
** Descriptions:        获取MD5值的结果
** Input parameters:    input：欲转换的信息
**                      len：输入信息的数据长度（字节数，4的整数倍）
** Output parameters:   output：用于输出的字符缓冲区
** Returned value:      None
*********************************************************************************************************/
void MD5Encode(uint8_t *output, uint32_t *input, uint32_t len)
{
    uint32_t i = 0, j = 0;
    while (j < len)
    {
        output[j] = input[i] & 0xFF;
        output[j + 1] = (input[i] >> 8) & 0xFF;
        output[j + 2] = (input[i] >> 16) & 0xFF;
        output[j + 3] = (input[i] >> 24) & 0xFF;
        i++;
        j += 4;
    }
}

/*********************************************************************************************************
** Function name:       MD5Decode
** Descriptions:        把字符形式的缓冲区中的数据copy到4字节的整数中（即以整数形式保存）
** Input parameters:    input:欲转换的字符缓冲区
**                      len:输入的字符缓冲区的长度，要求是4的整数倍
** Output parameters:   output：保存转换出的整数
** Returned value:      None
*********************************************************************************************************/
void MD5Decode(uint32_t *output, uint8_t *input, uint32_t len)
{
    uint32_t i = 0, j = 0;
    while (j < len)
    {
        output[i] = (input[j]) |
                    (input[j + 1] << 8) |
                    (input[j + 2] << 16) |
                    (input[j + 3] << 24);
        i++;
        j += 4;
    }
}

/*********************************************************************************************************
** Function name:       MD5Transform
** Descriptions:        对512bits信息(即block缓冲区)进行一次处理，每次处理包括四轮。
** Input parameters:    block[64]：欲加密的512bits信息。
** Output parameters:   state[4]：md5结构中的state[4]，用于保存对512bits信息加密的中间结果或者最终结果。
** Returned value:      None
*********************************************************************************************************/
void MD5Transform(uint32_t state[4], uint8_t block[SIZE_OF_MD5_BLOCK])
{
    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t x[SIZE_OF_MD5_BLOCK];
    MD5Decode(x, block, SIZE_OF_MD5_BLOCK);
    FF(a, b, c, d, x[0], 7, 0xd76aa478);   /* 1 */
    FF(d, a, b, c, x[1], 12, 0xe8c7b756);  /* 2 */
    FF(c, d, a, b, x[2], 17, 0x242070db);  /* 3 */
    FF(b, c, d, a, x[3], 22, 0xc1bdceee);  /* 4 */
    FF(a, b, c, d, x[4], 7, 0xf57c0faf);   /* 5 */
    FF(d, a, b, c, x[5], 12, 0x4787c62a);  /* 6 */
    FF(c, d, a, b, x[6], 17, 0xa8304613);  /* 7 */
    FF(b, c, d, a, x[7], 22, 0xfd469501);  /* 8 */
    FF(a, b, c, d, x[8], 7, 0x698098d8);   /* 9 */
    FF(d, a, b, c, x[9], 12, 0x8b44f7af);  /* 10 */
    FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
    FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
    FF(a, b, c, d, x[12], 7, 0x6b901122);  /* 13 */
    FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
    FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
    FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */
    /* Round 2 */
    GG(a, b, c, d, x[1], 5, 0xf61e2562);   /* 17 */
    GG(d, a, b, c, x[6], 9, 0xc040b340);   /* 18 */
    GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
    GG(b, c, d, a, x[0], 20, 0xe9b6c7aa);  /* 20 */
    GG(a, b, c, d, x[5], 5, 0xd62f105d);   /* 21 */
    GG(d, a, b, c, x[10], 9, 0x2441453);   /* 22 */
    GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
    GG(b, c, d, a, x[4], 20, 0xe7d3fbc8);  /* 24 */
    GG(a, b, c, d, x[9], 5, 0x21e1cde6);   /* 25 */
    GG(d, a, b, c, x[14], 9, 0xc33707d6);  /* 26 */
    GG(c, d, a, b, x[3], 14, 0xf4d50d87);  /* 27 */
    GG(b, c, d, a, x[8], 20, 0x455a14ed);  /* 28 */
    GG(a, b, c, d, x[13], 5, 0xa9e3e905);  /* 29 */
    GG(d, a, b, c, x[2], 9, 0xfcefa3f8);   /* 30 */
    GG(c, d, a, b, x[7], 14, 0x676f02d9);  /* 31 */
    GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */
    /* Round 3 */
    HH(a, b, c, d, x[5], 4, 0xfffa3942);   /* 33 */
    HH(d, a, b, c, x[8], 11, 0x8771f681);  /* 34 */
    HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
    HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
    HH(a, b, c, d, x[1], 4, 0xa4beea44);   /* 37 */
    HH(d, a, b, c, x[4], 11, 0x4bdecfa9);  /* 38 */
    HH(c, d, a, b, x[7], 16, 0xf6bb4b60);  /* 39 */
    HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
    HH(a, b, c, d, x[13], 4, 0x289b7ec6);  /* 41 */
    HH(d, a, b, c, x[0], 11, 0xeaa127fa);  /* 42 */
    HH(c, d, a, b, x[3], 16, 0xd4ef3085);  /* 43 */
    HH(b, c, d, a, x[6], 23, 0x4881d05);   /* 44 */
    HH(a, b, c, d, x[9], 4, 0xd9d4d039);   /* 45 */
    HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
    HH(b, c, d, a, x[2], 23, 0xc4ac5665);  /* 48 */
    /* Round 4 */
    II(a, b, c, d, x[0], 6, 0xf4292244);   /* 49 */
    II(d, a, b, c, x[7], 10, 0x432aff97);  /* 50 */
    II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
    II(b, c, d, a, x[5], 21, 0xfc93a039);  /* 52 */
    II(a, b, c, d, x[12], 6, 0x655b59c3);  /* 53 */
    II(d, a, b, c, x[3], 10, 0x8f0ccc92);  /* 54 */
    II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
    II(b, c, d, a, x[1], 21, 0x85845dd1);  /* 56 */
    II(a, b, c, d, x[8], 6, 0x6fa87e4f);   /* 57 */
    II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
    II(c, d, a, b, x[6], 15, 0xa3014314);  /* 59 */
    II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
    II(a, b, c, d, x[4], 6, 0xf7537e82);   /* 61 */
    II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
    II(c, d, a, b, x[2], 15, 0x2ad7d2bb);  /* 63 */
    II(b, c, d, a, x[9], 21, 0xeb86d391);  /* 64 */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}
