#include "HuffmanCode.h"

int main(){

    int nCodeWeight[]={5,29,7,8,14,23,3,11};
    string strCode="ABCDEFGH";

    HuffmanCode huffmanCode;
    huffmanCode.code(nCodeWeight,strCode,8);
    huffmanCode.decode("111111110");

    huffmanCode.code("ABBCCCDDDDEEEEEE");
    huffmanCode.decode("010");
    return 1;

/*
i    w    p    l    r
1    5    0    0    0
2    29   0   0    0
3    7    0   0    0
4    8    0   0    0
5    14   0   0    0
6    23   0   0    0
7    3    0    0    0
8    11   11   0    0
9    0    0    0    0
10   0    0    0    0
11   0    0    0    0
12   0    0    0    0
13   0    0    0    0
14   0    0    0    0
15   0    0    0    0

Huffman Tree :
i    w    p    l    r
1    5    9    0    0
2    29   14   0    0
3    7    10   0    0
4    8    11   0    0
5    14   12   0    0
6    23   13   0    0
7    3    9    0    0
8    11   11   0    0
9    8    10   1    7
10   15   12   3    9
11   19   13   4    8
12   29   14   5    10
13   42   15   6    11
14   58   15   2    12
15   100  0    13   14

reverse code
11110
10
1110
010
110
00
11111
011

forward code
00
010
011
10
110
1110
11110
11111
*/

}
