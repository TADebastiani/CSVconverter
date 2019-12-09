# CSV Converter

Tool for convert a CSV-like file to another CSV-like file. The resulting file will be created at the same directory, changing the extension accordingly to the REPLACE choice

## Instalation

``` shell
$ make
$ make install
```

## Usage

Assuming the input file `people.csv`:

```
id,name,age
0,Joe,21
1,Doe,31
2,James,25
```

``` shell
$ CSVconvert people.csv -C -P
```

Result on `people.psv`:

```
id|name|age
0|Joe|21
1|Doe|31
2|James|25
```
