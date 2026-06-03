# Release Notes

## Version Mapping

### Product Version

<a name="table62675726"></a>
<table><tbody><tr id="row41561572"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.1.1"><p id="p11044137"><a name="p11044137"></a><a name="p11044137"></a>Product Name</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.1.1 "><p id="p1597721693713"><a name="p1597721693713"></a><a name="p1597721693713"></a>Kunpeng BoostKit</p>
</td>
</tr>
<tr id="row24726251"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.2.1"><p id="p56669300"><a name="p56669300"></a><a name="p56669300"></a>Product Version</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.2.1 "><p id="p11923034"><a name="p11923034"></a><a name="p11923034"></a><span id="text14311218114"><a name="text14311218114"></a><a name="text14311218114"></a>26.0.RC1</span></p>
</td>
</tr>
<tr id="row1930811171892"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.3.1"><p id="p2030912172097"><a name="p2030912172097"></a><a name="p2030912172097"></a>Software Name</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.3.1 "><p id="p1730912179911"><a name="p1730912179911"></a><a name="p1730912179911"></a><span id="text17191017111119"><a name="text17191017111119"></a><a name="text17191017111119"></a>Protobuf (optimized Kunpeng Protobuf)</span></p>
</td>
</tr>
<tr id="row1930811171892"><th class="firstcol" valign="top" width="42.17%" id="mcps1.1.3.3.1"><p id="p2030912172097"><a name="p2030912172097"></a><a name="p2030912172097"></a>Software Version</p>
</th>
<td class="cellrowborder" valign="top" width="57.830000000000005%" headers="mcps1.1.3.3.1 "><p id="p1730912179911"><a name="p1730912179911"></a><a name="p1730912179911"></a><span id="text17191017111119"><a name="text17191017111119"></a><a name="text17191017111119"></a>V1.0.0</span></p>
</td>
</tr>
</tbody>
</table>

### OS, Compiler, and CPU

|OS|CPU|Compiler|
|--|--|--|
|openEuler 22.03 LTS SP3|Kunpeng 920|Clang 16.0.6+<br>GCC 9.0+|

## Version Updates

### V1.0.0

**New Features**

|Feature|Change Description|
|--|--|
|Added varint encoding and decoding optimization.|Optimized the varint encoding and decoding paths in `coded_stream.h` and `parse_context.h` by loop unrolling and vectorization, to improve the performance of packed array encoding and decoding.|

## Related Documentation

### V1.0.0 Related Documentation

<a name="table41916133"></a>
<table><thead align="left"><tr id="row28804032"><th class="cellrowborder" valign="top" width="45.019999999999996%" id="mcps1.1.4.1.1"><p id="p4697041"><a name="p4697041"></a><a name="p4697041"></a>Document Name</p>
</th>
<th class="cellrowborder" valign="top" width="38.019999999999996%" id="mcps1.1.4.1.2"><p id="p44916036"><a name="p44916036"></a><a name="p44916036"></a>Overview</p>
</th>
<th class="cellrowborder" valign="top" width="16.96%" id="mcps1.1.4.1.3"><p id="p14320308"><a name="p14320308"></a><a name="p14320308"></a>Delivery Method</p>
</th>
</tr>
</thead>
<tbody><tr id="row19094280">
<td class="cellrowborder" valign="top" width="40.510000000000005%" headers="mcps1.1.4.1.1"><p id="p1341193722116">Release Notes</p></td>
<td class="cellrowborder" valign="top" width="38.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p2042183752117"><a name="p2042183752117"></a><a name="p2042183752117"></a>Provides basic information and feature updates of each version of optimized Kunpeng Protobuf.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p12419194564814"><a name="p12419194564814"></a><a name="p12419194564814"></a>Open-source repository</p>
</td>
</tr>
<tr id="row17314122818119"><td class="cellrowborder" valign="top" width="45.019999999999996%" headers="mcps1.1.4.1.1 "><p id="p9534164291118"><a name="p9534164291118"></a><a name="p9534164291118"></a>Installation Guide</p>
</td>
<td class="cellrowborder" valign="top" width="38.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p1353414214111"><a name="p1353414214111"></a><a name="p1353414214111"></a>Provides detailed guidance on environment configuration, compilation, and installation of optimized Kunpeng Protobuf code.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p1164511541488"><a name="p1164511541488"></a><a name="p1164511541488"></a>Open-source repository</p>
</td>
</tr>
<tr id="row1021013311110"><td class="cellrowborder" valign="top" width="45.019999999999996%" headers="mcps1.1.4.1.1 "><p id="p18535154291116"><a name="p18535154291116"></a><a name="p18535154291116"></a>Quick Start</p>
</td>
<td class="cellrowborder" valign="top" width="38.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p1353554215117"><a name="p1353554215117"></a><a name="p1353554215117"></a>Provides quick start examples and compilation instructions for optimized Kunpeng Protobuf.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p2646175434816"><a name="p2646175434816"></a><a name="p2646175434816"></a>Open-source repository</p>
</td>
</tr>

<tr id="row1021013311110"><td class="cellrowborder" valign="top" width="45.019999999999996%" headers="mcps1.1.4.1.1 "><p id="p18535154291116"><a name="p18535154291116"></a><a name="p18535154291116"></a>API Reference</p>
</td>
<td class="cellrowborder" valign="top" width="38.019999999999996%" headers="mcps1.1.4.1.2 "><p id="p1353554215117"><a name="p1353554215117"></a><a name="p1353554215117"></a>Provides descriptions and definitions of APIs related to varint encoding and decoding optimization.</p>
</td>
<td class="cellrowborder" valign="top" width="16.96%" headers="mcps1.1.4.1.3 "><p id="p2646175434816"><a name="p2646175434816"></a><a name="p2646175434816"></a>Open-source repository</p>
</td>
</tr>
</tbody>
</table>

### Obtaining Documentation

Visit the [open-source repository](https://gitcode.com/boostkit/protobuf) to view or download related documents.
