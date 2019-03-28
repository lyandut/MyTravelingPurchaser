# Reading a TPPLIB file

A TPPLIB File is exactly a TSPLIB file + two more sections



## <span id="demsec">DEMAND_SECTION</span>

The number of products and the demand is specified in this section. An integer gives the number of products in the first line. Then, for each product, demand has to be given by mean the product's id followed by the demanded amount for this product.

- The first line is of the form
  `<integer>` 
- And each of the following lines is of the form
  `<integer> <integer> `



## <span id="offsec">OFFER_SECTION</span>

The offer of all markets are specified in the form (per line)

`<integer> <integer> [<integer> <integer> <integer>] ...`

where the first integer gives the market's id, the following integer the number of products offered by that market and as many triplets as the second integer.  Each integer of the triplet gives the product's id, product's cost and product's availability respectively. In the classical TPP particular instance the availability amount is 1.

Notice that the market with label "1" is always the depot, so no amount is offered in this market.



## EXAMPLE

```
NAME      :  SMALL TPP 
TYPE      :  TPP 
COMMENT   : EUCLIDEAN SMALL CTPP 
DIMENSION : 5 
EDGE_WEIGHT_TYPE   : EUC_2D 
DISPLAY_DATA_TYPE  : COORD_DISPLAY 
NODE_COORD_SECTION : 
1 445 258 
2 155 958 
3 383 849 
4 406 523 
5 463 414 
DEMAND_SECTION : 
5 
1  2 
2  3 
3  7 
4 11 
5 20 
OFFER_SECTION : 
1 0 
2 2  3  8  7   2 10 12 
3 3  5  9  3   4  9 10   1  1  3 
4 2  5  3 12   1  1  7 
5 2  5  8 14   4  4  3 
EOF
```



# TPP算例格式

| Section              | Type       | Explain                                                      |
| -------------------- | ---------- | ------------------------------------------------------------ |
| NAME                 | \<string>  | 算例名称, 标识数据文件                                       |
| TYPE                 | \<string>  | 算例类型, TPP, UTPP, RTPP(CTPP), ATPP, STPP                  |
| COMMENT              | \<string>  | 附加注释, 通常是算例贡献者的名字                             |
| DIMENSION            | \<integer> | 维数, 即节点个数                                             |
| EDGE_WEIGHT_TYPE     | \<string>  | 距离函数, 可选: EXPLICIT(明确给出), EUC_2D(二维空间欧式距离), EUC_3D(三维空间欧式距离), MAX_2D(二维空间中最大距离), MAN_2D(二维空间曼哈顿距离), GEO(实际距离)... |
| EDGE_WEIGHT_FORMAT   | \<string>  | 边的权重格式, 可选: FULL_MATRIX(全矩阵), UPPER_ROW(上三角、行优先、无对角线), LOWER_COL(下三角、列优先、无对角线), UPPER_DIAG_ROW(上三角、行优先、有对角线)... |
| EDGE_WEIGHT_SECTION  | 数据部分   | 按照`EDGE_WEIGHT_FORMAT`给出矩阵格式                         |
| EDGE_DATA_FORMAT     | \<string>  | 默认完全图`COMPLETE`；如果图不是完全的，则描述给出图的边格式，可选: EDGE_LIST(边列表描述), ADJ_LIST(邻接表描述)。 |
| EDGE_DATA_SECTION    | 数据部分   | 按照`EDGE_DATA_FORMAT`给出边的格式，如`EDGE_LIST`格式为`<integer> <integer>`；`ADJ_LIST`格式为`<integer> <integer> … <integer> -1`，其中第一个整数表示节点编号，后面的表示与该节点连接的节点编号，邻接表以-1结束。 |
| DISPLAY_DATA_TYPE    | \<string>  | 坐标产生方式, 可选: COORD_DISPLAY(点坐标), TWOD_DISPLAY(二维坐标), NO_DISPLAY(无) |
| DISPLAY_DATA_SECTION | 数据部分   | 按照`DISPLAY_DATA_TYPE`给出点坐标格式                        |
| NODE_COORD_TYPE      | \<string>  | 点坐标, 可选: TWOD_COORDS(二维坐标系), THREED_COORDS(三维坐标系), NO_COORDS(无坐标系). |
| NODE_COORD_SECTION   | 数据部分   | 按照`NODE_COORD_TYPE`给出点坐标, 如`TWOD_COORDS`格式为`<integer> <real> <real>`, 整数表示节点编号, 实数给出坐标系. |
| DEMAND_SECTION       | 数据部分   | 见[DEMAND_SECTION](#demsec)                                  |
| OFFER_SECTION        | 数据部分   | 见[OFFER_SECTION](#offsec)                                   |
| EOF                  | 结束标志   | 结束标志                                                     |

 